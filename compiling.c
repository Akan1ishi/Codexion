#include "coders/codexion.h"

mutex_op	get_mutex_op(t_signal signal)
{
	if (signal == LOCK)
		return pthread_mutex_lock;
	else if (signal == UNLOCK)
		return pthread_mutex_unlock;
	return NULL;
}

void	manage_dongles(t_coder *coder, t_signal signal)
{
	mutex_op	mutex_handler;

	mutex_handler = get_mutex_op(signal);
	if (coder->id % 2 == 0)
	{
		mutex_handler(&coder->left->mutex);
		mutex_handler(&coder->right->mutex);
	}
	else
	{
		mutex_handler(&coder->right->mutex);
		mutex_handler(&coder->left->mutex);
	}
	if (signal == LOCK)
	{
		coder->right->free = FALSE;
		coder->left->free = FALSE;
	}
}

void	inscribe_dongle_data(t_coder *coder)
{
	struct timeval	tz;

	gettimeofday(&tz, NULL);
	//printf("[DONGLE DEBUG]\n%ld %d\n", tz.tv_sec, tz.tv_usec);
	tz.tv_sec += coder->data.dongle_cooldown / 1000;
	tz.tv_usec += (coder->data.dongle_cooldown * 1000) % 1000000;
	//printf("[AFTER COOLDOWN]\nCD=%d %ld %d\n", coder->data.dongle_cooldown, tz.tv_sec, tz.tv_usec);
	coder->right->next_free = tz;
	coder->left->next_free = tz;
	coder->right->free = TRUE;
	coder->left->free = TRUE;
}
