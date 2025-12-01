/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:31:44 by jarregui          #+#    #+#             */
/*   Updated: 2025/12/01 19:15:10 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*monitor_routine(void *arg)
{
	t_philo	*philos;
	t_rules	*rules;
	long	now;
	long	last;
	int		i;

	philos = (t_philo *)arg;
	rules = philos[0].rules;
	(void)rules;
	while (1)
	{
		i = 0;
		while (i < rules->num_philosophers)
		{
			now = get_time_ms();
			
			// Leer last_meal_time protegido
			pthread_mutex_lock(&philos[i].meal_lock);
			last = philos[i].last_meal_time;
			pthread_mutex_unlock(&philos[i].meal_lock);

			if (now - last >= rules->time_to_die)
			{
				// Marcar la muerte ANTES de imprimir
				pthread_mutex_lock(&rules->death_flag_lock);
				rules->someone_died = 1;
				pthread_mutex_unlock(&rules->death_flag_lock);

				// Hacer el print asegurando que nadie más imprime
				pthread_mutex_lock(&rules->print_lock);
				printf(YELLOW "%ld" RESET " %d " RED "died\n" RESET,
					get_time_ms() - rules->simulation_start_time,
					philos[i].id);
				pthread_mutex_unlock(&rules->print_lock);

				return (NULL);
			}
			i++;
		}

		usleep(100); // 0.1 ms
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->id % 2 == 0)
		usleep(1000); // pequeño delay para evitar deadlocks

	while (!p->rules->someone_died)
	{
		take_forks(p);
		if (check_dead(p->rules))
			break ;
		eat(p);
		if (check_dead(p->rules))
			break ;
		if (p->rules->num_times_each_must_eat != -1 &&
			p->times_eaten >= p->rules->num_times_each_must_eat)
			break ;

		go_sleep(p);
		if (check_dead(p->rules))
			break ;
		think(p);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_rules	*rules;

	if (DEBUG)
		printf("\033[0;92mEmpezando el philosophers\033[0m\n");
	rules = malloc(sizeof(t_rules));
	if (!rules)
		return (error_message("Malloc failed\n", 1));
	parse_args_and_start_philos(argc, argv, rules);
	free(rules);
	return (0);
}

//make CFLAGS+=" -DDEBUG=1"

// checando esta solución:  https://github.com/dpetrosy/42-Philosophers