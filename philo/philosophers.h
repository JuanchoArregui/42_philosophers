/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 20:11:51 by jarregui          #+#    #+#             */
/*   Updated: 2025/12/02 10:55:38 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <sys/types.h>
# include <sys/time.h>
# include <unistd.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <pthread.h>

# ifndef DEBUG
#  define DEBUG 0
# endif

# if DEBUG == 1
#  define RED "\033[0;91m"
#  define YELLOW "\033[0;93m"
#  define RESET "\033[0m"
#  define GREEN "\033[0;92m"
#  define CYAN "\033[0;96m"
# else
#  define RED ""
#  define YELLOW  ""
#  define RESET ""
#  define GREEN ""
#  define CYAN ""
# endif

# define PHILO_MAX_COUNT 200
# define END_TRANSMISSION '\0'

typedef struct s_rules
{
	int				num_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				num_times_each_must_eat;
	int				someone_died;
	long			simulation_start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	death_flag_lock;
}	t_rules;

typedef struct s_philo
{
	int				id;
	int				times_eaten;
	t_rules			*rules;
	int				left_fork_index;
	int				right_fork_index;
	long			last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	meal_lock;
}	t_philo;

//philo_utils.c
void	take_forks(t_philo *p);
void	eat(t_philo *p);
void	go_sleep(t_philo *p);
void	think(t_philo *p);
void	smart_sleep(long duration, t_rules *rules);

//philosophers.c
void	*philo_routine(void *arg);
void	*monitor_routine(void *arg);

//start.c
int		parse_args_and_start_philos(int argc, char **argv, t_rules *rules);

//utils_1.c
int		error_message(char *text, int signal);
long	get_time_ms(void);
void	print_status(t_philo *p, char *msg, int num_meals);
int		check_dead(t_rules *rules);

//utils_2.c
long	ft_atoi(const char *str);
void	print_left(t_philo *p);
void	print_right(t_philo *p);

#endif