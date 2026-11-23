#ifndef CNET_HELPER_H
#define CNET_HELPER_H

#include <string.h>
#include <stdint.h>

#define CNET_LAYER_ETHER 2
#define CNET_LAYER_IP 3
#define CNET_BROADCAST {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
#define CNET_IP_ICMP 0x01
#define CNET_IP_TCP 0x06
#define CNET_IP_UDP 0x11
#define CNET_PORT_FTP_DAT 20
#define CNET_PORT_FTP_SET 21
#define CNET_PORT_SSH 22
#define CNET_PORT_TELNET 23
#define CNET_PORT_SMTP 25
#define CNET_PORT_DNS 53
#define CNET_PORT_DHCP_IP 67
#define CNET_PORT_DHCP_CLIENT 68
#define CNET_PORT_TFTP 69
#define CNET_PORT_HTTP 80
#define CNET_PORT_POP3 110
#define CNET_PORT_NTP 123
#define CNET_PORT_IMAP 143
#define CNET_PORT_SNMP_N1 161
#define CNET_PORT_SNMP_N2 162
#define CNET_PORT_LDAP 389
#define CNET_PORT_HTTPS 443
#define CNET_PORT_SMB 445
#define CNET_PORT_SYSLOG 514
#define CNET_PORT_LDAPS 636
#define CNET_PORT_IMAPS 993
#define CNET_PORT_POP3S 995
#define CNET_PORT_MSSQL 1433
#define CNET_PORT_ORACLE 1521
#define CNET_PORT_MQTT 1883
#define CNET_PORT_MYSQL 3306
#define CNET_PORT_RDP 3389
#define CNET_PORT_SIP_N1 5060
#define CNET_PORT_SIP_N2 5061
#define CNET_PORT_PSQL 5432
#define CNET_PORT_RMQ 5672
#define CNET_PORT_VNC 5900
#define CNET_PORT_REDIS 6379
#define CNET_PORT_HTTP8 8080
#define CNET_PORT_HTTPS8 8443
#define CNET_PORT_FPM 9000
#define CNET_PORT_KAFKA 9092
#define CNET_PORT_MDB 27017

#define MEGASIZE 4
#define SIZEBUFF() (MEGASIZE*1024*1024)

typedef enum
{
	CNET_SCFL = 0,
	CNET_ERR_IFADDRS = -1,
	CNET_ERR_SOCKET = -2,
	CNET_ERR_IOCTL = -3,
	CNET_ERR_NIFACE = -4,
    CNET_OVERFLOW = -5,
    CNET_UNSUPPORT_IP_PROTOCOL = -6,
    CNET_TCP_INFO_BAD_CONNECTION = -7,
	CNET_ERR_SETSOCKOPT_TX = -8,
	CNET_ERR_RING_MAP_FAILED = -9,
	CNET_ERR_IFINDEX = -10,
	CNET_ERR_BIND = -11,
	CNET_ERR_SETSOCKOPT_QDISC = -12,
	CNET_ERR_SETSOCKOPT_LOSS = -13
} cnet_errno_t;

struct cnet_dipv6
{
	int version;
	int tc;
	uint32_t flow;
};

struct cnet_dicmpv6
{
	int r;
	int s;
	int o;
};

struct cnet_dvlan
{
	int v;
	int d;
	int p;
};

struct cnet_ntp_dtime
{
	uint32_t sec;
	uint32_t frc;
};

struct cnet_expand_info
{
    uint8_t *buffer;
    size_t buffer_size;

    void *main;
    size_t main_count;

    void *options[64];
    size_t options_size[64];
    size_t count;
};

struct cnet_frame_fcs
{
    union
    {
        uint32_t fcs32;
        uint16_t fcs16[2];
        uint8_t fcs8[4];
    } fcs;
}__attribute__((packed));

#define CNET_MAX_IPLEN 16
#define CNET_MAX_ETHLEN 18

union cnet_packet_fast_config
{
	struct {
      int doff;
      uint16_t flags;
      int src;
      int dst;
      uint32_t seq;
      uint32_t aseq;
      int win;
      int uptr;
	} tcp;

	struct {
      int src;
      int dst;
      int len;
	} udp;

	struct {
	  int type;
	  int id;
	  int frag;
	  int ttl;
	  int proto;

	  char src[CNET_MAX_IPLEN];
	  char dst[CNET_MAX_IPLEN];
	} ip;

	struct {
	  char src[CNET_MAX_ETHLEN];
	  char dst[CNET_MAX_ETHLEN];
	  int next;
	} eth;
};

#define CNET_FRAME_MACROS_FCS32 fcs.fcs32
#define CNET_FRAME_MACROS_FCS16 fcs.fcs16
#define CNET_FRAME_MACROS_FCS8 fcs.fcs8

static inline cnet_errno_t CNET_HELPER_SET_IP_SRC(union cnet_packet_fast_config *pfc, const char *src);
static inline cnet_errno_t CNET_HELPER_SET_IP_DST(union cnet_packet_fast_config *pfc, const char *dst);
static inline cnet_errno_t CNET_HELPER_SET_ETH_SRC(union cnet_packet_fast_config *pfc, const char *src);
static inline cnet_errno_t CNET_HELPER_SET_ETH_DST(union cnet_packet_fast_config *pfc, const char *dst);

static inline cnet_errno_t CNET_HELPER_SET_IP_SRC(union cnet_packet_fast_config *pfc, const char *src)
{
    int src_len = strlen(src);

    if (src_len+1 >= CNET_MAX_IPLEN) return CNET_OVERFLOW;

    for (int i = 0; i <= src_len; i++) {
        if (i != src_len) {
            pfc->ip.src[i] = src[i];

            continue;
        }

        pfc->ip.src[i] = '\0';
    }

    return CNET_SCFL;
}

static inline cnet_errno_t CNET_HELPER_SET_IP_DST(union cnet_packet_fast_config *pfc, const char *dst)
{
    int dst_len = strlen(dst);

    if (dst_len+1 >= CNET_MAX_IPLEN) return CNET_OVERFLOW;

    for (int i = 0; i <= dst_len; i++) {
        if (i != dst_len) {
            pfc->ip.dst[i] = dst[i];

            continue;
        }

        pfc->ip.dst[i] = '\0';
    }

    return CNET_SCFL;
}

static inline cnet_errno_t CNET_HELPER_SET_ETH_SRC(union cnet_packet_fast_config *pfc, const char *src)
{
    int src_len = strlen(src);

    if (src_len+1 >= CNET_MAX_ETHLEN) return CNET_OVERFLOW;

    for (int i = 0; i <= src_len; i++) {
        if (i != src_len) {
            pfc->eth.src[i] = src[i];

            continue;
        }

        pfc->eth.src[i] = '\0';
    }

    return CNET_SCFL;
}

static inline cnet_errno_t CNET_HELPER_SET_ETH_DST(union cnet_packet_fast_config *pfc, const char *dst)
{
    int dst_len = strlen(dst);

    if (dst_len+1 >= CNET_MAX_ETHLEN) return CNET_OVERFLOW;

    for (int i = 0; i <= dst_len; i++) {
        if (i != dst_len) {
            pfc->eth.dst[i] = dst[i];

            continue;
        }

        pfc->eth.dst[i] = '\0';
    }

    return CNET_SCFL;
}

#endif