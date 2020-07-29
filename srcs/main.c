/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justasze <justasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/10 11:14:53 by justasze          #+#    #+#             */
/*   Updated: 2019/01/09 12:00:30 by justasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

t_ping_data	g_data;

static void		parse_option_bis(char **av, size_t *arg, size_t i)
{
	if (av[*arg][i] == 't')
	{
		*arg += 1;
		g_data.time_to_live = (uint32_t)ft_atoi(av[*arg]);
		if (g_data.time_to_live == 0)
		{
			dprintf(2, BAD_TTL, av[0]);
			exit(EXIT_ERROR);
		}
	}
	else if (av[*arg][i] == 'c')
	{
		*arg += 1;
		g_data.count_to_send = ft_atoi(av[*arg]);
		if (g_data.count_to_send == 0)
		{
			dprintf(2, BAD_COUNT_TO_TRANSMIT, av[0]);
			exit(EXIT_ERROR);
		}
	}
	else
	{
		dprintf(2, INVALID_OPTION, av[0], av[*arg][i], av[0]);
		exit(EXIT_ERROR);
	}
}

static void		parse_option(char **av, size_t *arg)
{
	size_t	i;

	i = 1;
	while (av[*arg][i])
	{
		if (av[*arg][i] == 'h')
		{
			dprintf(2, USAGE, av[0]);
			exit(EXIT_ERROR);
		}
		else if (av[*arg][i] == 'v')
			g_data.flags |= VERBOSE_FLAG;
		else if (av[*arg][i] == 'a')
			g_data.flags |= AUDIBLE_FLAG;
		else if (av[*arg][i] == 'q')
			g_data.flags |= QUIET_FLAG;
		else if (av[*arg][i] == 'f')
			g_data.flags |= FLOOD_FLAG;
		else
		{
			parse_option_bis(av, arg, i);
			return ;
		}
		i++;
	}
}

static size_t	get_arguments(int ac, char **av)
{
	size_t	i;

	i = 1;
	while (i < (size_t)ac)
	{
		if (av[i][0] != '-')
			return (i);
		parse_option(av, &i);
		i++;
	}
	return (i);
}

int				main(int ac, char **av)
{
	struct addrinfo	hints;
	size_t			host_arg;

	if (ac < 2)
	{
		dprintf(2, USAGE, av[0]);
		return (2);
	}
	else if (getuid() != 0)
	{
		dprintf(2, "You need to be root to launch this program\n");
		return (1);
	}
	ft_bzero(&hints, sizeof(struct addrinfo));
	host_arg = get_arguments(ac, av);
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_INET;
	if (getaddrinfo(av[host_arg], NULL, &hints, &g_data.host_infos) == -1)
		dprintf(2, "Error with getaddrinfo\n");
	else if (g_data.host_infos != NULL)
		return (ping_launcher(av[host_arg]));
	else
		dprintf(2, "%s: %s: Name or service not known.\n", av[0], av[host_arg]);
	return (EXIT_ERROR);
}
