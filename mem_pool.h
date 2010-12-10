#ifndef MEM_POOL_H
#define MEM_POOL_H

struct slab {
    void *cache;
    void *free;
    struct slab *next;
};

typedef struct slab slab_t;

struct mem_pool {
    int obj_size;
    int real_obj_size;
    int cache_size;
    slab_t *slab_head;
};

typedef struct mem_pool mem_pool_t;

mem_pool_t *create_mem_pool(int obj_size);
void destroy_mem_pool(mem_pool_t *mpl);

void *mem_pool_alloc(mem_pool_t *mpl);

void mem_pool_free(mem_pool_t *mpl, void *mem);

#endif
