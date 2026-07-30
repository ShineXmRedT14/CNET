#ifndef CNET_ROUTE_H
#define CNET_ROUTE_H

#include <stdint.h>

struct cnet_route
{
	char iface[16];
	uint8_t mac[6];
	
	union
	{
		uint8_t i8p[4];
		uint32_t i32p;
	} ip;
};

#endif