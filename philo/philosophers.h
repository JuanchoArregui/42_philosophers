/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 20:11:51 by jarregui          #+#    #+#             */
/*   Updated: 2025/07/11 13:12:25 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSPHERS_H
# define PHILOSPHERS_H


# include <sys/types.h>
# include <unistd.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>

# ifndef DEBUG
#  define DEBUG 0
# endif

# define PHILO_MAX_COUNT 200
# define END_TRANSMISSION '\0'

long	ft_atoi(const char *str);

#endif