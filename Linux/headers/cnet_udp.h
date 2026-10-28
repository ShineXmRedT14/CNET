#ifndef CNET_UDP_H
#define CNET_UDP_H

#include <stdint.h>

struct cnet_udp
{
	uint16_t src;
	uint16_t dst;
    uint16_t len;
    uint16_t sum;
}__attribute__((packed));

#endif