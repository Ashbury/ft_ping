/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_error_message.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justasze <justasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 17:30:48 by justasze          #+#    #+#             */
/*   Updated: 2019/01/07 17:37:04 by justasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void	print_error_message(struct icmphdr *icmp_header,
		struct icmphdr *origin_header)
{
	const char	*unreachable[4] = {NET_UNREACH, HOST_UNREACH, PROT_UNREACH,
			PORT_UNREACH};

	if (icmp_header->type == ICMP_DEST_UNREACH)
	{
		printf(ERROR_UNREACH_FORMAT, g_data.host_addr,
			origin_header->un.echo.sequence);
		if (icmp_header->code < 4)
			printf("%s", unreachable[icmp_header->code]);
		else
			printf(UNREACHABLE_UNKNOWN);
	}
	else if (icmp_header->type == ICMP_TIME_EXCEEDED)
	{
		printf(ERROR_TTL_FORMAT, g_data.reverse_dns,
				g_data.host_addr, origin_header->un.echo.sequence);
		printf(TTL_EXCEEDED);
	}
	else
		printf(UNKNOWN_ERROR_FORMAT, g_data.reverse_dns,
				g_data.host_addr, origin_header->un.echo.sequence);
}

int			check_error_message(struct iphdr *ip_header,
		struct icmphdr *icmp_header)
{
	char			*buffer;
	struct icmphdr	*origin_header;

	buffer = (char*)icmp_header;
	origin_header = (struct icmphdr *)(buffer + sizeof(struct icmphdr)
			+ sizeof(struct iphdr));
	if (origin_header->un.echo.id == g_data.id)
	{
		if ((g_data.flags & QUIET_FLAG) == 0)
		{
			if (g_data.reverse_dns[0] == '\0')
			{
				inet_ntop(AF_INET, &ip_header->saddr, g_data.host_addr,
						INET_ADDRSTRLEN);
				reverse_dns_lookup(ip_header->saddr);
			}
			print_error_message(icmp_header, origin_header);
		}
		g_data.nb_errors += 1;
		return (1);
	}
	return (0);
}
