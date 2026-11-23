#ifndef CNET_ROUTE_H
#define CNET_ROUTE_H

#include <stdint.h>

#define CNET_IFMAX 32
#define CNET_LOCMAX 16

#include <cnet_platform.h>

#if defined(CNET_PLATFORM_LINUX)
    struct cnet_route
    {
	    char iface[CNET_IFMAX][IFNAMSIZ];
	    char active[IFNAMSIZ];
	    int count;

	    uint8_t mac[6];

	    union
	    {
		    uint8_t i8p[4];
		    uint32_t i32p;
	    } ip;
    };

#elif defined(CNET_PLATFORM_WINDOWS)
    struct cnet_route
    {
	    char iface[CNET_IFMAX][IFNAMSIZ];
	    char active[IFNAMSIZ];
	    int count;

	    char local[CNET_LOCMAX][CNET_LOCMAX];
	    int local_count;

	    uint8_t smac[6];
	    uint8_t dmac[6];

	    union
	    {
		    uint8_t i8p[4];
		    uint32_t i32p;
	    } ip;
    };

#elif !defined(CNET_PLATFORM_ESP32)
    #error "cnet_route: platform error."
#endif
#endif