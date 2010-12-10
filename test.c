#include <string.h>
#include "mem_pool.h"

const int SIZE = 16*1024;

int main(int argc, char* argv[])
{
    mem_pool_t *mpl;
    void *new;
    int i;
    void *array[1000];
       
    mpl = create_mem_pool(SIZE);

    for (i = 0; i < 820; i++) {
        new = mem_pool_alloc(mpl);
        if (new) {
            memset(new, 'a', SIZE);
            array[i] = new;
        }
    }

    for (i = 0; i < 10; i++) {
        if (array[i]) {
            mem_pool_free(mpl, array[i]);
        }
    }
    for (i = 10; i < 30; i++) {
        if (array[i]) {
            mem_pool_free(mpl, array[i]);
        }
    }
    for (i = 30; i < 820; i++) {
        if (array[i]) {
            mem_pool_free(mpl, array[i]);
        }
    }

    destroy_mem_pool(mpl);

    return 0;
}
