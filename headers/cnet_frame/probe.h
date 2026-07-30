#ifndef CNET_FRAME_PROBE_H
#define CNET_FRAME_PROBE_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_probe_req
{
	struct cnet_frame_hdr cnet_hdr;
}__attribute__((packed));

struct cnet_frame_probe_resp
{
	struct cnet_frame_hdr cnet_hdr;
	uint64_t tstamp;
	uint16_t interval;
	uint16_t info
}__attribute__((packed));

#endif