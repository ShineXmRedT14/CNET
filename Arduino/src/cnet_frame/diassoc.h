#ifndef CNET_FRAME_DIASSOC_H
#define CNET_FRAME_DIASSOC_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_diassoc
{
	struct cnet_frame_hdr cnet_hdr;
	uint16_t code;
}__attribute__((packed));

#endif