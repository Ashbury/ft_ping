/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justasze <justasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/30 11:21:39 by justasze          #+#    #+#             */
/*   Updated: 2019/01/09 17:39:10 by justasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static uint16_t	checksum(uint16_t *buffer, size_t len)
{
	uint32_t	sum;

	sum = 0;
	while (len > 1)
	{
		sum += *buffer;
		buffer++;
		len -= 2;
	}
	if (len == 1)
		sum += *(unsigned char*)buffer;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	return ((uint16_t)~sum);
}

static int		check_response(char *buffer, ssize_t ret)
{
	struct iphdr	*ip_header;
	struct icmphdr	*icmp_header;
	struct timeval	*timeval_msg;

	ip_header = (struct iphdr *)buffer;
	icmp_header = (struct icmphdr *)(buffer + sizeof(struct iphdr));
	timeval_msg = (struct timeval *)(buffer + sizeof(struct iphdr)
			+ sizeof(struct icmphdr));
	if (icmp_header->un.echo.id == g_data.id
			&& icmp_header->un.echo.sequence == g_data.sequence)
	{
		if (g_data.flags & FLOOD_FLAG)
			write(1, "\x08", 1);
		else if ((g_data.flags & QUIET_FLAG) == 0)
			print_ack_message(ret, ip_header, icmp_header, timeval_msg);
		g_data.nb_received_packets += 1;
		return (1);
	}
	else
		return (check_error_message(ip_header, icmp_header));
}

static void		get_response(void)
{
	char					buffer[548];
	struct sockaddr			copy;
	struct iovec			iov;
	struct msghdr			message;
	ssize_t					ret;

	iov.iov_base = buffer;
	iov.iov_len = sizeof(buffer);
	message.msg_name = &copy;
	message.msg_namelen = sizeof(struct sockaddr);
	message.msg_iov = &iov;
	message.msg_iovlen = 1;
	message.msg_control = 0;
	message.msg_controllen = 0;
	ret = recvmsg(g_data.socket, &message, 0);
	if (ret > 0 && check_response(buffer, ret) == 0
			&& (g_data.flags & FLOOD_FLAG) == 0)
		get_response();
	if (g_data.nb_sent_packets == g_data.count_to_send)
		end_program(1);
}

static int		sending_error(void)
{
	dprintf(2, "Error while sending packet\n");
	close(g_data.socket);
	freeaddrinfo(g_data.host_infos);
	exit(EXIT_ERROR);
}

void			ping(int sig)
{
	t_packet		packet;
	ssize_t			ret;

	(void)sig;
	ft_bzero(&packet, sizeof(packet));
	packet.icmp_header.type = ICMP_ECHO;
	packet.icmp_header.un.echo.id = g_data.id;
	packet.icmp_header.un.echo.sequence = ++g_data.sequence;
	gettimeofday((struct timeval*)(packet.msg), NULL);
	packet.icmp_header.checksum = checksum((uint16_t*)&packet, sizeof(packet));
	ret = sendto(g_data.socket, &packet, sizeof(packet), 0,
			g_data.host_infos->ai_addr, sizeof(struct sockaddr_in));
	if ((g_data.flags & FLOOD_FLAG) == 0)
		alarm(1);
	if (ret == -1)
		sending_error();
	g_data.nb_sent_packets += 1;
	if ((g_data.flags & FLOOD_FLAG) && (g_data.flags & AUDIBLE_FLAG))
		write(1, "\a.", 2);
	else if (g_data.flags & FLOOD_FLAG)
		write(1, ".", 1);
	get_response();
}
