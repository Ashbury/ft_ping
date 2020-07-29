/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_dns_lookup.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justasze <justasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/07 17:30:09 by justasze          #+#    #+#             */
/*   Updated: 2019/01/07 17:30:46 by justasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	reverse_dns_lookup(uint32_t ip_addr)
{
	struct sockaddr_in	temp_addr;
	socklen_t			len;

	temp_addr.sin_family = AF_INET;
	temp_addr.sin_addr.s_addr = ip_addr;
	len = sizeof(struct sockaddr_in);
	if (getnameinfo((struct sockaddr *)&temp_addr, len, g_data.reverse_dns,
			NI_MAXHOST, NULL, 0, NI_NOFQDN) != 0)
		ft_strcpy(g_data.reverse_dns, g_data.host_addr);
}
