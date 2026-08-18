#ifndef CNET_FRAME_CF_H
#define CNET_FRAME_CF_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_cf
{
	struct cnet_frame_type cnet_type;
	uint16_t id;
	uint8_t ra[6];
	uint8_t bssid[6];
}__attribute__((packed));

struct cnet_frame_cf_ack
{
	struct cnet_frame_type cnet_type;
	uint16_t id;
	uint8_t ra[6];
	uint8_t bssid[6];
}__attribute__((packed));

#endif