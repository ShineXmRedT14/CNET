#ifndef CNET_IGMP_H
#define CNET_IGMP_H

#include <stdint.h>

struct cnet_igmp
{
	uint8_t type;
	uint8_t code;
	uint16_t sum;
	uint32_t group;
}__attribute__((packed));

#endif