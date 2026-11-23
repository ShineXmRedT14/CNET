#ifndef CNET_ETHER_H
#define CNET_ETHER_H

#include <stdint.h>

#define CNET_ETH_IP 0x0800
#define CNET_ETH_ARP 0x0806
#define CNET_ETH_RARP 0x8035
#define CNET_ETH_IPV6 0x86DD
#define CNET_ETH_VLAN 0x8100

struct cnet_ether_bytes
{
	uint8_t bytes[6];
};

struct cnet_ether
{
	uint8_t dst[6];
	uint8_t src[6];
	uint16_t next;
}__attribute__((packed));

#endif