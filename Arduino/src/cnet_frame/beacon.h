#ifndef CNET_FRAME_BEACON_H
#define CNET_FRAME_BEACON_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_beacon
{
	struct cnet_frame_hdr cnet_hdr;
	uint64_t tstamp;
	uint16_t bint;
	uint16_t cinfo;
}__attribute__((packed));

#endif