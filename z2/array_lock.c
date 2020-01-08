#include "lock.h"
#include "../common/alloc.h"


struct lock_struct {
        int size;
        int flag[64];
				int tail;
};

__thread unsigned long int myslot;      //thread local integer variable


lock_t *lock_init(int nthreads)
{
        lock_t *lock;
        int i;
        XMALLOC(lock, 1);

        lock->size = nthreads;
        for (i=1;i<lock->size;i++) lock->flag[i] = 0;
        lock->flag[0]=1;
        lock->tail = 0;

        return lock;
}

void lock_free(lock_t *lock)
{
        XFREE(lock);
}

void lock_acquire(lock_t *lock)
{
        myslot = __sync_fetch_and_add(&lock->tail,1);
        while(!(lock->flag[myslot%lock->size])){};
}

void lock_release(lock_t *lock)
{
        lock->flag[myslot%lock->size] = 0;
        lock->flag[(myslot+1)%lock->size] = 1;
}
