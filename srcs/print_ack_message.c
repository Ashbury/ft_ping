/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ack_message.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justasze <justasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 17:25:18 by justasze          #+#    #+#             */
/*   Updated: 2019/01/07 17:30:43 by justasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void		print_ack_message(ssize_t ret, struct iphdr *ip_header,
		struct icmphdr *icmp_header, struct timeval *timeval_msg)
{
	double			elapsed_time;
	struct timeval	timeval_now;

	gettimeofday(&timeval_now, NULL);
	if (g_data.reverse_dns[0] == '\0')
	{
		inet_ntop(AF_INET, &ip_header->saddr, g_data.host_addr,
				INET_ADDRSTRLEN);
		reverse_dns_lookup(ip_header->saddr);
	}
	elapsed_time = (timeval_now.tv_usec - timeval_msg->tv_usec)
			/ 1000.0f;
	if (g_data.flags & AUDIBLE_FLAG)
		printf("\a");
	printf(RESPONSE_FORMAT, ret - sizeof(struct iphdr),
			g_data.reverse_dns, g_data.host_addr,
			icmp_header->un.echo.sequence, ip_header->ttl);
	if (elapsed_time > 100.f)
		printf(TIME_FORMAT1, (ssize_t)elapsed_time);
	else
		printf(TIME_FORMAT2, elapsed_time);
}
