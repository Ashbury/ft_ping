/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_program.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justasze <justasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 19:27:32 by justasze          #+#    #+#             */
/*   Updated: 2019/01/07 17:13:42 by justasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	end_program(int sig)
{
	struct timeval	exit_time;

	(void)sig;
	gettimeofday(&exit_time, NULL);
	if (g_data.nb_errors == 0)
	{
		printf(STATS_FORMAT, g_data.host_name, g_data.nb_sent_packets,
				g_data.nb_received_packets,
				(size_t)((double)(1.0f - (double)g_data.nb_received_packets
				/ (double)g_data.nb_sent_packets) * 100.f),
				(exit_time.tv_sec - g_data.time_at_launch.tv_sec) * 1000
				+ (exit_time.tv_usec - g_data.time_at_launch.tv_usec) / 1000);
	}
	else
	{
		printf(STATS_ERROR, g_data.host_name, g_data.nb_sent_packets,
				g_data.nb_received_packets, g_data.nb_errors,
				(size_t)((double)(1.0f - (double)g_data.nb_received_packets
				/ (double)g_data.nb_sent_packets) * 100.f),
				(exit_time.tv_sec - g_data.time_at_launch.tv_sec) * 1000
				+ (exit_time.tv_usec - g_data.time_at_launch.tv_usec) / 1000);
	}
	close(g_data.socket);
	freeaddrinfo(g_data.host_infos);
	exit(EXIT_SUCCESS);
}
