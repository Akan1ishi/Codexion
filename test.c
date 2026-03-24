#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_control
{
	long long time;
}	t_control;

typedef struct s_printer
{
	pthread_t	thread;
	int	id;
	int rounds;
	long long *time;
}t_printer;

void	*timer(void *arg)
{
	t_control	controller;
	struct	timeval	tz;
	long long	start_time;

	controller = *(t_control *)arg;
	gettimeofday(&tz, NULL);
	start_time = (long long) tz.tv_sec * 1000;
	while(1)
	{
		gettimeofday(&tz, NULL);
		*&controller.time = (long long) (tz.tv_sec * 1000 ) - start_time;
	}
	return NULL;
}

void	*print(void *arg)
{
	t_printer	printer;

	printer = *(t_printer *) arg;
	while (1)
	{
		printf("[Printer %d] || Round [%d]|| Time[%lld]\n", printer.id, printer.rounds + 1, *printer.time / 1000);
		printer.rounds += 1;
		if (printer.rounds == 20)
			return NULL;
	}
	return NULL;
}

int main(void)
{
	pthread_t	timer_thread;
	t_control	controller;
	t_printer	*printers;
	int		nb_printers;

	nb_printers = 3;
	controller.time = 0;
	printers = malloc(sizeof(t_printer) * nb_printers);
	for (int i = 0; i < nb_printers; i++)
	{
		printers[i].id = i;
		printers[i].time = &controller.time;
		printers[i].rounds = 0;
	}
	pthread_create(&timer_thread, NULL, timer, (void *) &controller);
	for (int i = 0; i < nb_printers; i++)
		pthread_create(&printers[i].thread, NULL, print, (void *) &printers[i]);
	for (int i = 0; i < nb_printers; i++)
		pthread_join(printers[i].thread, NULL);
	free(printers);
	return 0;
}

