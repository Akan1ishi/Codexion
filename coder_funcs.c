#include "coders/codexion.h"

long long	get_actual_time_ms(struct timeval beginning, struct timeval current)
{
	long long	seconds;
	long long	u_seconds;

	seconds = current.tv_sec - beginning.tv_sec;
	u_seconds = current.tv_usec - beginning.tv_usec;
	if (u_seconds < 0)
	{
		seconds -= 1;
		u_seconds += 1000000;
	}
	return (seconds * 1000 + u_seconds / 1000);
}

void	log_status(t_coder *coder, t_type operation)
{
	struct timeval	tz;
	long long	actual_time;

	gettimeofday(&tz, NULL);
	actual_time = get_actual_time_ms(coder->start_time, tz);
	pthread_mutex_lock(coder->output_mutex);
	if (operation == COMPILING)
		printf("%lld %d is compiling\n", actual_time, coder->id);	
	else if (operation == REFACTORING)
		printf("%lld %d is refactoring\n", actual_time, coder->id);	
	else if (operation == DEBUGGING)
		printf("%lld %d is debugging\n", actual_time, coder->id);	
	else if (operation == TAKE)
		printf("%lld %d has taken a dongle\n", actual_time, coder->id);	
	pthread_mutex_unlock(coder->output_mutex);
}

void	wait_cooldown(t_type operation, t_coder_info data)
{
	if (operation == COMPILING)
		usleep(data.compile_time * 1000);
	else if (operation == REFACTORING)
		usleep(data.refactor_time * 1000);
	else
		usleep(data.debug_time * 1000);
}

void	work_schedule(t_coder *coder, t_type operation)
{
	if (operation != COMPILING)
	{
		log_status(coder, operation);
		wait_cooldown(operation, coder->data);
		return ;
	}
	manage_dongles(coder, LOCK);
	pthread_mutex_unlock(coder->global_mutex);
	log_status(coder, operation);
	wait_cooldown(operation, coder->data);
	pthread_mutex_lock(coder->global_mutex);
	*coder->nb_compiles += 1;
	//printf("[DEBUG] id=%d, compiles=%d\n", coder->id, *coder->nb_compiles);
	inscribe_dongle_data(coder);
	pthread_mutex_unlock(coder->global_mutex);
	manage_dongles(coder, UNLOCK);
	pthread_cond_broadcast(coder->global_cond);
}
