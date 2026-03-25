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

void	log(int id, t_type operation, t_coder_info data)
{
	struct timeval	tz;

	gettimeofday(&tz, NULL);
	pthread_mutex_lock(coder.output_mutex);
	if (operation == COMPILING)
		printf("%lld %d is compiling\n", get_actual_time_ms(coder.start_time, tz), coder.id);	
	else if (operation == REFACTORING)
		printf("%lld %d is refactoring\n", get_actual_time_ms(coder.start_time, tz), coder.id);	
	else if (operation == DEBUGGING)
		printf("%lld %d is debugging\n", get_actual_time_ms(coder.start_time, tz), coder.id);	
	else if (operation == TAKE)
		printf("%lld %d has taken a dongle\n", get_actual_time_ms(coder.start_time, tz), coder.id);	
	pthread_mutex_unlock(coder.output_mutex);
}

void	wait(t_type operation, t_coder_info data)
{
	if (operation == COMPILING)
		usleep(data.compiling_time * 1000);
	else if (operation == REFACTORING)
		usleep(data.refactoring_time * 1000);
	else
		usleep(data.debugging_time * 1000);
}



