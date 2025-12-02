/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 13:07:09 by jarregui          #+#    #+#             */
/*   Updated: 2025/12/02 17:04:34 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	err_msg(char *text, int signal)
{
	if (text)
		printf("%s\n", text);
	return (signal);
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void	print_status(t_philo *p, char *msg, int num_meals)
{
	int	died;

	pthread_mutex_lock(&p->rules->print_lock);
	pthread_mutex_lock(&p->rules->death_flag_lock);
	died = p->rules->someone_died;
	pthread_mutex_unlock(&p->rules->death_flag_lock);
	if (!died)
	{
		printf(YELLOW "%ld" RESET " %d %s",
			get_time_ms() - p->rules->simulation_start_time,
			p->id, msg);
		if (DEBUG && num_meals >= 0)
			printf(CYAN " (total meals: %d/%d)" RESET, num_meals,
				p->rules->num_times_each_must_eat);
		printf("\n");
	}
	pthread_mutex_unlock(&p->rules->print_lock);
}

int	check_dead(t_rules *rules)
{
	int	died;

	pthread_mutex_lock(&rules->death_flag_lock);
	died = rules->someone_died;
	pthread_mutex_unlock(&rules->death_flag_lock);
	return (died);
}
