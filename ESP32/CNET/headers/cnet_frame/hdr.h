#ifndef CNET_FRAME_HDR_H
#define CNET_FRAME_HDR_H

#include <stdint.h>
#include <endian.h>

#define CNET_FRAME_TYPE_MGT 0x0
#define CNET_FRAME_TYPE_CTL 0x1
#define CNET_FRAME_TYPE_DATA 0x2
#define CNET_FRAME_STYPE_ASSOC_REQ 0x0
#define CNET_FRAME_STYPE_ASSOC_RESP 0x1
#define CNET_FRAME_STYPE_REASSOC_REQ 0x2
#define CNET_FRAME_STYPE_REASSOC_RESP 0x3
#define CNET_FRAME_STYPE_PROBE_REQ 0x4
#define CNET_FRAME_STYPE_PROBE_RESP 0x5
#define CNET_FRAME_STYPE_ADV 0x6
#define CNET_FRAME_STYPE_RES7 0x7
#define CNET_FRAME_STYPE_BEACON 0x8
#define CNET_FRAME_STYPE_ATIM 0x9
#define CNET_FRAME_STYPE_DIASSOC 0xA
#define CNET_FRAME_STYPE_AUTH 0xB
#define CNET_FRAME_STYPE_DEAUTH 0xC
#define CNET_FRAME_STYPE_ACTION 0xD
#define CNET_FRAME_STYPE_ACTION_NA 0xE
#define CNET_FRAME_STYPE_RES15 0xF

#define CNET_FRAME_VERSION 0x0

#define CNET_FRAME_STYPE_TRIGGER 0x2
#define CNET_FRAME_STYPE_BFRPT 0x4
#define CNET_FRAME_STYPE_VNA 0x5
#define CNET_FRAME_STYPE_CTLE 0x6
#define CNET_FRAME_STYPE_CTLW 0x7
#define CNET_FRAME_STYPE_BAR 0x8
#define CNET_FRAME_STYPE_BA 0x9
#define CNET_FRAME_STYPE_PSP 0xA
#define CNET_FRAME_STYPE_RTS 0xB
#define CNET_FRAME_STYPE_CTS 0xC
#define CNET_FRAME_STYPE_ACK 0xD
#define CNET_FRAME_STYPE_CFE 0xE
#define CNET_FRAME_STYPE_CFEA 0xF

#define CNET_FRAME_STYPE_DATA 0x0
#define CNET_FRAME_STYPE_DATA_CFA 0x1
#define CNET_FRAME_STYPE_DATA_CFP 0x2
#define CNET_FRAME_STYPE_DATA_CFAP 0x3
#define CNET_FRAME_STYPE_NF 0x4
#define CNET_FRAME_STYPE_CF 0x5
#define CNET_FRAME_STYPE_CFA 0x5
#define CNET_FRAME_STYPE_CFP 0x6
#define CNET_FRAME_STYPE_CFAP 0x7
#define CNET_FRAME_STYPE_QOSD 0x8
#define CNET_FRAME_STYPE_QOSD_CFA 0x9
#define CNET_FRAME_STYPE_QOSD_CFP 0xA
#define CNET_FRAME_STYPE_QOSD_CFAP 0xB
#define CNET_FRAME_STYPE_QOS_NF 0xC
#define CNET_FRAME_STYPE_QOS_CFA 0xD
#define CNET_FRAME_STYPE_QOS_CFP 0xE
#define CNET_FRAME_STYPE_QOS_CFAP 0xF

#define CNET_FRAME_FLAG_TO_DS 0x01
#define CNET_FRAME_FLAG_FROM_DS 0x02
#define CNET_FRAME_FLAG_MORE_FRAG 0x04
#define CNET_FRAME_FLAG_RETRY 0x08
#define CNET_FRAME_FLAG_PWR 0x10
#define CNET_FRAME_FLAG_MORE_DATA 0x20
#define CNET_FRAME_FLAG_PROTECT 0x40
#define CNET_FRAME_FLAG_ORDER 0x80

struct cnet_frame_type
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint16_t version:2;
    uint16_t type:2;
    uint16_t stype:4;
    uint16_t flags:8;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint16_t flags:8;
    uint16_t stype:4;
    uint16_t type:2;
    uint16_t version:2;
#else
#error "Big/Little Endians bytes-error"
#endif
}__attribute__((packed));

struct cnet_frame_hdr
{
	struct cnet_frame_type cnet_type;
	uint16_t id;
	uint8_t addr1[6];
	uint8_t addr2[6];
	uint8_t addr3[6];
	uint16_t seq;
}__attribute__((packed));

#endif