#ifndef CNET_DNS_H
#define CNET_DNS_H

#include <stdint.h>

#define CNET_DNS_FLAG_QR 0x8000
#define CNET_DNS_FLAG_OPCODE 0x7800
#define CNET_DNS_FLAG_AA 0x0400
#define CNET_DNS_FLAG_TC 0x0200
#define CNET_DNS_FLAG_RD 0x0100
#define CNET_DNS_FLAG_RA 0x0080
#define CNET_DNS_FLAG_RES 0x0070
#define CNET_DNS_FLAG_RCODE 0x000F

#define CNET_DNS_OPCODE_QUERY 0x0000
#define CNET_DNS_OPCODE_IQUERY 0x0001
#define CNET_DNS_OPCODE_STATUS 0x0002
#define CNET_DNS_OPCODE_NOTIFY 0x0004
#define CNET_DNS_OPCODE_UPDATE 0x0005

#define CNET_DNS_RCODE_NERR 0x0000
#define CNET_DNS_RCODE_FMERR 0x0001
#define CNET_DNS_RCODE_SFAIL 0x0002
#define CNET_DNS_RCODE_NXDOMAIN 0x0003
#define CNET_DNS_RCODE_NTIMP 0x0004
#define CNET_DNS_RCODE_REFUSED 0x0005

#define CNET_DNS_TYPE_A 0x0001
#define CNET_DNS_TYPE_NS 0x0002
#define CNET_DNS_TYPE_CNAME 0x0005
#define CNET_DNS_TYPE_SOA 0x0006
#define CNET_DNS_TYPE_PTR 0x000C
#define CNET_DNS_TYPE_MX 0x000F
#define CNET_DNS_TYPE_TXT 0x0010
#define CNET_DNS_TYPE_AAAA 0x001C
#define CNET_DNS_TYPE_SRV 0x0021
#define CNET_DNS_TYPE_OPT 0x0029

#define CNET_DNS_CLASS 0x0001

struct cnet_dns
{
	uint16_t id;
	uint16_t flags;
	uint16_t qdc;
	uint16_t anc;
	uint16_t nsc;
	uint16_t arc;
}__attribute__((packed));

struct cnet_dns_question
{
	uint16_t type;
	uint16_t class;
}__attribute__((packed));

struct cnet_dns_record
{
	uint16_t type;
	uint16_t class;
	uint32_t ttl;
	uint16_t len;
}__attribute__((packed));

#endif