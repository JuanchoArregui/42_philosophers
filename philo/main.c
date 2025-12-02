/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 18:04:06 by jarregui          #+#    #+#             */
/*   Updated: 2025/12/02 22:01:22 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	all_philos_ate_enough(t_rules *rules, t_philo *philos)
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
		if (philos[i].t_eaten < rules->n_must_eat)
			all_done = 0;
		pthread_mutex_unlock(&philos[i].meal_lock);
		i++;
	}
	if (DEBUG && all_done)
		printf(GREEN
			"Todos los %d filósofos han comido lo suficiente: %d veces.\n"
			RESET, rules->num_philosophers, rules->n_must_eat);
	return (all_done);
}

static int	monitor_check_dead(t_rules *rules, t_philo *philos, int i)
{
	long	now;
	long	last;

	while (i < rules->num_philosophers)
	{
		now = get_time_ms();
		pthread_mutex_lock(&philos[i].meal_lock);
		last = philos[i].last_meal_time;
		pthread_mutex_unlock(&philos[i].meal_lock);
		if (now - last > rules->time_to_die)
		{
			pthread_mutex_lock(&rules->death_flag_lock);
			rules->someone_died = 1;
			pthread_mutex_unlock(&rules->death_flag_lock);
			pthread_mutex_lock(&rules->print_lock);
			printf(YELLOW "%ld" RESET " %d " RED "died\n" RESET,
				get_time_ms() - rules->simulation_start_time,
				philos[i].id);
			pthread_mutex_unlock(&rules->print_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philos;
	t_rules	*rules;

	philos = (t_philo *)arg;
	rules = philos[0].rules;
	(void)rules;
	while (1)
	{
		if (rules->n_must_eat != -1
			&& all_philos_ate_enough(rules, philos))
			return (NULL);
		if (monitor_check_dead(rules, philos, 0))
			return (NULL);
		usleep(10);
	}
}

int	main(int argc, char **argv)
{
	t_rules	*rules;
	int		ret;

	if (DEBUG)
		printf(GREEN "Empezando el philosophers\n" RESET);
	rules = malloc(sizeof(t_rules));
	if (!rules)
		return (err_msg("Malloc failed\n", 1));
	ret = parse_args_and_start_philos(argc, argv, rules);
	if (DEBUG && ret == 0)
	{
		printf(CYAN "Fin simulación con estos parámetros:\n");
		printf(" -Número de filósofos: %d\n", rules->num_philosophers);
		printf(" -Tiempo para morir: %ld ms\n", rules->time_to_die);
		printf(" -Tiempo para comer: %ld ms\n", rules->time_to_eat);
		printf(" -Tiempo para dormir: %ld ms\n" RESET, rules->time_to_sleep);
		if (rules->n_must_eat != -1)
			printf(CYAN " -Núm veces cada filósofo debe comer: %d\n" RESET,
				rules->n_must_eat);
	}
	free(rules);
	return (ret);
}
