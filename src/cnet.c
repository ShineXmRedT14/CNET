#include <cnet.h>

/*

definition of CNET_PLATFORM_LINUX in the <cnet_platform.h> file
<cnet_platform.h> already included in the cnet.h file

*/

// Functions for Linux platform;
#if defined(CNET_PLATFORM_LINUX)
    cnet_errno_t CNET_GET_TCP_INFO(int socket, struct cnet_tcp_info *info)
    {
	    struct tcp_info linfo;
	    socklen_t len = sizeof(linfo);

	    CNET_MEMSET(&linfo, len);
	    CNET_MEMSET(info, sizeof(*info));

	    if (getsockopt(socket, CNET_IP_TCP, TCP_INFO, &linfo, &len) < 0) return CNET_ERR_SOCKET;

	    info->state = linfo.tcpi_state;
	    info->cwnd = linfo.tcpi_snd_cwnd;
	    info->rtt = linfo.tcpi_rtt;
	    info->rttv = linfo.tcpi_rttvar;
	    info->smss = linfo.tcpi_snd_mss;
	    info->unack = linfo.tcpi_unacked;
	    info->lost = linfo.tcpi_lost;
	    info->probes = linfo.tcpi_probes;
	    info->off = linfo.tcpi_backoff;
	    info->options = linfo.tcpi_options;
	    info->sscale = linfo.tcpi_snd_wscale;
	    info->rscale = linfo.tcpi_rcv_wscale;
	    info->rto = linfo.tcpi_rto;
	    info->ato = linfo.tcpi_ato;
	    info->rmss = linfo.tcpi_rcv_mss;
	    info->pmtu = linfo.tcpi_pmtu;
	    info->sack = linfo.tcpi_sacked;
	    info->ret = linfo.tcpi_retrans;
	    info->ackets = linfo.tcpi_retransmits;
	    info->las = linfo.tcpi_last_ack_sent;
	    info->lds = linfo.tcpi_last_data_sent;
	    info->lar = linfo.tcpi_last_ack_recv;
	    info->ldr = linfo.tcpi_last_data_recv;
	    info->reorder = linfo.tcpi_reordering;
	    info->rspace = linfo.tcpi_rcv_space;
	    info->tret = linfo.tcpi_total_retrans;
	    info->advmss = linfo.tcpi_advmss;
	    info->sth = linfo.tcpi_snd_ssthresh;
	    info->rrtt = linfo.tcpi_rcv_rtt;
	    info->rth = linfo.tcpi_rcv_ssthresh;

	    switch (linfo.tcpi_ca_state)
	    {
		    case TCP_CA_Open:
		      info->cnet_state_ca = CNET_TCP_OPEN;
		      break;
		    case TCP_CA_Disorder:
		      info->cnet_state_ca = CNET_TCP_DISC;
		      break;
		    case TCP_CA_CWR:
		      info->cnet_state_ca = CNET_TCP_CWR;
		      break;
		    case TCP_CA_Recovery:
		      info->cnet_state_ca = CNET_TCP_RECOV;
		      break;
		    case TCP_CA_Loss:
		      info->cnet_state_ca = CNET_TCP_LOSS;
		      break;

		    default: return CNET_TCP_INFO_BAD_CONNECTION;
	    }

    	return CNET_SCFL;
    }

	struct cnet_ring CNET_RING_INIT(int layer, struct cnet_route *route)
    {
	    struct cnet_ring ring;
	    CNET_MEMSET(&ring, sizeof(ring));
	    ring.code = 0;

	    int socket_type = SOCK_RAW;
	    if (layer == CNET_LAYER_IP) socket_type = SOCK_DGRAM;

	    ring.sockfd = socket(AF_PACKET, socket_type, CNET_BIG16(0x0800));
	    if (ring.sockfd < 0) {
	    	ring.code = CNET_ERR_SOCKET;
		    return ring;
	    }

	    struct tpacket_req req;
	    CNET_MEMSET(&req, sizeof(req));

	    req.tp_block_size = 4096 * 16;
	    req.tp_block_nr = 4;
	    req.tp_frame_size = 2048;
	    req.tp_frame_nr = (req.tp_block_size * req.tp_block_nr)/req.tp_frame_size;

	    if (setsockopt(ring.sockfd, SOL_PACKET, PACKET_TX_RING, (void *)&req, sizeof(req)) < 0) {
		    ring.code = CNET_ERR_SETSOCKOPT_TX;
		    close(ring.sockfd);
		    ring.sockfd = -1;
		    return ring;
	    }

	    ring.ring_size = req.tp_block_size * req.tp_block_nr;
	    ring.ring_ptr = mmap(NULL, ring.ring_size, PROT_READ | PROT_WRITE, MAP_SHARED, ring.sockfd, 0);
	    if (ring.ring_ptr == MAP_FAILED) {
		    ring.code = CNET_ERR_RING_MAP_FAILED;
		    close(ring.sockfd);
		    ring.sockfd = -1;
		    return ring;
	    }

	    struct ifreq ifr;
	    CNET_MEMSET(&ifr, sizeof(ifr));
	    strncpy(ifr.ifr_name, route->active, IFNAMSIZ - 1);

	    if (ioctl(ring.sockfd, SIOCGIFINDEX, &ifr) < 0) {
	    	ring.code = CNET_ERR_IFINDEX;
		    munmap(ring.ring_ptr, ring.ring_size);
		    close(ring.sockfd);
		    ring.sockfd = -1;
		    return ring;
	    }

	    struct sockaddr_ll dll;
	    CNET_MEMSET(&dll, sizeof(dll));

	    dll.sll_family = AF_PACKET;
	    dll.sll_ifindex = ifr.ifr_ifindex;
	    dll.sll_protocol = CNET_BIG16(0x0800);

	    if (bind(ring.sockfd, (struct sockaddr *)&dll, sizeof(dll)) < 0) {
		    ring.code = CNET_ERR_BIND;
		    munmap(ring.ring_ptr, ring.ring_size);
		    close(ring.sockfd);
		    ring.sockfd = -1;
		    return ring;
	    }

	    int flg = 1;
	    if (setsockopt(ring.sockfd, SOL_PACKET, PACKET_QDISC_BYPASS, &flg, sizeof(flg)) < 0) {
		    ring.code = CNET_ERR_SETSOCKOPT_QDISC;
		    munmap(ring.ring_ptr, ring.ring_size);
		    close(ring.sockfd);
		    ring.sockfd = -1;
		    return ring;
	    }

	    int disc = 1;
	    if (setsockopt(ring.sockfd, SOL_PACKET, PACKET_LOSS, &disc, sizeof(disc)) < 0) {
		    ring.code = CNET_ERR_SETSOCKOPT_LOSS;
		    munmap(ring.ring_ptr, ring.ring_size);
		    close(ring.sockfd);
		    ring.sockfd = -1;
		    return ring;
	    }

	    ring.frame_size = req.tp_frame_size;
	    ring.frame_count = req.tp_frame_nr;
	    ring.position = 0x00000000;

	    return ring;
    }

	struct sockaddr_ll CNET_SOCK_ADDR_LL(struct cnet_route *route, int sock)
    {
	    struct ifreq ifr;
	    struct sockaddr_ll addr;
	    CNET_MEMSET(&addr, sizeof(addr));
    	CNET_MEMSET(&ifr, sizeof(ifr));
	    strncpy(ifr.ifr_name, route->active, IFNAMSIZ-1);
	    ioctl(sock, SIOCGIFINDEX, &ifr);

	    addr.sll_family = AF_PACKET;
	    addr.sll_ifindex = ifr.ifr_ifindex;

	    return addr;
    }

    void CNET_BUFFER_CLONE(struct cnet_buffer *cbuff, int cl1, int cl2)
    {
	    for (int i = 0; i < APX; i++)
	    {
		    cbuff->buff[cl1][i] = cbuff->buff[cl2][i];
	    }

	    cbuff->size[cl1] = cbuff->size[cl2];
    }

	cnet_errno_t CNET_CONFIGURE(struct cnet_route *route)
	{
		cnet_errno_t switcher = CNET_SCFL;

		struct ifaddrs *ifaddr;
		struct ifaddrs *ifa;

		if (getifaddrs(&ifaddr) == -1)
		{
			fprintf(stderr, "GET-IF-GETIF-ERR: %s (errno=%d)\n", strerror(errno), errno);
			return CNET_ERR_IFADDRS;
		}

		size_t ifac = 0;
		for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
		{
			strncpy(route->iface[route->count], ifa->ifa_name, IFNAMSIZ-1);
			route->iface[route->count][IFNAMSIZ-1] = '\0';

			if (route->count >= CNET_IFMAX) {break;}

			if (
				strcmp(route->iface[ifac], "lo") != 0 && strcmp(route->iface[ifac], "lo0") != 0 &&
				strncmp(route->iface[ifac], "tun", 3) != 0 && strncmp(route->iface[ifac], "ppp", 3) != 0
			) {
				unsigned int ifindex = if_nametoindex(route->iface[ifac]);
		    	if (ifindex > 0) strncpy(route->active, route->iface[ifac], IFNAMSIZ-1);
			}

			route->count++;
			ifac++;
		}

		freeifaddrs(ifaddr);

		int usock = socket(AF_INET, SOCK_DGRAM, 0);

		if (usock < 0)
		{
			fprintf(stderr, "GET-IP-SOCK-ERR: %s (errno=%d)\n", strerror(errno), errno);
			return CNET_ERR_SOCKET;
		}

		struct ifreq ifr;
		struct ifreq *ifrq = &ifr;
		CNET_MEMSET(ifrq, sizeof(*ifrq));

		for (int i = 0; i < route->count; i++) {
			if (strcmp(route->iface[i], "lo") == 0 || strcmp(route->iface[i], "lo0") == 0) continue;

			strncpy(ifrq->ifr_name, route->iface[i], IFNAMSIZ-1);
			ifrq->ifr_name[IFNAMSIZ-1] = '\0';

			if (ioctl(usock, SIOCGIFFLAGS, ifrq) == 0) {
				if ((ifrq->ifr_flags & IFF_UP) && !(ifrq->ifr_flags & IFF_LOOPBACK)) {
					break;
				}
			}

			ifrq->ifr_name[0] = '\0';
		}

		if (ifrq->ifr_name[0] == '\0') {
			close(usock);
			return CNET_ERR_NIFACE;
		}

		if (ioctl(usock, SIOCGIFADDR, ifrq) == -1)
		{
			fprintf(stderr, "GET-IP-IOCTL-ERR: %s (errno=%d)\n", strerror(errno), errno);
			close(usock);
			return CNET_ERR_IOCTL;
		}

		struct sockaddr_in *addr = (struct sockaddr_in *)&ifrq->ifr_addr;

		uint8_t *ibytes = (uint8_t *)&addr->sin_addr;
		for (int i = 0; i < 4; i++) {route->ip.i8p[i]=ibytes[i];}

		if (ioctl(usock, SIOCGIFHWADDR, ifrq) == -1)
		{
		    fprintf(stderr, "GET-MAC-IOCTL-ERR: %s (errno=%d)\n", strerror(errno), errno);
		    close(usock);
	   	    return CNET_ERR_IOCTL;
	    }

	    char *hwaddr = ifrq->ifr_ifru.ifru_hwaddr.sa_data;
	    for (int i = 0; i < 6; i++) {route->mac[i]=(uint8_t)hwaddr[i];}

	    close(usock);
	    return CNET_SCFL;
    }

	void CNET_BUFFER_INIT_SET(struct cnet_buffer *cbuf, int ind, void *addr, int layer)
	{
		cbuf->iov[ind].iov_base = cbuf->buff[ind];
		cbuf->iov[ind].iov_len = cbuf->size[ind];
		cbuf->msg[ind].msg_hdr.msg_iov = &cbuf->iov[ind];
		cbuf->msg[ind].msg_hdr.msg_iovlen = 1;

		if (layer==2)
		{
	    	struct sockaddr_ll *scaddr = (struct sockaddr_ll *)addr;
    		cbuf->msg[ind].msg_hdr.msg_name = scaddr;
    		cbuf->msg[ind].msg_hdr.msg_namelen = sizeof(*scaddr);
		} else {
			struct sockaddr_in *scaddr = (struct sockaddr_in *)addr;
			cbuf->msg[ind].msg_hdr.msg_name = scaddr;
			cbuf->msg[ind].msg_hdr.msg_namelen = sizeof(*scaddr);
		}
	}

	void CNET_BUFFER_INIT_ALL(struct cnet_buffer *cbuf, int ind, void *addr, int layer)
	{
		for (int i = 0; i < ind; i++) {
			cbuf->iov[i].iov_base = cbuf->buff[i];
			cbuf->iov[i].iov_len = cbuf->size[i];
			cbuf->msg[i].msg_hdr.msg_iov = &cbuf->iov[i];
			cbuf->msg[i].msg_hdr.msg_iovlen = 1;

			if (layer==2)
			{
		   		struct sockaddr_ll *scaddr = (struct sockaddr_ll *)addr;
	    		cbuf->msg[i].msg_hdr.msg_name = scaddr;
	    		cbuf->msg[i].msg_hdr.msg_namelen = sizeof(*scaddr);
			} else {
				struct sockaddr_in *scaddr = (struct sockaddr_in *)addr;
				cbuf->msg[i].msg_hdr.msg_name = scaddr;
				cbuf->msg[i].msg_hdr.msg_namelen = sizeof(*scaddr);
			}
		}
	}

	int CNET_SOCK(struct cnet_route *route, int layer, uint8_t flag)
	{
		if (layer == CNET_LAYER_ETHER) {
    		int sockfd = socket(AF_PACKET, SOCK_RAW, 0);

    		if (sockfd < 0) {
    			fprintf(stderr, "SOCK-ERR-CREATE: %s (errno=%d)\n", strerror(errno), errno);
    			return -1;
    		}

			struct ifreq ifr;
			CNET_MEMSET(&ifr, sizeof(ifr));

    		strncpy(ifr.ifr_name, route->active, IFNAMSIZ - 1);

    		if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
    			fprintf(stderr, "SOCK-ERR-IOCTL: %s (errno=%d)\n", strerror(errno), errno);
    			close(sockfd);
    			return -1;
    		}

    		struct sockaddr_ll sll;
    		memset(&sll, 0, sizeof(sll));
    		sll.sll_family = AF_PACKET;
    		sll.sll_ifindex = ifr.ifr_ifindex;
    		sll.sll_protocol = 0;

    		if (bind(sockfd, (struct sockaddr *)&sll, sizeof(sll)) < 0) {
    			fprintf(stderr, "SOCK-ERR-BIND: %s (errno=%d)\n", strerror(errno), errno);
    			close(sockfd);
    			return -1;
    		}

    		int sbuf = SIZEBUFF();
    		if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sbuf, sizeof(sbuf)) < 0) {
    			fprintf(stderr, "SOCK-ERR-SETOPT: %s (errno=%d)\n", strerror(errno), errno);
	    		close(sockfd);
	    		return -1;
    		}

    		int bflg = 1;
    		if (setsockopt(sockfd, SOL_PACKET, PACKET_QDISC_BYPASS, &bflg, sizeof(bflg)) < 0) {
    			fprintf(stderr, "SOCK-ERR-SETOPT: %s (errno=%d)\n", strerror(errno), errno);
	    		close(sockfd);
	    		return -1;
    		}

    		return sockfd;

		} else if (layer == CNET_LAYER_IP) {
			int opt = 1;
			int sockfd = socket(AF_INET, SOCK_RAW, flag);

			if (sockfd < 0) {
				fprintf(stderr, "SOCK-ERR-CREATE: %s (errno=%d)\n", strerror(errno), errno);
				return -1;
			}

			if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt)) < 0) {
				fprintf(stderr, "SOCK-ERR-SETOPT: %s (errno=%d)\n", strerror(errno), errno);
				return -1;
			}

			return sockfd;

		} else {printf("CNET-SOCK-INVALID-LAYER\n");}

		return -1;
	}

// Functions for Windows platform;
#elif defined(CNET_PLATFORM_WINDOWS)
	#include <winerror.h>
	#include <ntddndis.h>

	void CNET_BUFFER_CLONE(struct cnet_buffer *cbuff, int cl1, int cl2)
    {
	    for (int i = 0; i < APX; i++)
	    {
		    cbuff->buff[cl1][i] = cbuff->buff[cl2][i];
	    }

	    cbuff->size[cl1] = cbuff->size[cl2];
    }

	cnet_errno_t CNET_CONFIGURE(struct cnet_route *route, pcap_t *socket)
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
				if (route->count >= CNET_IFMAX) {break;}
				strncpy(route->iface[route->count], d->name, IFNAMSIZ-1);

				if (a->addr && a->addr->sa_family == AF_INET)
				{
					struct sockaddr_in *addrhdr = (struct sockaddr_in *)a->addr;
					char *fip = inet_ntoa(addrhdr->sin_addr);

					if (addrhdr->sin_addr.s_addr != inet_addr("127.0.0.1")) {
						if (route->count >= CNET_IFMAX) {break;}
						strncpy(route->iface[route->count], d->name, IFNAMSIZ-1);
					}

					if (strcmp(fip, "127.0.0.1") != 0)
					{
						for (int i = 0; i < CNET_LOCMAX; i++) {
							if (i == 15) {
								route->local[route->local_count][i] = '\0';
								break;
							}

							route->local[route->local_count][i] = fip[i];
						}

						route->local_count++;
					}
				}

				route->count++;
			}
		}

		pcap_freealldevs(localdevs);
		if (route->local[0] == '\0' || route->iface[0][0] == '\0')
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
		route->ip.i32p = gater.dwForwardNextHop;

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
				if (arp->table[i].dwAddr == route->ip.i32p)
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

		if (cbuf->pos >= 64 || cbuf->pos >= ind) return;

		for (int i = cbuf->pos; i < ind; i++) {
			pkthdr.len = (uint32_t)cbuf->size[i];
			pkthdr.caplen = (uint32_t)cbuf->size[i];

			if (pcap_sendqueue_queue(cbuf->buffer, &pkthdr, cbuf->buff[i]) < 0) {
				continue;
			}

			cbuf->pos++;
		}
	}

	void CNET_BUFFER_INIT(struct cnet_buffer *cbuf)
	{
		CNET_MEMSET(cbuf, sizeof(*cbuf));

		size_t pool = APY * (APX + sizeof(struct pcap_pkthdr));
		cbuf->buffer = pcap_sendqueue_alloc((uint32_t)pool);
	}

	pcap_t* CNET_SOCK(struct cnet_route *route)
	{
		char errbuf[PCAP_ERRBUF_SIZE];

		pcap_t *socket = pcap_open_live((const char *)route->active, 65535, 1, 1, errbuf);
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

/*
No functions in the ESP32 platform
*/

#elif !defined(CNET_PLATFORM_ESP32)
    #error "cnet.c: platform error."
#endif

/*
Main functions
*/

cnet_errno_t CNET_PACKET_TCP_FAST_SET(union cnet_packet_fast_config *pfc, struct cnet_tcp *tcp, void *ip, uint8_t type)
{
	if (
		pfc->tcp.src > 0xFFFF ||
		pfc->tcp.dst > 0xFFFF ||
		pfc->tcp.doff > 0x0F ||
		pfc->tcp.win > 0xFFFF ||
		pfc->tcp.uptr > 0xFFFF
	) return CNET_OVERFLOW;

	tcp->src = CNET_BIG16(pfc->tcp.src);
	tcp->dst = CNET_BIG16(pfc->tcp.dst);
	tcp->seq = CNET_BIG32(pfc->tcp.seq);
	tcp->aseq = CNET_BIG32(pfc->tcp.aseq);
	tcp->res1 = CNET_TCP_RES1;
	tcp->res2 = CNET_TCP_RES2;
	tcp->doff = pfc->tcp.doff;
	tcp->flags = pfc->tcp.flags;
	tcp->win = CNET_BIG16(pfc->tcp.win);
	tcp->uptr = CNET_BIG16(pfc->tcp.uptr);

	tcp->sum = CNET_L4_CSUM(type, ip, tcp, sizeof(*tcp));

	return CNET_SCFL;
}

cnet_errno_t CNET_PACKET_UDP_FAST_SET(union cnet_packet_fast_config *pfc, struct cnet_udp *udp, void *ip, uint8_t type)
{
	if (
		pfc->udp.src > 0xFFFF ||
		pfc->udp.dst > 0xFFFF
	) return CNET_OVERFLOW;

	udp->src = CNET_BIG16(pfc->udp.src);
	udp->dst = CNET_BIG16(pfc->udp.dst);
	udp->len = CNET_BIG16(sizeof(*udp));

	udp->sum = CNET_L4_CSUM(type, ip, udp, sizeof(*udp));

	return CNET_SCFL;
}

cnet_errno_t CNET_PACKET_IP_FAST_SET(union cnet_packet_fast_config *pfc, struct cnet_ip *ip, uint8_t options)
{
	uint8_t ihl = (options == CNET_IP_OPTIONS) ? 60 : 20;

	if (
		pfc->ip.type > 0xFF ||
		pfc->ip.id > 0xFFFF ||
		pfc->ip.frag > 0xFFFF ||
		pfc->ip.ttl > 0xFF ||
		pfc->ip.proto > 0xFF
	) return CNET_OVERFLOW;

	CNET_MEMSET(ip, sizeof(*ip));
	struct cnet_ip_bytes bsrc, bdst;
	CNET_IP_BYTES(&bsrc, pfc->ip.src);
	CNET_IP_BYTES(&bdst, pfc->ip.dst);

	size_t reserve = 0;

	switch (pfc->ip.proto) {
		case CNET_IP_TCP:
		  reserve = sizeof(struct cnet_tcp);
		  break;
		case CNET_IP_UDP:
		  reserve = sizeof(struct cnet_udp);
		  break;
		case CNET_IP_ICMP:
		  reserve = sizeof(struct cnet_icmp);
		  break;
		case CNET_IP_IGMP:
		  reserve = sizeof(struct cnet_igmp);
		  break;

		default:return CNET_UNSUPPORT_IP_PROTOCOL;
	}

	ip->ihl = IP_IHL_DECODE(ihl);
	ip->version = CNET_IP_VERSION;
	ip->type = CNET_IP_TYPE;
	ip->len = CNET_BIG16(ihl+reserve);
	ip->id = CNET_BIG16(pfc->ip.id);
	ip->frag = CNET_BIG16(pfc->ip.frag);
	ip->ttl = pfc->ip.ttl;
	ip->proto = pfc->ip.proto;

	ip->CNET_IP_MACROS_SRC32 = bsrc.bytes.b32its;
	ip->CNET_IP_MACROS_DST32 = bdst.bytes.b32its;

	ip->sum = CNET_L3_CSUM(ip, sizeof(*ip));

	return CNET_SCFL;
}

cnet_errno_t CNET_PACKET_ETHER_FAST_SET(union cnet_packet_fast_config *pfc, struct cnet_ether *ether)
{
	if (pfc->eth.next > 0xFFFF) return CNET_OVERFLOW;

	CNET_MEMSET(ether, sizeof(struct cnet_ether));
	struct cnet_ether_bytes bsrc, bdst;
	CNET_MAC_BYTES(&bsrc, pfc->eth.src);
	CNET_MAC_BYTES(&bdst, pfc->eth.dst);

	CNET_SET_SMAC(ether, &bsrc);
	CNET_SET_DMAC(ether, &bdst);
	ether->next = CNET_BIG16(pfc->eth.next);

	return CNET_SCFL;
}

void CNET_PACKET_IP_FAST_SET_TIMESTAMP(struct cnet_ip_timestamp *timestamp, uint8_t custom, char *ip)
{
	if (custom == CNET_IP_NCUSTOM)
	{
		timestamp->len = 0x28;
		timestamp->ptr = 0x05;
		timestamp->flow = 0x00;
		timestamp->flags = 0x00;
	}

	if ((uint8_t)timestamp->flags == 0x00)
	{
		timestamp->data[timestamp->ptr - 0x05] = CNET_BIG32(CNET_GET_IP_TIMESTAMP());
		timestamp->ptr++;
	} else if ((uint8_t)timestamp->flags == 0x01)
	{
		struct cnet_ip_bytes src;
		CNET_IP_BYTES(&src, ip);

		timestamp->data[timestamp->ptr - 0x05] = src.bytes.b32its;
		timestamp->ptr++;
		timestamp->data[timestamp->ptr - 0x05] = CNET_BIG32(CNET_GET_IP_TIMESTAMP());
		timestamp->ptr++;
	} else if ((uint8_t)timestamp->flags == 0x03)
	{
		struct cnet_ip_bytes src;
		CNET_IP_BYTES(&src, ip);

		timestamp->data[timestamp->ptr - 0x05] = src.bytes.b32its;
		timestamp->ptr++;
	} else {printf("CNET_PACKET_IP_FAST_SET_TIMESTAMP: Warning, Unreadable Flag (%d)\n", (int)timestamp->flags);}
}

void CNET_PACKET_TCP_FAST_SET_INFO(struct cnet_tcp_info *info, uint8_t flag)
{
	if (flag == CNET_TCP_INFO_LOCAL) return;

	info->cwnd = CNET_BIG32(info->cwnd);
	info->rtt = CNET_BIG32(info->rtt);
	info->rttv = CNET_BIG32(info->rttv);
	info->smss = CNET_BIG32(info->smss);
	info->unack = CNET_BIG32(info->unack);
	info->lost = CNET_BIG32(info->lost);
	info->rto = CNET_BIG32(info->rto);
	info->ato = CNET_BIG32(info->ato);
	info->rmss = CNET_BIG32(info->rmss);
	info->pmtu = CNET_BIG32(info->pmtu);
	info->sack = CNET_BIG32(info->sack);
	info->ret = CNET_BIG32(info->ret);
	info->ackets = CNET_BIG32(info->ackets);
	info->las = CNET_BIG32(info->las);
	info->lds = CNET_BIG32(info->lds);
	info->lar = CNET_BIG32(info->lar);
	info->ldr = CNET_BIG32(info->ldr);
	info->reorder = CNET_BIG32(info->reorder);
	info->rspace = CNET_BIG32(info->rspace);
	info->tret = CNET_BIG32(info->tret);
	info->advmss = CNET_BIG32(info->advmss);
	info->sth = CNET_BIG32(info->sth);
	info->rrtt = CNET_BIG32(info->rrtt);
	info->rth = CNET_BIG32(info->rth);
}

uint32_t CNET_GET_IP_TIMESTAMP()
{
	struct timespec ts;
	CNET_MEMSET(&ts, sizeof(ts));

	clock_gettime(CLOCK_REALTIME, &ts);

	uint64_t ms = ((uint64_t)ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
	uint64_t day_ms = 86400000;

	return (uint32_t)(ms % day_ms);
}

int CNET_RAND(int min, int max)
{
	static _Thread_local uint32_t xorstate = 0;

	if (xorstate == 0) {
		struct timespec ts;
	    clock_gettime(CLOCK_MONOTONIC, &ts);
		cnet_xorrand((uint32_t)(ts.tv_sec ^ ts.tv_nsec), &xorstate);
	}

	uint32_t x = xorstate;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	xorstate = x;

	return min + (x % (max - min + 1));
}

void CNET_SCTP_CSUM(void *sctp, size_t len)
{
	struct cnet_sctp *nsctp = (struct cnet_sctp *)sctp;
	nsctp->sum = 0x00000000;

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
	nsctp->sum = csum;
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

void CNET_EXPAND(struct cnet_expand_info *info)
{
	if (info->buffer == NULL || info->main == NULL || info->main_count == 0 || info->buffer_size == 0) {
		printf("CNET_EXPAND - ERROR, pointer is NULL or size equal to 0x00\n");
		return;
	}

	size_t offset = 0;
	uint8_t *casted_main = (uint8_t *)info->main;

	for (int i = 0; i < info->main_count; i++) {
		if (i + offset >= info->buffer_size) {
			printf("CNET_EXPAND - ERROR, buffer overflow in main\n");
			return;
		}

		info->buffer[i + offset] = casted_main[i];
	}
	offset += info->main_count;

	if (info->count == 0) {return;}

	uint8_t **casted_options = (uint8_t **)info->options;

	for (int c = 0; c < info->count; c++) {
		uint8_t *options = casted_options[c];

		if (options == NULL) continue;

		for (int i = 0; i < info->options_size[c]; i++) {
			if (i + offset >= info->buffer_size) {
			    printf("CNET_EXPAND - ERROR, buffer overflow in options\n");
			    return;
		    }

			info->buffer[i + offset] = options[i];
		}
		offset += info->options_size[c];
	}
}

uint16_t CNET_L4_CSUM(uint16_t type, void *l3, void *l4, size_t len)
{
	uint32_t sum = 0x00000000;
	size_t copy_len = len;

	uint16_t *bytes = (uint16_t *)l4;

	while (copy_len > 1) {
		sum += *bytes++;
		copy_len -= 2;
	}

	if (type == CNET_ETH_IP) {
		struct cnet_ip *ip = (struct cnet_ip *)l3;

		sum += CNET_BIG16((uint16_t)(ip->proto));

		uint32_t temp_src = ip->src.s32rc;
		uint16_t *src_sum = (uint16_t *)&temp_src;
		sum += src_sum[0] + src_sum[1];

		uint32_t temp_dst = ip->dst.d32st;
		uint16_t *dst_sum = (uint16_t *)&temp_dst;
		sum += dst_sum[0] + dst_sum[1];

		sum += CNET_BIG16((uint16_t)len);

	} else if (type == CNET_ETH_IPV6) {
		struct cnet_ipv6 *ipv6 = (struct cnet_ipv6 *)l3;

		uint16_t *src6_sum = (uint16_t *)ipv6->src;
		for (int i = 0; i < 8; i++) {
			sum += src6_sum[i];
		}

		uint16_t *dst6_sum = (uint16_t *)ipv6->dst;
		for (int i = 0; i < 8; i++) {
			sum += dst6_sum[i];
		}

		sum += CNET_BIG16((uint16_t)(ipv6->proto));

		uint16_t len16[2] = {
			CNET_BIG16((uint16_t)((len >> 16) & 0xFFFF)),
			CNET_BIG16((uint16_t)(len & 0xFFFF))
		};
		sum += len16[0] + len16[1];

	} else {return 0x0000;}

	if (copy_len) {
		uint8_t temp[2] = {*(uint8_t *)bytes, 0x00};
		sum += *(uint16_t *)temp;
	}

	while (sum >> 16) {
		sum = (sum & 0xFFFF)+(sum >> 16);
	}

	return (uint16_t)(sum ^ 0xFFFF);
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

void CNET_FRAME_FCS(struct cnet_frame_fcs *ffcs, void *data, size_t len)
{
	ffcs->fcs.fcs32 = 0x0000;
	uint8_t *bytes = (uint8_t *)data;
	uint32_t crc = 0xFFFFFFFF;

	for (size_t i = 0; i < len; i++)
	{
		for (int bit = 0; bit < 8; bit++)
		{
			uint32_t ent = (crc ^ (bytes[i] >> bit)) & 1;
			crc >>= 1;
			if (ent)
			{
				crc ^= 0xEDB88320;
			}
		}
	}

	ffcs->CNET_FRAME_MACROS_FCS32 = crc ^ 0xFFFFFFFF;
}

/*


------------------------------|
      Library - CNET          |
------------------------------|
Version: VERSION.md           |
Install: INSTALL.md ->        |
-> installers -> .sh, .bat    |
License: LICENSE -> (MIT)     |
Author: ShineXmRedT14 (Shine) |
------------------------------|


*/