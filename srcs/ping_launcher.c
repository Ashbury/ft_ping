/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping_launcher.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justasze <justasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/30 11:19:59 by justasze          #+#    #+#             */
/*   Updated: 2019/01/09 15:58:07 by justasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static const char	*get_host_ip_str(char *buffer)
{
	void				*addr;
	struct sockaddr_in	*ipv4;
	struct sockaddr_in6	*ipv6;

	if (g_data.host_infos->ai_family == AF_INET)
	{
		addr = g_data.host_infos->ai_addr;
		ipv4 = (struct sockaddr_in *)addr;
		addr = &(ipv4->sin_addr);
	}
	else
	{
		addr = g_data.host_infos->ai_addr;
		ipv6 = (struct sockaddr_in6 *)addr;
		addr = &(ipv6->sin6_addr);
	}
	return (inet_ntop(g_data.host_infos->ai_family, addr, buffer,
			g_data.host_infos->ai_addrlen));
}

static int			set_socket_options(void)
{
	struct timeval	timeout;

	if (g_data.time_to_live == 0)
		g_data.time_to_live = 64;
	if (setsockopt(g_data.socket, SOL_IP, IP_TTL, &g_data.time_to_live,
			sizeof(g_data.time_to_live)) != 0)
	{
		dprintf(2, "Error: couldn't set time to live.\n");
		close(g_data.socket);
		return (0);
	}
	timeout.tv_sec = g_data.flags & FLOOD_FLAG ? 0 : 1;
	timeout.tv_usec = g_data.flags & FLOOD_FLAG ? 10000 : 0;
	if (setsockopt(g_data.socket, SOL_SOCKET, SO_RCVTIMEO, &timeout,
			sizeof(timeout)) != 0)
	{
		dprintf(2, "Error: couldn't set receive timeout.\n");
		close(g_data.socket);
		return (0);
	}
	return (1);
}

int					ping_launcher(char *host_name)
{
	g_data.socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g_data.socket == -1)
	{
		dprintf(2, "Error: couldn't create socket.\n");
		return (1);
	}
	if (set_socket_options() == 0)
		return (1);
	gettimeofday(&g_data.time_at_launch, NULL);
	g_data.host_name = host_name;
	g_data.id = (uint16_t)getpid();
	signal(SIGINT, end_program);
	signal(SIGALRM, ping);
	get_host_ip_str(g_data.host_addr);
	printf("PING %s (%s) %zu(%zu) bytes of data.\n", g_data.host_name,
			g_data.host_addr, MSG_SIZE, PACKET_SIZE);
	ping(0);
	while (1)
	{
		if (g_data.flags & FLOOD_FLAG)
			ping(0);
	}
}
