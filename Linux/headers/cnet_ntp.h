#ifndef CNET_NTP_H
#define CNET_NTP_H

#include <stdint.h>
#include <endian.h>

#define CNET_NTP_VERSION 0x4
#define CNET_NTP_STRAT_KOD 0x00
#define CNET_NTP_STRAT_PRIMARY 0x01

#define CNET_NTP_LI_NONE 0x0
#define CNET_NTP_LI_LM61 0x1
#define CNET_NTP_LI_LM59 0x2
#define CNET_NTP_LI_ALARM 0x3

#define CNET_NTP_MD_RES 0x0
#define CNET_NTP_MD_SACT 0x1
#define CNET_NTP_MD_SPAS 0x2
#define CNET_NTP_MD_CLIENT 0x3
#define CNET_NTP_MD_SERVER 0x4
#define CNET_NTP_MD_BCAST 0x5
#define CNET_NTP_MD_CNTRL 0x6
#define CNET_NTP_MD_PRIVATE 0x7

struct cnet_short
{
	uint16_t sec;
	uint16_t frc;
}__attribute__((packed));

struct cnet_ntp_timestamp
{
	uint32_t sec;
	uint32_t frc;
}__attribute__((packed));

struct cnet_ntp
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t li:2;
    uint8_t vn:3;
    uint8_t md:3;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint8_t md:3;
    uint8_t vn:3;
    uint8_t li:2;
#else
#error "Big/Little Endians bytes-error"
#endif
    uint8_t strat;
    uint8_t poll;
    uint8_t prec;
    
    struct cnet_ntp_short delay;
    struct cnet_ntp_short disper;
    
    uint32_t ref_id;
    
    struct cnet_ntp_timestamp ref_tm;
    struct cnet_ntp_timestamp org_tm;
    struct cnet_ntp_timestamp recv_tm;
    struct cnet_ntp_timestamp trsm_tm;
}__attribute__((packed));

#endif