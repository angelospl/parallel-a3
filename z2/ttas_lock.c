#include "lock.h"
#include "../common/alloc.h"

typedef enum{
        UNLOCKED=0,
        LOCKED
}lock_state_t;

struct lock_struct {
        lock_state_t state;
};

lock_t *lock_init(int nthreads)
{
        lock_t *lock;

        XMALLOC(lock, 1);
        /* other initializations here. */
        lock->state = UNLOCKED;
        return lock;
}

void lock_free(lock_t *lock)
{
        XFREE(lock);
}

void lock_acquire(lock_t *lock)
{
        lock_t *ttas_lock = lock;
        while(1){
                while(ttas_lock->state == LOCKED) {};
                if(__sync_lock_test_and_set(&ttas_lock->state, LOCKED) != LOCKED)
                        return;
        }
}

void lock_release(lock_t *lock)
{
        lock_t *ttas_lock = lock;
        __sync_lock_release(&ttas_lock->state);
}
