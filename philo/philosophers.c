/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:31:44 by jarregui          #+#    #+#             */
/*   Updated: 2025/12/02 18:09:49 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	set_last_meal_time(t_philo *p)
{
	pthread_mutex_lock(&p->meal_lock);
	p->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&p->meal_lock);
}

static void	*philo_alone(t_philo *p)
{
	pthread_mutex_lock(&p->rules->forks[p->left_fork_index]);
	print_left(p);
	while (!check_dead(p->rules))
		usleep(100);
	pthread_mutex_unlock(&p->rules->forks[p->left_fork_index]);
	return (NULL);
}

void	*philo_routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->rules->num_philosophers == 1)
		return (philo_alone(p));
	if (p->id % 2 == 0)
		usleep(p->id * 200);
	set_last_meal_time(p);
	while (!check_dead(p->rules))
	{
		take_forks(p);
		if (check_dead(p->rules))
			break ;
		eat(p);
		if (check_dead(p->rules))
			break ;
		if (p->rules->n_must_eat != -1 && p->t_eaten >= p->rules->n_must_eat)
			break ;
		go_sleep(p);
		if (check_dead(p->rules))
			break ;
		think(p);
	}
	return (NULL);
}
