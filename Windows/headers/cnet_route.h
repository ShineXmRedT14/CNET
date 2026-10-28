#ifndef CNET_ROUTE_H
#define CNET_ROUTE_H

#include <stdint.h>

struct cnet_route
{
	uint8_t smac[6];
	uint8_t dmac[6];
	char *iface;
	char local[16];

	union
	{
		uint8_t u8ip[4];
		uint32_t u32ip;
	} gateway;
};

#endif