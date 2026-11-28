#include <cnet.h>

#define IPC "127.0.0.1"
#define MACC "00:00:00:00:00:00"
#define SPORT 12345
#define DPORT 54321

int main() {
    struct cnet_route route;
    CNET_MEMSET(&route, sizeof(route));
    if (CNET_CONFIGURE(&route) != CNET_SCFL) {
        return 1;
    }

    route.active[0] = 'l';
    route.active[1] = 'o';
    route.active[2] = '\0';
    
    int fd = CNET_SOCK(&route, CNET_LAYER_ETHER, 0x00);
    if (fd < 0 ) {
        return 1;
    }

    struct cnet_buffer buff;
    CNET_MEMSET(&buff, sizeof(buff));

    int offset = 0;
    uint8_t *pkt = CNET_PACKET_UPTR(&buff, 0);

    struct cnet_ether *eth = CNET_EXTRA_LAYER(struct cnet_ether, pkt, &offset);
    struct cnet_ip *ip = CNET_EXTRA_LAYER(struct cnet_ip, pkt, &offset);
    struct cnet_tcp *tcp = CNET_EXTRA_LAYER(struct cnet_tcp, pkt, &offset);

    union cnet_packet_fast_config fcnf;

    CNET_MEMSET(&fcnf, sizeof(fcnf));
    // Ethernet
    fcnf.eth.next = (int)CNET_ETH_IP;
    CNET_HELPER_SET_ETH_SRC(&fcnf, MACC);
    CNET_HELPER_SET_ETH_DST(&fcnf, MACC);

    if (CNET_PACKET_ETHER_FAST_SET(&fcnf, eth) != CNET_SCFL) {
        return 1;
    }

    CNET_MEMSET(&fcnf, sizeof(fcnf));
    // IPv4
    fcnf.ip.type = (int)CNET_IP_TYPE;
    fcnf.ip.ttl = (int)CNET_IP_TTL;
    fcnf.ip.frag = 0;
    fcnf.ip.id = 1234;
    fcnf.ip.proto = (int)CNET_IP_TCP;

    CNET_HELPER_SET_IP_SRC(&fcnf, IPC);
    CNET_HELPER_SET_IP_DST(&fcnf, IPC);

    if (CNET_PACKET_IP_FAST_SET(&fcnf, ip, CNET_IP_NOPTIONS) != CNET_SCFL) {
        return 1;
    }

    CNET_MEMSET(&fcnf, sizeof(fcnf));
    // TCP
    fcnf.tcp.src = SPORT;
    fcnf.tcp.dst = DPORT;
    fcnf.tcp.seq = (uint32_t)12345;
    fcnf.tcp.aseq = 0x00000000;
    fcnf.tcp.doff = (int)CNET_TCP_DOFF;
    fcnf.tcp.flags = CNET_TCP_SYN;
    fcnf.tcp.win = (int)CNET_TCP_WIN;
    fcnf.tcp.uptr = 0;

    if (CNET_PACKET_TCP_FAST_SET(&fcnf, tcp, ip, CNET_ETH_IP) != CNET_SCFL) {
        return 1;
    }

    CNET_MEMSET(&fcnf, sizeof(fcnf));

    CNET_SIZE_SET(&buff, 0, offset);

    for (int i = 1; i < APY; i++) {
        CNET_BUFFER_CLONE(&buff, i, 0);
    }

    struct sockaddr_ll addr = CNET_SOCK_ADDR_LL(&route, fd);
    CNET_BUFFER_INIT_ALL(&buff, APY, &addr, CNET_LAYER_ETHER);
    CNET_BURST(&buff, fd, APY);

    return 0;
}