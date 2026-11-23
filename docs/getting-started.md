# Getting started

## 1. Requirements

| Target | Needs |
|---|---|
| Linux | GCC/Clang, `make` optional, root (or `CAP_NET_RAW`) for raw sockets, Linux headers |
| Windows | MinGW-w64 GCC, Npcap runtime **with WinPcap API-compatible mode**, Npcap SDK (headers + `wpcap.lib` / `Packet.lib`), Administrator for installation |
| ESP32 | ESP-IDF or Arduino-ESP32, C and C++ compiler, lwIP headers (`tcp_priv.h`), `esp_wifi` |

The library has no external dependencies beyond the platform's socket /
capture API. On Windows the only external dependency is Npcap.

## 2. Installation

### Linux

From the repository root:

```bash
chmod +x installers/cnet_linux.sh
sudo ./installers/cnet_linux.sh
```

The script:

1. copies every header from `headers/` into `/usr/local/include`;
2. compiles `src/cnet.c` into `/usr/local/lib/libcnet.so`;
3. creates the compatibility symlink `/usr/local/lib/cnet.so`;
4. registers `/usr/local/lib` with `ldconfig`.

Override the prefix with `PREFIX=/some/prefix`. Compile against it with:

```bash
gcc program.c -I/usr/local/include -L/usr/local/lib -lcnet -o program
```

### Windows

1. Install [Npcap](https://npcap.com/) and enable **WinPcap API-compatible
   Mode** during setup.
2. Download and extract the Npcap SDK.
3. Open an **Administrator** terminal and run:

```bat
installers\cnet_win.bat
```

The script locates the SDK (or uses `NPCAP_SDK`), compiles
`src/cnet.c` into `%ProgramFiles%\CNET\lib\cnet.dll`, copies the headers to
`%ProgramFiles%\CNET\include`, and adds both the library directory and the
Npcap runtime directory to the system `PATH`.

Compile against it with MinGW-w64:

```bat
gcc program.c -I"%ProgramFiles%\CNET\include" ^
  -I"%NPCAP_SDK%\Include" ^
  "%ProgramFiles%\CNET\lib\cnet.dll" ^
  -o program.exe
```

> `cnet.h` contains `#pragma comment(lib, ...)` for MSVC. MinGW ignores it,
> so link `-liphlpapi -lws2_32` explicitly (the installer already does this
> when building the DLL).

### ESP32

There is no installer. Copy the repository into an Arduino library folder or
an ESP-IDF component as described in
[`extension/src/INSTALL.md`](../extension/src/INSTALL.md):

```text
libraries/CNET/
├── library.properties
├── READ.md
└── src/
    ├── cnet.h
    ├── cnet_*.h
    ├── cnet_frame/*.h
    ├── cnet.c
    └── cnet.cpp
```

In an Arduino `.ino` file, wrap includes in `extern "C"` and include
`cnet_init.h` first:

```cpp
extern "C" {
  #include <cnet_init.h>
  #include <cnet_frame/probe.h>
}
```

For ESP-IDF, add `headers/` to the include path and compile both
`src/cnet.c` and `src/cnet.cpp`.

## 3. Compiling without installing

Because CNET is a small source library, you can also compile it directly:

```bash
gcc my_program.c src/cnet.c -Iheaders -o my_program
```

On Windows add the Npcap include directory and import libraries:

```bat
gcc my_program.c src\cnet.c -Iheaders -I"%NPCAP_SDK%\Include" ^
  "%NPCAP_SDK%\Lib\x64\wpcap.lib" "%NPCAP_SDK%\Lib\x64\Packet.lib" ^
  -liphlpapi -lws2_32 -o my_program.exe
```

## 4. The smallest useful program (Linux, Layer 3)

```c
#include <cnet.h>

int main(void)
{
    /* 1. Find the active interface: fills route.active, route.ip, route.mac */
    struct cnet_route route;
    CNET_MEMSET(&route, sizeof(route));
    if (CNET_CONFIGURE(&route) != CNET_SCFL)
        return 1;

    /* 2. Open a raw IPv4 socket for TCP */
    int fd = CNET_SOCK(&route, CNET_LAYER_IP, CNET_IP_TCP);
    if (fd < 0)
        return 1;

    /* 3. Build IP + TCP in one buffer */
    struct cnet_buffer buf;
    CNET_MEMSET(&buf, sizeof(buf));

    int offset = 0;
    uint8_t *pkt = CNET_PACKET_UPTR(&buf, 0);

    struct cnet_ip  *ip  = CNET_EXTRA_LAYER(struct cnet_ip,  pkt, &offset);
    struct cnet_tcp *tcp = CNET_EXTRA_LAYER(struct cnet_tcp, pkt, &offset);

    ip->version = CNET_IP_VERSION;
    ip->ihl     = IP_IHL_DECODE(sizeof(*ip));
    ip->type    = CNET_IP_TYPE;
    ip->len     = CNET_BIG16(sizeof(*ip) + sizeof(*tcp));
    ip->ttl     = CNET_IP_TTL;
    ip->proto   = CNET_IP_TCP;
    ip->src.CNET_IP_MACROS_SRC32 = route.ip.i32p;
    ip->dst.CNET_IP_MACROS_DST32 = inet_addr("192.168.1.1");
    ip->sum = CNET_L3_CSUM(ip, sizeof(*ip));

    tcp->src   = CNET_BIG16(40000);
    tcp->dst   = CNET_BIG16(CNET_PORT_HTTP);
    tcp->seq   = CNET_BIG32(1);
    tcp->doff  = CNET_TCP_DOFF;
    tcp->flags = CNET_TCP_SYN;
    tcp->win   = CNET_BIG16(CNET_TCP_WIN);
    tcp->sum   = CNET_L4_CSUM(CNET_ETH_IP, ip, tcp, sizeof(*tcp));

    CNET_SIZE_SET(&buf, 0, offset);

    /* 4. Send */
    struct sockaddr_in dst = CNET_SOCK_ADDR_IN(ip);
    sendto(fd, pkt, offset, 0, (struct sockaddr *)&dst, sizeof(dst));

    return 0;
}
```

Notes:

- `CNET_PACKET_UPTR()` returns a zeroed slot. On Linux/Windows `buf.buff` is
  a two-dimensional array; the second argument selects the slot. On ESP32
  there is a single packet, so the argument is omitted.
- `CNET_EXTRA_LAYER` advances `offset` by `sizeof(struct)`; it returns `NULL`
  and prints an error if either pointer is `NULL`.
- `CNET_SIZE_SET` records the packet length for buffered/burst sends.
- Checksums must be computed **after** the fields they cover are filled.

## 5. Where to go next

- [api.md](api.md) — pick the helper you need.
- [protocols.md](protocols.md) — exact field layout of every protocol.
- [examples.md](examples.md) — full Ethernet, UDP, ICMP, fast-path and ESP32 examples.
- [platforms.md](platforms.md) — why `CNET_SOCK` differs on each platform.
