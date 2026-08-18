#ifndef CNET_FRAME_ADV_H
#define CNET_FRAME_ADV_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_adv
{
	struct cnet_frame_hdr cnet_hdr;
	uint64_t tstamp;
	uint16_t info;
}__attribute__((packed));

#endif