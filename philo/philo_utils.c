/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 21:54:51 by jarregui          #+#    #+#             */
/*   Updated: 2025/12/02 18:11:41 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_forks(t_philo *p)
{
	if (check_dead(p->rules))
		return ;
	if (p->id % 2 == 0)
	{
		pthread_mutex_lock(&p->rules->forks[p->left_fork_index]);
		print_left(p);
		pthread_mutex_lock(&p->rules->forks[p->right_fork_index]);
		print_right(p);
	}
	else
	{
		pthread_mutex_lock(&p->rules->forks[p->right_fork_index]);
		print_right(p);
		pthread_mutex_lock(&p->rules->forks[p->left_fork_index]);
		print_left(p);
	}
}

void	eat(t_philo *p)
{
	if (DEBUG)
		print_status(p, "is eating 🍝", p->t_eaten + 1);
	else
		print_status(p, "is eating", -1);
	smart_sleep(p->rules->time_to_eat, p->rules);
	set_last_meal_time(p);
	p->t_eaten++;
	pthread_mutex_unlock(&p->rules->forks[p->left_fork_index]);
	pthread_mutex_unlock(&p->rules->forks[p->right_fork_index]);
}

void	go_sleep(t_philo *p)
{
	if (check_dead(p->rules))
		return ;
	if (DEBUG)
		print_status(p, "is sleeping 🛌", -1);
	else
		print_status(p, "is sleeping", -1);
	smart_sleep(p->rules->time_to_sleep, p->rules);
}

void	think(t_philo *p)
{
	if (check_dead(p->rules))
		return ;
	if (DEBUG)
		print_status(p, "is thinking 🧠", -1);
	else
		print_status(p, "is thinking", -1);
	if (p->rules->num_philosophers % 2 != 0)
		usleep(500);
}

void	smart_sleep(long duration, t_rules *rules)
{
	long	start;
	int		died;

	start = get_time_ms();
	while ((get_time_ms() - start) < duration)
	{
		pthread_mutex_lock(&rules->death_flag_lock);
		died = rules->someone_died;
		pthread_mutex_unlock(&rules->death_flag_lock);
		if (died)
			break ;
		usleep(100);
	}
}
