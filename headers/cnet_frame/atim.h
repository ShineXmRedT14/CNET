#ifndef CNET_FRAME_ATIM_H
#define CNET_FRAME_ATIM_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_atim
{
	struct cnet_frame_hdr cnet_hdr;
    uint8_t fcs[4];
}__attribute__((packed));

#endif
