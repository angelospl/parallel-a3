#include "lock.h"
#include "../common/alloc.h"
#include <pthread.h>

struct lock_struct {
	pthread_spinlock_t *p_lock;
};

lock_t *lock_init(int nthreads)
{
	lock_t *lock;

	XMALLOC(lock, 1);
	pthread_spin_init(&(lock->p_lock),PTHREAD_PROCESS_SHARED);
	return lock;
}

void lock_free(lock_t *lock)
{
	pthread_spin_destroy(&(lock->p_lock));
	XFREE(lock);
}

void lock_acquire(lock_t *lock)
{
	pthread_spin_lock(&(lock->p_lock));
}

void lock_release(lock_t *lock)
{
	pthread_spin_unlock(&(lock->p_lock));
}
