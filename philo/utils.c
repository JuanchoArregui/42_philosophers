/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 13:07:09 by jarregui          #+#    #+#             */
/*   Updated: 2025/12/01 19:15:47 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	ft_atoi(const char *str)
{
	long	num;
	int		sign;
	int		i;

	i = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		++i;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	num = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		num *= 10;
		num = num + (str[i] - '0');
		++i;
		if (num > INT_MAX)
			break ;
	}
	return (num * sign);
}

int	error_message(char *text, int signal)
{
	if (text)
		printf("%s\n", text); //checar salida
	exit(signal); //Checar exit no autorizado
	return (signal);
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void	print_status(t_philo *p, char *msg)
{
	int	died;

	pthread_mutex_lock(&p->rules->print_lock);
	pthread_mutex_lock(&p->rules->death_flag_lock);
	died = p->rules->someone_died;
	pthread_mutex_unlock(&p->rules->death_flag_lock);
	if (!died)
	{
		printf(YELLOW "%ld" RESET " %d %s\n",
			get_time_ms() - p->rules->simulation_start_time,
			p->id,
			msg);
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
