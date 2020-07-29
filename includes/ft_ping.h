/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: justasze <justasze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/10 11:13:22 by justasze          #+#    #+#             */
/*   Updated: 2019/01/10 13:37:51 by justasze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H

# define FT_PING_H

# include "libft.h"
# include <arpa/inet.h>
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/ip_icmp.h>
# include <sys/param.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>

# define USAGE "Usage: %s [-hvaqf] [-c count] [-t ttl] destination\n"
# define INVALID_OPTION "%s: invalid option -- '%c'\n" USAGE
# define BAD_TTL "%s: can't set unicast time-to-live: Invalid argument\n"
# define BAD_COUNT_TO_TRANSMIT "%s: bad number of packets to transmit.\n"

# define STATS_FORMAT1 "\n--- %s ping statistics ---\n%zu packets transmited, "
# define STATS_FORMAT2 "%zu received, %zu%% packet loss, time %zums\n"
# define STATS_FORMAT STATS_FORMAT1 STATS_FORMAT2

# define STATS_ERROR1 "%zu received, +%zu errors, %zu%% packet loss, "
# define STATS_ERROR2 "time %zums\n\n"
# define STATS_ERROR STATS_FORMAT1 STATS_ERROR1 STATS_ERROR2

# define RESPONSE_FORMAT "%zd bytes from %s (%s): icmp_seq=%hu ttl=%u "
# define TIME_FORMAT1 "time=%zd ms\n"
# define TIME_FORMAT2 "time=%#.3g ms\n"

# define ERROR_UNREACH_FORMAT "From %s icmp_seq=%hu "
# define NET_UNREACH "Destination Net Unreachable\n"
# define HOST_UNREACH "Destination Host Unreachable\n"
# define PROT_UNREACH "Destination Protocol Unreachable\n"
# define PORT_UNREACH "Destination Port Unreachable\n"
# define UNREACHABLE_UNKNOWN "Destination unreachable for unknown reasons\n"
# define ERROR_TTL_FORMAT "From %s (%s) icmp_seq=%hu "
# define TTL_EXCEEDED "Time to live exceeded\n"
# define UNKNOWN_ERROR_FORMAT "From %s (%s) icmp_seq=%hu Unknown error\n"

# define MSG_SIZE 56L
# define PACKET_SIZE MSG_SIZE + sizeof(struct icmphdr) + sizeof(struct iphdr)

# define VERBOSE_FLAG 0x1
# define AUDIBLE_FLAG 0x2
# define QUIET_FLAG 0x4
# define FLOOD_FLAG 0x8

# define EXIT_SUCCESS 0
# define EXIT_NO_RESPONSE 1
# define EXIT_ERROR 2

typedef struct		s_packet
{
	struct icmphdr	icmp_header;
	char			msg[MSG_SIZE];
}					t_packet;

typedef struct		s_ping_data
{
	uint16_t		id;
	uint16_t		sequence;
	uint32_t		time_to_live;
	int				flags;
	int				socket;
	size_t			nb_sent_packets;
	size_t			nb_received_packets;
	size_t			nb_errors;
	size_t			count_to_send;
	struct addrinfo	*host_infos;
	char			*host_name;
	char			host_addr[INET_ADDRSTRLEN];
	char			reverse_dns[NI_MAXHOST];
	struct timeval	time_at_launch;
}					t_ping_data;

extern t_ping_data	g_data;

int					check_error_message(struct iphdr *ip_header,
							struct icmphdr *icmp_header);
void				end_program(int sig);
int					ping_launcher(char *host_name);
void				ping(int sig);
void				print_ack_message(ssize_t ret, struct iphdr *ip_header,
							struct icmphdr *icmp_header,
							struct timeval *timeval_msg);
void				reverse_dns_lookup(uint32_t ip_addr);

#endif
