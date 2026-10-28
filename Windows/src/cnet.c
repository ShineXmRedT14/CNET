#include <cnet.h>
#include <pcap/pcap.h>
#include <iprtrmib.h>
#include <minwindef.h>
#include <psdk_inc/_ip_types.h>
#include <stdint.h>
#include <time.h>
#include <winerror.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <ntddndis.h>
#include <string.h>

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

void CNET_BUFFER_CLONE(struct cnet_buffer *cbuff, int cl1, int cl2)
{
	if (cl1 >= cl2) {return;}

	for (int i = cl1; i < cl2; i++)
	{
		cbuff->buff[cl1][i] = cbuff->buff[cl2][i];
	}

	cbuff->size[cl1] = cbuff->size[cl2];
}

cnet_errno_t CNET_GET_CONFIGURE(struct cnet_route *route, pcap_t *socket)
{
	MIB_IPFORWARDROW ifgate;
	pcap_if_t *localdevs, *d;
	char errbuf[PCAP_ERRBUF_SIZE];

	if (GetBestRoute(0, 0, &ifgate) != NO_ERROR)
	{
		return CNET_N_FND;
	}

	DWORD current = ifgate.dwForwardIfIndex;

	if (pcap_findalldevs(&localdevs, errbuf) == -1) {
		return CNET_N_FND;
	}

	for (d = localdevs; d != NULL; d = d->next) {
		for (pcap_addr_t *a = d->addresses; a != NULL; a = a->next) {
			if (a->addr && a->addr->sa_family == AF_INET)
			{
				struct sockaddr_in *addrhdr = (struct sockaddr_in *)a->addr;
				char *fip = inet_ntoa(addrhdr->sin_addr);

				if (addrhdr->sin_addr.s_addr != inet_addr("127.0.0.1"))
				{
					route->iface = _strdup(d->name);
				}

				if (strcmp(fip, "127.0.0.1") != 0)
				{
					for (int i = 0; i < 16; i++) {
						if (i == 15) {
							route->local[i] = '\0';
							break;
						}

						route->local[i] = fip[i];
					}
				}
			}
		}
	}

	pcap_freealldevs(localdevs);
	if (route->iface == NULL || route->local[0] == '\0')
	{
		return CNET_N_FND;
	}

	ULONG oid = OID_802_3_CURRENT_ADDRESS;
	size_t len = 6;

	if (pcap_oid_get_request(socket, oid, route->smac, &len) != 0)
	{
		return CNET_N_FND;
	}

	MIB_IPFORWARDROW gater;
	DWORD gateway = GetBestRoute(0, 0, &gater);
	if (gateway != NO_ERROR) {
		return CNET_N_FND;
	}
	route->gateway.u32ip = gater.dwForwardNextHop;

	unsigned long bufflen = 0;
	PMIB_IPNETTABLE arp = NULL;

	GetIpNetTable(NULL, &bufflen, FALSE);

	arp = (PMIB_IPNETTABLE)malloc(bufflen);
	if (arp == NULL) {
		return CNET_N_FND;
	}

	if (GetIpNetTable(arp, &bufflen, FALSE) == NO_ERROR)
	{
		for (DWORD i = 0; i < arp->dwNumEntries; i++) {
			if (arp->table[i].dwAddr == route->gateway.u32ip)
			{
				for (int b = 0; b < 6; b++) {
					route->dmac[b] = arp->table[i].bPhysAddr[b];
				}

				free(arp);
				break;
			}
		}
	}

	free(arp);
	if (route->dmac[0] == 0x00 && route->dmac[1] == 0x00 && route->dmac[2] == 0x00 && route->dmac[3] == 0x00 && route->dmac[4] == 0x00 && route->dmac[5] == 0x00)
	{
		return CNET_N_FND;
	}

	return CNET_FND;
}

void CNET_BUFFER_PACKETS(struct cnet_buffer *cbuf, int ind)
{
	struct pcap_pkthdr pkthdr;
	pkthdr.ts.tv_sec = 0;
	pkthdr.ts.tv_usec = 0;

	if (cbuf->pos >= 64 || cbuf->pos >= ind) {return;}

	for (int i = cbuf->pos; i < ind; i++) {
		pkthdr.len = (uint32_t)cbuf->size[i];
		pkthdr.caplen = (uint32_t)cbuf->size[i];

		if (pcap_sendqueue_queue(cbuf->buffer, &pkthdr, cbuf->buff[i]) < 0) {
			continue;
		}

		cbuf->pos++;
	}
};

void CNET_BUFFER_INIT(struct cnet_buffer *cbuf)
{
	CNET_MEMSET(cbuf, sizeof(*cbuf));

	size_t pool = APY * (APX + sizeof(struct pcap_pkthdr));
	cbuf->buffer = pcap_sendqueue_alloc((uint32_t)pool);
}

pcap_t* CNET_SOCK(struct cnet_route *route)
{
	char errbuf[PCAP_ERRBUF_SIZE];

	pcap_t *socket = pcap_open_live((const char *)route->iface, 65535, 1, 1, errbuf);
	if (socket == NULL) {
		fprintf(stderr, "CNET-SOCK/CREATE-ERR: %s\n", errbuf);
		return NULL;
	}

	if (pcap_setmintocopy(socket, 0) != 0)
	{
		fprintf(stderr, "CNET-SOCK/OPTIMIZATION-WARNING/ERR: pcap_setmintocopy()\n");
	}

	return socket;
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