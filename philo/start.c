/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 10:08:11 by jarregui          #+#    #+#             */
/*   Updated: 2025/12/02 17:20:05 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	check_args(int argc, char **argv, int i)
{
	long	num;

	if (argc < 5 || argc > 6)
		return (print_err_params());
	while (++i < argc)
	{
		if (!is_number(argv[i]))
			return (err_msg("ERROR: NOT VALID PARAMS.\n", 1));
		num = ft_atoi(argv[i]);
		if (num < 0)
			return (err_msg("ERROR: NEGATIVE OR NOT VALID PARAM.\n", 1));
		else if (i == 1 && (num < 1 || num > PHILO_MAX_COUNT))
			return (err_msg("[Argument ERROR PHILO_MAX_COUNT]\n", 1));
		else if (i == 5 && (num < 0 || num > INT_MAX))
			return (err_msg("[Argument ERROR INT_MAX]\n", 1));
		else if (i != 1 && i != 5 && (num < 1 || num > INT_MAX))
			return (err_msg("[Argument ERROR]\n", 1));
	}
	return (0);
}

static int	init_rules(int argc, char **argv, t_rules *rules)
{
	int	i;

	i = 0;
	rules->num_philosophers = ft_atoi(argv[1]);
	rules->time_to_die = ft_atoi(argv[2]);
	rules->time_to_eat = ft_atoi(argv[3]);
	rules->time_to_sleep = ft_atoi(argv[4]);
	rules->num_times_each_must_eat = -1;
	if (argc == 6)
		rules->num_times_each_must_eat = ft_atoi(argv[5]);
	rules->someone_died = 0;
	rules->simulation_start_time = 0;
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->num_philosophers);
	if (!rules->forks)
		return (err_msg("Malloc failed\n", 1));
	while (i < rules->num_philosophers)
	{
		pthread_mutex_init(&rules->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&rules->print_lock, NULL);
	pthread_mutex_init(&rules->death_flag_lock, NULL);
	return (0);
}

static void	init_philos(t_rules *rules, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < rules->num_philosophers)
	{
		philos[i].id = i + 1;
		philos[i].times_eaten = 0;
		philos[i].rules = rules;
		philos[i].left_fork_index = i;
		philos[i].right_fork_index = (i + 1) % rules->num_philosophers;
		philos[i].last_meal_time = 0;
		pthread_mutex_init(&philos[i].meal_lock, NULL);
		i++;
	}
}

static int	start_threads(t_rules *rules, t_philo *philos)
{
	int			i;
	pthread_t	monitor;

	rules->simulation_start_time = get_time_ms();
	i = 0;
	while (i < rules->num_philosophers)
	{
		philos[i].last_meal_time = rules->simulation_start_time;
		if (pthread_create(&philos[i].thread, NULL,
				philo_routine, &philos[i]) != 0)
			return (err_msg("Thread creation failed", 1));
		i++;
	}
	pthread_create(&monitor, NULL, monitor_routine, philos);
	pthread_join(monitor, NULL);
	i = 0;
	while (i < rules->num_philosophers)
		pthread_join(philos[i++].thread, NULL);
	return (0);
}

int	parse_args_and_start_philos(int argc, char **argv, t_rules *rules)
{
	t_philo	*philos;
	int		i;

	if (check_args(argc, argv, 1) != 0)
		return (1);
	if (init_rules(argc, argv, rules) != 0)
		return (1);
	philos = malloc(sizeof(t_philo) * rules->num_philosophers);
	if (!philos)
		return (err_msg("Malloc failed", 1));
	init_philos(rules, philos);
	if (start_threads(rules, philos) != 0)
		return (1);
	i = 0;
	while (i < rules->num_philosophers)
		pthread_mutex_destroy(&rules->forks[i++]);
	pthread_mutex_destroy(&rules->print_lock);
	pthread_mutex_destroy(&rules->death_flag_lock);
	i = 0;
	while (i < rules->num_philosophers)
		pthread_mutex_destroy(&philos[i++].meal_lock);
	free(philos);
	free(rules->forks);
	return (0);
}
