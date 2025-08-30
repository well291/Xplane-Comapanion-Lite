#include "hl_types.h"
#include "UTL_thread.h"

#include <pthread.h>

#define APP_alert_oserr(__Err, __Proc)	\
	if ((__Err) != 0)						\
			APP_alert(#__Proc " OS error", int_to_str(err),"","")

#define	FATAL_TIMEOUT_MS		5000

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// GENERAL UTILITIES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
UTL_thread_id		UTL_get_current_thread(void)
{
	return pthread_self();
}

xint				UTL_count_processors(void)
{
	return 1;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// THREADS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// Threads - when we join a thread, we wait until that thread finishes.
// When we 'stop' a thread, it just stops.  stop is provided for the purpose of
// debugging, e.g. "oh no we're screwed" - we can stop our threads so no one else is
// running, then safely print a lot of stuff, then quit.
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••

void				UTL_thread_create(
							UTL_threadproc_t	in_proc,
							void *				in_param,
							UTL_thread *		out_thread,
							UTL_thread_id *		out_id,
							UTL_thread_joinable*out_join)
{
	xint err = pthread_create(out_thread, NULL, in_proc, in_param);
	APP_alert_oserr(err, "pthread_create");
	*out_join = *out_id = *out_thread;

}

void *				UTL_thread_join(
							UTL_thread *		in_thread,
							UTL_thread_joinable*in_join)
{
	void* stat;
	int err = pthread_join(*in_thread, &stat);

	if(DEV && err)
		APP_alert("Warning: Wait on thread clean-up failed!","","","");

	return stat;
}

void	UTL_thread_exit(void * ret_val)
{
		pthread_exit(ret_val);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CRITICAL SECTIONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••

void				UTL_critical_section_create(
							UTL_critical_section *	out_critical_section)
{
	xint err;

	pthread_mutexattr_t	attr;
	err = pthread_mutexattr_init(&attr);
	APP_alert_oserr(err,"::pthread_mutexattr_init");
	err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	APP_alert_oserr(err,"::pthread_mutexattr_settype");
	err = pthread_mutex_init(out_critical_section, &attr);
	APP_alert_oserr(err,"::pthread_mutex_init");
	err = pthread_mutexattr_destroy(&attr);
	APP_alert_oserr(err,"::pthread_mutexattr_destroy");
}

void				UTL_critical_section_destroy(
							UTL_critical_section *	in_critical_section)
{
	xint err;
	err = pthread_mutex_destroy(in_critical_section);
	APP_alert_oserr(err,"::pthread_mutex_destroy");
}

void				UTL_critical_section_lock(
							UTL_critical_section *	in_critical_section)
{
	xint err;
	err = pthread_mutex_lock(in_critical_section);
	APP_alert_oserr(err, "pthread_mutex_lock");
}

void				UTL_critical_section_unlock(
							UTL_critical_section *	in_critical_section)
{
	xint err;
	err = pthread_mutex_unlock(in_critical_section);
	APP_alert_oserr(err, "pthread_mutex_unlock");

}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MESSAGE QUEUE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
struct	UTL_message_queue
{
		pthread_mutex_t		lock;
	  	pthread_cond_t		queue;
	  	list<void *>		messages;
	  	int					threads;

};

void				UTL_message_queue_create(
							UTL_message_queue ** 	queue)
{
	*queue = new UTL_message_queue;

	xint err;
	(*queue)->threads = 0;
	err = pthread_mutex_init(&(*queue)->lock, NULL);
	APP_alert_oserr(err,"::pthread_mutex_init");

	err = pthread_cond_init(&(*queue)->queue, NULL);
	APP_alert_oserr(err,"::pthread_cond_init");

}

void				UTL_message_queue_destroy(
							UTL_message_queue * 	queue)
{
	xint err;
	err = pthread_mutex_destroy(&queue->lock);
	APP_alert_oserr(err,"::pthread_mutex_destroy");

	err = pthread_cond_destroy(&queue->queue);
	APP_alert_oserr(err,"::pthread_cond_desroy");

	delete queue;
}

void				UTL_message_queue_write(
							UTL_message_queue * 	queue,
							void *					in_message)
{
	xint err;

	err = pthread_mutex_lock(&queue->lock);
	APP_alert_oserr(err,"pthread_mutex_lock");

	if (queue->threads > 0)
	{
		err = pthread_cond_signal(&queue->queue);
		APP_alert_oserr(err,"pthread_cond_signal");
	}

	queue->messages.push_back(in_message);

	err = pthread_mutex_unlock(&queue->lock);
	APP_alert_oserr(err,"pthread_mutex_unlock");
}

void				UTL_message_queue_multi_write(
							UTL_message_queue * 	queue,
							void *					in_messages[],
							xint					in_message_count,
							xint					in_front)
{
	xint err;

	err = pthread_mutex_lock(&queue->lock);
	APP_alert_oserr(err,"pthread_mutex_lock");

	if (queue->threads > 0)
	{
		xint count = min(queue->threads, in_message_count);		// One signal for each message or waiting thread; signal is guaranteed to wake up AT LEAST
		while(count--) {										// one thread.  If another thread slips through we get zero or more false wakeups.  Since we are
			err = pthread_cond_signal(&queue->queue);			// holding the lock, we know the number of threads waiting while we do this.  No one can START
			APP_alert_oserr(err,"pthread_cond_signal");		// waiting in addition to us until we release the lock.  (This is why the condition idiom has
		}														// waiters change the lock count and signalers signal while the mutex is held.)
	}

	queue->messages.insert(in_front ? queue->messages.begin() : queue->messages.end(), in_messages, in_messages + in_message_count);

	err = pthread_mutex_unlock(&queue->lock);
	APP_alert_oserr(err,"pthread_mutex_unlock");
}

xint				UTL_message_queue_edit(
							UTL_message_queue *			queue,
							UTL_message_queue_edit_f	edit_func,
							void *						ref)
{
	xint err = pthread_mutex_lock(&queue->lock);
	APP_alert_oserr(err,"pthread_mutex_lock");

	vector<void *> msgs(queue->messages.begin(),queue->messages.end());
	xint r = edit_func(msgs,ref);
	// Edit cannot ADD messages - since we do not "wake up" a thread, we would be "stuck".
//	dev_assert(msgs.size() <= queue->messages.size());
	queue->messages.clear();
	queue->messages.insert(queue->messages.end(),msgs.begin(),msgs.end());

	err = pthread_mutex_unlock(&queue->lock);
	APP_alert_oserr(err,"pthread_mutex_unlock");

	return r;
}

void* UTL_message_queue_read(
							UTL_message_queue * 	queue,
							xint					in_wait_ms)
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// Ben sez: it might not be obvious why this has to be a WHILE loop!!!!  This queue is based on this article:
// http://www.cs.wustl.edu/~schmidt/win32-cv-1.html
// Here is why this is necessary though: the READER decrements the  "waiting thread" variable.  But the WRITER uses this variable to know if it should signal. So:
// The first reader blocks on the queue
// The writer signals the semaphore and queues a message.
// The first reader is now unblocked but doesn't get to run
// A second reader goes in and sees the queue is full and doesn't block at all.
// The first reader now gets to run but its message is STOLEN!  SCHEISE!!!!
// So...the first reader has to go to bed and try again.
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••

	xint err;
	void * ret;
	struct timespec ts = { 0 };
	if(in_wait_ms != UTL_wait_forever && in_wait_ms != 0)
	{
		struct timeval end_time;												// pthreads uses an absolute time spec.  This is actually good (even though it is annoying)
		struct timezone tzp = { 0 };											// because if we wake up early and have to go back to sleep (see above comment about "stolen"
		gettimeofday(&end_time, &tzp);											// messages) our absolute time-out deadline is still the same...thus we cannot sleep for longer
		TIMEVAL_TO_TIMESPEC(&end_time, &ts);									// than requested due to multiple wake-ups.
		ts.tv_sec += (in_wait_ms / 1000);
		ts.tv_nsec += (in_wait_ms % 1000) * 1000000;
		if (ts.tv_nsec >= 1000000000) { ts.tv_nsec -= 1000000000; ts.tv_sec++; }

	}

	err = pthread_mutex_lock(&queue->lock);
	APP_alert_oserr(err,"pthread_mutex_lock");

	if (in_wait_ms == 0 && queue->messages.empty())								// No-wait case...if we are not going to sleep and we don't have a message, do not call pthread_cond_timedwait.
	{																			// Instead simply unlock the queue and go home empty.  (Since the message queue is locked, we can trust that if
		err = pthread_mutex_unlock(&queue->lock);								// our wait time IS zero and we DO have a message, we will never run through the while loop, because no one can
		APP_alert_oserr(err,"pthread_mutex_unlock");							// message with the message queue.)  This case reduces the "quick check" for finished work to a single critical
		return NULL;															// section - rather than a full blocking thread-op.
	}

	queue->threads++;

	while (queue->messages.empty())
	{
		if (in_wait_ms == UTL_wait_forever)		err = pthread_cond_wait(&queue->queue, &queue->lock);
		else									err = pthread_cond_timedwait( &queue->queue, &queue->lock, &ts);

		if (err == ETIMEDOUT)													// Time-out case...decrement the waiting thread count (so we don't get extra signals being sent) and just bail out.
		{
			queue->threads--;
			err = pthread_mutex_unlock(&queue->lock);
			APP_alert_oserr(err,"pthread_mutex_unlock");
			return NULL;
		}
		APP_alert_oserr(err,"pthread_cond_timedwait");
	}

	queue->threads--;															// If we made it to here, we have the lock, and the message queue is not empty - and we didn't time out. 
	ret = queue->messages.front();												// In otherwords, a successful queue read.  Decrement the thread count, read the message, dump the lock and go home
	queue->messages.pop_front();												// happy.

	err = pthread_mutex_unlock(&queue->lock);
	APP_alert_oserr(err,"pthread_mutex_unlock");
	return ret;
}
