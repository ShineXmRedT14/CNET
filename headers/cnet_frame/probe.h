#ifndef CNET_FRAME_PROBE_H
#define CNET_FRAME_PROBE_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

#define CNET_FRAME_PROBE_BODY_EID 0x00
#define CNET_FRAME_PROBE_BODY_LEN 0x00
#define CNET_FRAME_PROBE_RATES_EID 0x01
#define CNET_FRAME_PROBE_RATES_LEN 0x01
#define CNET_FRAME_PROBE_RATES_MB {0x82, 0x84, 0x8B, 0x96, 0x06, 0x09, 0x0C, 0x12}

struct cnet_frame_probe_body
{
    uint8_t eid; // CNET_FRAME_PROBE_BODY_EID
    uint8_t len; // CNET_FRAME_PROBE_BODY_LEN
}__attribute__((packed));

struct cnet_frame_probe_rates
{
    uint8_t eid; // CNET_FRAME_PROBE_RATES_EID
    uint8_t len; // CNET_FRAME_PROBE_RATES_LEN
    uint8_t rates[8]; // CNET_FRAME_PROBE_RATES_MB
}__attribute__((packed));

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
