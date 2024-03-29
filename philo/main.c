/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elrichar <elrichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 11:45:30 by elrichar          #+#    #+#             */
/*   Updated: 2023/10/05 20:32:53 by elrichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	join_threads(t_philo *philos, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		if (pthread_join((philos[i]).thread_id, NULL) != 0)
			write(2, "Error : pthread_join issue.\n", 28);
		i++;
	}
}

int	init_threads(t_philo *philos)
{
	int			nb;
	int			i;

	nb = philos->nb_philo;
	i = 0;
	while (i < nb)
	{
		if (pthread_create(&(philos[i].thread_id), 0, \
		routine, (void *)(&philos[i])))
		{
			*(philos->init_check) = 1;
			write(2, "Error : pthread_create failed\n", 30);
			while (--i >= 0)
			{
				if (pthread_join(philos[i].thread_id, NULL) != 0)
					write(2, "Error : pthread_join issue.\n", 28);
			}
			return (0);
		}
		i++;
	}
	join_threads(philos, nb);
	return (1);
}

int	main(int ac, char **av)
{
	pthread_mutex_t	*forks;
	t_philo			*philos;

	forks = NULL;
	philos = NULL;
	if (!check_args(ac, av))
		return (1);
	if (!init_variables(av, ac, &forks, &philos))
	{
		free(philos);
		free(forks);
		return (1);
	}
	if (!init_threads(philos))
	{
		free_and_destroy(av, philos, forks);
		return (1);
	}
	free_and_destroy(av, philos, forks);
	return (0);
}
