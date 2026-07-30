#ifndef CNET_FRAME_DATA_H
#define CNET_FRAME_DATA_H

#include <stdint.h>
#include <cnet_frame/hdr.h>
#include <cnet_frame/definition.h>

struct cnet_frame_data
{
	struct cnet_frame_hdr cnet_hdr;
}__attribute__((packed));

struct cnet_frame_null
{
	struct cnet_frame_hdr cnet_hdr;
}__attribute__((packed));

struct cnet_frame_data_wds
{
	struct cnet_frame_hdr cnet_hdr;
	uint8_t addr4[6];
}__attribute__((packed));

struct cnet_frame_qos_data
{
	struct cnet_frame_hdr cnet_hdr;
	uint16_t qos;
}__attribute__((packed));

struct cnet_frame_qos_data_wds
{
	struct cnet_frame_hdr cnet_hdr;
	uint8_t addr4[6];
	uint16_t qos;
}__attribute__((packed));

struct cnet_frame_qos_data_ht
{
	struct cnet_frame_hdr cnet_hdr;
	uint16_t qos;
	uint32_t ht;
}__attribute__((packed));

struct cnet_frame_qos_data_merge
{
	struct cnet_frame_hdr cnet_hdr;
	uint8_t addr4[6];
	uint16_t qos;
	uint32_t ht;
}__attribute__((packed));

struct cnet_frame_qos_null
{
	struct cnet_frame_hdr cnet_hdr;
	uint16_t qos;
}__attribute__((packed));

#endif