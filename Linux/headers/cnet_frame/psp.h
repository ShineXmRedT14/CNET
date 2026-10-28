#ifndef CNET_FRAME_PSP_H
#define CNET_FRAME_PSP_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_psp
{
	struct cnet_frame_type cnet_type;
	uint16_t aid;
	uint8_t bssid[6];
	uint8_t ta[6];
    uint8_t fcs[4];
}__attribute__((packed));

#endif
