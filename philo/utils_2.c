/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 10:48:08 by jarregui          #+#    #+#             */
/*   Updated: 2025/12/02 17:18:20 by jarregui         ###   ########.fr       */
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

int	is_number(char *s)
{
	int	i;

	i = 0;
	if (!s || !s[0])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	print_left(t_philo *p)
{
	if (DEBUG)
		print_status(p, "has taken LEFT fork 🍴", -1);
	else
		print_status(p, "has taken a fork", -1);
}

void	print_right(t_philo *p)
{
	if (DEBUG)
		print_status(p, "has taken RIGHT fork 🍴", -1);
	else
		print_status(p, "has taken a fork", -1);
}

int	print_err_params(void)
{
	printf(RED "ERROR - You must enter 4 arguments:\n");
	printf("	-number_of_philosophers\n	-time_to_die\n");
	printf("	-time_to_eat\n	-time_to_sleep\n");
	printf("And an optional 5th argument:\n");
	printf("	-number_of_times_each_philosopher_must_eat\n" RESET);
	return (1);
}
