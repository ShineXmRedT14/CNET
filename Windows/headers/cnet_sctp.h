#ifndef CNET_SCTP_H
#define CNET_SCTP_H

#include <stdint.h>

#define CNET_SCTP_DATA 0x00
#define CNET_SCTP_INIT 0x01
#define CNET_SCTP_INIT_ACK 0x02
#define CNET_SCTP_SACK 0x03
#define CNET_SCTP_HBEAT 0x04
#define CNET_SCTP_HBEAT_ACK 0x05
#define CNET_SCTP_ABORT 0x06
#define CNET_SCTP_SHUTDOWN 0x07
#define CNET_SCTP_SHUTDOWN_ACK 0x08
#define CNET_SCTP_ERR 0x09
#define CNET_SCTP_COOKIE_ECHO 0x0A
#define CNET_SCTP_COOKIE_ACK 0x0B
#define CNET_SCTP_ECN_ECNE 0x0C
#define CNET_SCTP_ECN_CWR 0x0D
#define CNET_SCTP_SHUTDOWN_COMPLETE 0x0E
#define CNET_SCTP_AUTH 0x0F
#define CNET_SCTP_TSN 0xC0
#define CNET_SCTP_ASCONF 0xC1
#define CNET_SCTP_ASCONF_ACK 0x80

#define CNET_SCTP_DATA_E 0x01
#define CNET_SCTP_DATA_B 0x02
#define CNET_SCTP_DATA_U 0x04
#define CNET_SCTP_DATA_I 0x08

#define CNET_SCTP_PARAM_HBEAT_INFO 0x0001
#define CNET_SCTP_PARAM_IPV4_ADDR 0x0005
#define CNET_SCTP_PARAM_IPV6_ADDR 0x0006
#define CNET_SCTP_PARAM_STCOOKIE 0x0007
#define CNET_SCTP_PARAM_UNR_PARAM 0x0008
#define CNET_SCTP_PARAM_COOKIE_PRESERV 0x0009
#define CNET_SCTP_PARAM_HOST_ADDR 0x000B
#define CNET_SCTP_PARAM_SUPPORTED_ADDR 0x000C
#define CNET_SCTP_PARAM_ECN_CAP 0x8000
#define CNET_SCTP_PARAM_RANDOM 0x8002
#define CNET_SCTP_PARAM_LCHUNK 0x8003
#define CNET_SCTP_PARAM_HMAC 0x8004
#define CNET_SCTP_PARAM_SUPPORTED_EXT 0x8008
#define CNET_SCTP_PARAM_SUPPORTED_TSN 0xC000

#define CNET_SCTP_ABORT_T 0x01
#define CNET_SCTP_SHUTDOWN_T 0x01

#define CNET_SCTP_ERROR_INVALID_STREAM 0x0001
#define CNET_SCTP_ERROR_MISSING_PARAM 0x0002
#define CNET_SCTP_ERROR_STALE_COOKIE 0x0003
#define CNET_SCTP_ERROR_OOR 0x0004
#define CNET_SCTP_ERROR_UNR_ADDR 0x0005
#define CNET_SCTP_ERROR_UNR_CHUNK 0x0006
#define CNET_SCTP_ERROR_INVALID_PARAM 0x0007
#define CNET_SCTP_ERROR_UNR_PARAM 0x0008
#define CNET_SCTP_ERROR_NUSER_DATA 0x0009
#define CNET_SCTP_ERROR_COOKIE_SHUTDOWN 0x000A
#define CNET_SCTP_ERROR_RASSOC 0x000B
#define CNET_SCTP_ERROR_UIA 0x000C
#define CNET_SCTP_ERROR_PV 0x000D

#define CNET_SCTP_PARAM_ADD_IP        0xC001
#define CNET_SCTP_PARAM_DEL_IP        0xC002
#define CNET_SCTP_PARAM_ERR_CAUSE     0xC003
#define CNET_SCTP_PARAM_SET_PRIMARY   0xC004
#define CNET_SCTP_PARAM_SUCCESS       0xC005
#define CNET_SCTP_PARAM_ADAPT_LAYER   0xC006

struct cnet_sctp
{
	uint16_t sport;
	uint16_t dport;
	uint32_t vtag;
	uint32_t sum;
}__attribute__((packed));

struct cnet_sctp_chunk
{
	uint8_t type;
	uint8_t flags;
	uint16_t len;
}__attribute__((packed));

struct cnet_sctp_data_chunk
{
	uint8_t type;
	uint8_t flags; //data flags
	uint16_t len;
	uint32_t tsn; //seq
	uint16_t sid; //stream id
	uint16_t sseq; //stream seq
	uint32_t ppid; //payload proto id
}__attribute__((packed));

struct cnet_sctp_init_chunk
{
	uint8_t type;
	uint8_t flags; // 0
	uint16_t len;
	uint32_t itag; //init tag
	uint32_t arwnd;
	uint16_t nos;
	uint16_t nis;
	uint32_t itsn; //init tsn
}__attribute__((packed));

struct cnet_sctp_init_ack_chunk
{
	uint8_t type;
	uint8_t flags;
	uint16_t len;
	uint32_t itag;
	uint32_t arwnd;
	uint16_t nos;
	uint16_t nis;
	uint32_t itsn;
}__attribute__((packed));

struct cnet_sctp_sack_chunk
{
	uint8_t type;
	uint8_t flags;
	uint16_t len;
	uint32_t cta;
	uint32_t arwnd;
	uint16_t nga;
	uint16_t ndt;
}__attribute__((packed));

struct cnet_sctp_param
{
	uint16_t type;
	uint16_t len;
}__attribute__((packed));

struct cnet_sctp_gap_ack
{
	uint16_t start;
	uint16_t end;
}__attribute__((packed));

struct cnet_sctp_hbeat_chunk
{
	uint8_t type;
	uint8_t flags; // 0
	uint16_t len;
}__attribute__((packed));

struct cnet_sctp_hbeat_ack_chunk
{
	uint8_t type;
	uint8_t flags;
	uint16_t len;
}__attribute__((packed));

struct cnet_sctp_abort_chunk
{
	uint8_t type;
	uint8_t flags; //Abort T
	uint16_t len;
}__attribute__((packed));

struct cnet_sctp_shutdown_chunk
{
	uint8_t type;
	uint8_t flags;
	uint16_t len;
	uint32_t cta;
}__attribute__((packed));

struct cnet_sctp_shutdown_ack_chunk
{
	uint8_t type;
	uint8_t flags;
	uint16_t len;
}__attribute__((packed));

struct cnet_sctp_shutdown_complete_chunk
{
	uint8_t type;
	uint8_t flags; //Shutdown T
	uint16_t len;
}__attribute__((packed));

struct cnet_sctp_error_chunk
{
	uint8_t type;
	uint8_t flags;
	uint16_t len;
}__attribute__((packed));

struct cnet_sctp_error_c_chunk
{
	uint16_t cc;
	uint16_t cl;
}__attribute__((packed));

struct cnet_sctp_cookie_echo_chunk
{
	uint8_t type;
	uint8_t flags;
	uint16_t len;
}__attribute__((packed));

struct cnet_sctp_cookie_ack_chunk
{
	uint8_t type;
	uint8_t flags;
	uint16_t len;
}__attribute__((packed));

struct cnet_sctp_tsn_stream
{
	uint16_t sid;
	uint16_t sseq;
}__attribute__((packed));

struct cnet_sctp_tsn_chunk
{
	uint8_t type;
	uint8_t flags;
	uint16_t len;
	uint32_t nct;
}__attribute__((packed));

struct cnet_sctp_ecne_chunk
{
	uint8_t type;
	uint8_t flags;
	uint16_t len;
	uint32_t ltsn;
}__attribute__((packed));

struct cnet_sctp_cwr_chunk
{
	uint8_t type;
	uint8_t flags; // 0
	uint16_t len;
	uint32_t ltsn;
}__attribute__((packed));

struct cnet_sctp_auth_chunk
{
	uint8_t type;
	uint8_t flags; // 0
	uint16_t len;
	uint16_t skey;
	uint16_t hmac;
}__attribute__((packed));

struct cnet_sctp_asconf_chunk
{
	uint8_t type;
	uint8_t flags; // 0
	uint16_t len;
	uint32_t serial;
}__attribute__((packed));

struct cnet_sctp_asconf_ack_chunk
{
	uint8_t type;
	uint8_t flags; // 0
	uint16_t len;
	uint32_t serial;
}__attribute__((packed));

struct cnet_sctp_asconf_param
{
	uint16_t type;
	uint16_t len;
	uint32_t id;
}__attribute__((packed));

struct cnet_sctp_asconf_res
{
	uint16_t type;
	uint16_t len;
	uint32_t id;
}__attribute__((packed));

#endif