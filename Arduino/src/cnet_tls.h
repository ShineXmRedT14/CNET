#ifndef CNET_TLS_H
#define CNET_TLS_H

#include <stdint.h>

#define CNET_TLS_TYPE_CCS 0x14
#define CNET_TLS_TYPE_ALERT 0x15
#define CNET_TLS_TYPE_HANDSHAKE 0x16
#define CNET_TLS_TYPE_DATA 0x17

#define CNET_TLS_VERSION_1_0 0x0301
#define CNET_TLS_VERSION_1_1 0x0302
#define CNET_TLS_VERSION_1_2 0x0303
#define CNET_TLS_VERSION_1_3 0x0304

#define CNET_TLS_HANDSHAKE_CLIENT 0x01
#define CNET_TLS_HANDSHAKE_SERVER 0x02
#define CNET_TLS_HANDSHAKE_CERT 0x0B
#define CNET_TLS_HANDSHAKE_SERVER_KE 0x0C
#define CNET_TLS_HANDSHAKE_SERVER_DONE 0x0E
#define CNET_TLS_HANDSHAKE_CLIENT_KE 0x10
#define CNET_TLS_HANDSHAKE_FINISHED 0x14

struct cnet_tls_record
{
	uint8_t type;
	uint16_t version;
	uint16_t len;
}__attribute__((packed));

struct cnet_tls_handshake
{
	uint8_t type;
	uint8_t len[3];
}__attribute__((packed));

#endif