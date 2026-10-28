#ifndef CNET_ETHER_H
#define CNET_ETHER_H

#include <stdint.h>

#define CNET_ETH_IP 0x0800

struct cnet_ether
{
	uint8_t dst[6];
	uint8_t src[6];
	uint16_t next;
}__attribute__((packed));

#endif