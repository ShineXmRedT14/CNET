#include <cnet.h>

int CNET_RAND(int min, int max)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	uint32_t xorstate = 1;
	cnet_xorrand((uint32_t)(ts.tv_sec ^ ts.tv_nsec), &xorstate);
	
	uint32_t x = xorstate;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	xorstate = x;
	
	return min + (x % (max - min + 1));
}

void CNET_SCTP_CSUM(void *sctp, size_t len)
{
	uint32_t sum32[256];
	
	for (uint32_t i = 0; i < 256; i++)
	{
		uint32_t crc = i;
		for (uint32_t j = 0; j < 8; j++)
		{
			crc = (crc & 1)?(crc >> 1)^0x82F63B78:(crc >> 1);
		}
		sum32[i] = crc;
	}
	
	uint8_t *data = (uint8_t *)sctp;
	uint32_t crc32 = 0xFFFFFFFF;
	
	for (size_t i = 0; i < len; i++)
	{
		crc32 = sum32[(crc32 ^ data[i]) & 0xFF] ^ (crc32 >> 8);
	}
	
	uint32_t csum = crc32 ^ 0xFFFFFFFF;
	struct cnet_sctp *csctp = (struct cnet_sctp *)sctp;
	
	csctp->sum = csum;
}

void CNET_MAC_BYTES(void *ceth, char mac[])
{
	struct cnet_ether_bytes *cether = (struct cnet_ether_bytes *)ceth;
	CNET_MEMSET(cether, sizeof(*cether));
	sscanf(mac, "%hhu:%hhu:%hhu:%hhu:%hhu:%hhu", &cether->bytes[0], &cether->bytes[1], &cether->bytes[2], &cether->bytes[3], &cether->bytes[4], &cether->bytes[5]);
}

void CNET_IP_BYTES(void *cipb, char sip[])
{
	struct cnet_ip_bytes *cipbyte = (struct cnet_ip_bytes *)cipb;
	CNET_MEMSET(cipbyte, sizeof(*cipbyte));
	sscanf(sip, "%hhu.%hhu.%hhu.%hhu", &cipbyte->bytes.b8its[0], &cipbyte->bytes.b8its[1], &cipbyte->bytes.b8its[2], &cipbyte->bytes.b8its[3]);
}

uint16_t CNET_L3_CSUM(void *ndata, size_t nlen)
{
	uint32_t sum = 0;
	uint16_t *rdata = (uint16_t *)ndata;
	
	while (nlen > 1)
	{
		sum += *rdata++;
		nlen -= 2;
	}
	
	if (nlen)
	{
		uint8_t temp[2] = {*(uint8_t *)rdata, 0x00};
		sum += *(uint16_t *)temp;
	}
	
	while (sum >> 16)
	{
		sum = (sum & 0xFFFF)+(sum >> 16);
	}
	
	return (uint16_t)(~sum);
}

uint16_t CNET_L4_CSUM(void *cip, void *ndata, size_t nlen)
{
	size_t tcplen = nlen;
	struct cnet_ip *cnip = (struct cnet_ip *)cip;
	uint32_t sum = 0;
	uint16_t *rdata = (uint16_t *)ndata;
	
	while (nlen > 1)
	{
		sum += *rdata++;
		nlen -= 2;
	}

	sum += CNET_BIG16((uint16_t)(tcplen));
	sum += CNET_BIG16((uint16_t)(0x00));
	sum += CNET_BIG16((uint16_t)(cnip->proto));
	sum += cnip->src.s32rc;
	sum += cnip->dst.d32st;
	
	if (nlen)
	{
		uint8_t temp[2] = {*(uint8_t *)rdata, 0x00};
		sum += *(uint16_t *)temp;
	}
	
	while (sum >> 16)
	{
		sum = (sum & 0xFFFF)+(sum >> 16);
	}
	
	return (uint16_t)(~sum);
}

struct cnet_ntp_dtime CNET_NTP_TIMESTAMP()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	
	struct cnet_ntp_dtime ts;
	ts.sec = tv.tv_sec + 2208988800UL;
	ts.frc = (uint32_t)((double)tv.tv_usec * (4294967296.0/1000000.0));
	
	return ts;
} 