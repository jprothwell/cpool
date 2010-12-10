#include <stdio.h>
#include <stdlib.h>
#include "mem_pool.h"

const size_t OBJ_NUM = 128;

void set_freepointer(mem_pool_t *mpl, void *obj, void *fp)
{
    *(void **)(obj + mpl->obj_size) = fp;
}

void *get_freepointer(mem_pool_t *mpl, void *obj)
{
    return *(void **)(obj + mpl->obj_size);
}

void init_slab(mem_pool_t *mpl, slab_t *slab)
{
    void *start = slab->cache;
    void *end = slab->cache + mpl->cache_size;

    slab->free = start;
    for (; start < end; start += mpl->real_obj_size) {
        set_freepointer(mpl, start, start + mpl->real_obj_size);
    }

    set_freepointer(mpl, start - mpl->real_obj_size, NULL);
}

slab_t *add_new_slab(mem_pool_t *mpl)
{
    slab_t *new_slab;

    new_slab = malloc(sizeof(slab_t));
    new_slab->cache = malloc(mpl->cache_size);

    init_slab(mpl, new_slab);

    new_slab->next = mpl->slab_head;
    mpl->slab_head = new_slab;

    return new_slab;
}

void free_slab(slab_t *slab)
{
    free(slab->cache);
    free(slab);
}

mem_pool_t *create_mem_pool(int obj_size)
{
    mem_pool_t *mpl = malloc(sizeof(mem_pool_t));

    if (obj_size % sizeof(long)) {
        mpl->obj_size = ( obj_size / sizeof(long) + 1 ) * sizeof(long);
    }
    else {
        mpl->obj_size = obj_size;
    }
    mpl->real_obj_size = mpl->obj_size + sizeof(void*);
    mpl->cache_size = OBJ_NUM * mpl->real_obj_size;

    mpl->slab_head = NULL;
}

void destroy_mem_pool(mem_pool_t *mpl)
{
    slab_t *slab;

    if (mpl) {
        slab = mpl->slab_head;
        while (slab) {
            mpl->slab_head = slab->next;
            free_slab(slab);
            slab = mpl->slab_head;
        }
        free(mpl);
    }
}

void *mem_pool_alloc(mem_pool_t *mpl)
{
    slab_t *found_slab;
    void *object;

    found_slab = mpl->slab_head;
    while (found_slab) {
        if (found_slab->free) {
            goto found;
        }
        found_slab = found_slab->next;
    }

    found_slab = add_new_slab(mpl);

found:
    object = found_slab->free;
    found_slab->free = get_freepointer(mpl, object);
    set_freepointer(mpl, object, found_slab);

    return object;
}    

void mem_pool_free(mem_pool_t *mpl, void *object)
{
    slab_t *slab;

    slab = (slab_t*)get_freepointer(mpl, object);
    set_freepointer(mpl, object, slab->free);
    slab->free = object;
}
