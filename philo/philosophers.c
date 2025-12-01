/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:31:44 by jarregui          #+#    #+#             */
/*   Updated: 2025/12/02 00:07:06 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	all_philos_ate_enough(t_rules *rules, t_philo *philos)
{
	int	i;
	int	all_done;

	i = 0;
	all_done = 1;
	pthread_mutex_lock(&rules->death_flag_lock);
	if (rules->someone_died)
	{
		pthread_mutex_unlock(&rules->death_flag_lock);
		return (0);
	}
	pthread_mutex_unlock(&rules->death_flag_lock);
	while (i < rules->num_philosophers)
	{
		pthread_mutex_lock(&philos[i].meal_lock);
		if (philos[i].times_eaten < rules->num_times_each_must_eat)
			all_done = 0;
		pthread_mutex_unlock(&philos[i].meal_lock);
		i++;
	}
	if (DEBUG && all_done)
		printf(GREEN
			"Todos los %d filósofos han comido lo suficiente: %d veces.\n"
			RESET, rules->num_philosophers, rules->num_times_each_must_eat);
	return (all_done);
}


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
		// 1. Chequea si TODOS comieron lo suficiente
		if (rules->num_times_each_must_eat != -1 && 
			all_philos_ate_enough(rules, philos))
			return (NULL);

        // 2. Chequea muertes
		i = 0;
		while (i < rules->num_philosophers)
		{
			now = get_time_ms();
			
			// Leer last_meal_time protegido
			pthread_mutex_lock(&philos[i].meal_lock);
			last = philos[i].last_meal_time;
			pthread_mutex_unlock(&philos[i].meal_lock);

			if (now - last > rules->time_to_die) //quitamos el = para dar ese pelin más de margen en caso de ser exactamente iguales
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

		
	pthread_mutex_lock(&p->meal_lock);
    p->last_meal_time = get_time_ms();  // ← Cada filósofo inicializa SU propio tiempo
    pthread_mutex_unlock(&p->meal_lock);

	while (!check_dead(p->rules))
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