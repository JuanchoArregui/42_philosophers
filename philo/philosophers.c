/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 17:31:44 by jarregui          #+#    #+#             */
/*   Updated: 2025/07/11 13:29:52 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	error_message(char *text, int signal)
{
	if (text)
		printf("%s\n", text);
	exit(signal);
}

void	check_args(int argc, char **argv)
{
	int		i;
	long	num;

	i = 0;
	if (argc < 5 || argc > 6)
	{
		printf("\033[0;91mERROR - You must enter 5 arguments:\n");
		printf("	-number_of_philosophers\n");
		printf("	-time_to_die\n");
		printf("	-time_to_eat\n");
		printf("	-time_to_sleep\n");
		printf("	-number_of_times_each_philosopher_must_eat\033[0m\n");
		exit(1);
	}
	while (++i < argc)
	{
		num = ft_atoi(argv[i]);
		if (i == 1 && (num < 1 || num > PHILO_MAX_COUNT))
			error_message("[Argument ERROR]\n", 1);
		else if (i == 5 && (num < 0 || num > INT_MAX))
			error_message("[Argument ERROR]\n", 1);
		else if (i != 1 && i != 5 && (num < 1 || num > INT_MAX))
			error_message("[Argument ERROR]\n", 1);
	}
}

int	main(int argc, char **argv)
{
	printf("\033[0;92mEmpezando el philosophers\033[0m\n");

	check_args(argc, argv);

	return (0);
}

//make CFLAGS+=" -DDEBUG=1"

// checando esta solución:  https://github.com/dpetrosy/42-Philosophers