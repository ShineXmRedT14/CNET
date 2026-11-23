# CNET documentation

CNET is a header-first C library for building and transmitting raw network
packets. It provides wire-format `struct` definitions, byte-order and checksum
helpers, a small packet-builder layer, and per-platform send backends for
Linux, Windows (Npcap) and ESP32.

This documentation describes the repository as it is now:

```text
CNET/
├── headers/            public headers (all protocols + platform API)
│   └── cnet_frame/     802.11 frame definitions (22 headers)
├── src/
│   ├── cnet.c          portable core + Linux/Windows backends
│   └── cnet.cpp        ESP32 backend (lwIP / esp_wifi)
├── installers/
│   ├── cnet_linux.sh   Linux installer
│   ├── cnet_win.bat    Windows installer
│   └── cnet_esp32.md   ESP32 packaging notes
├── extension/          Arduino library packaging
├── docs/               this documentation
├── README.md
├── INSTALL.md
├── VERSION.md
└── LICENSE
```

## Documentation map

| Page | Contents |
|---|---|
| [getting-started.md](getting-started.md) | Requirements, installation, first working program |
| [platforms.md](platforms.md) | Linux, Windows and ESP32 backends in detail |
| [api.md](api.md) | Every public function and macro-helper |
| [structures.md](structures.md) | Every `struct` and `enum` |
| [defines.md](defines.md) | Every public `#define`, grouped by header |
| [protocols.md](protocols.md) | Wire layouts and usage of each protocol header |
| [examples.md](examples.md) | Complete worked examples |
| [caveats.md](caveats.md) | Platform limits and known issues |

## Design in one minute

CNET does three things:

1. **Describe packets.** Each protocol header is a `__attribute__((packed))`
   struct whose fields sit exactly where the protocol expects them on the
   wire (`cnet_ip`, `cnet_tcp`, `cnet_udp`, `cnet_icmp`, `cnet_sctp`, …).
2. **Fill them correctly.** Helpers convert byte order, compute checksums,
   parse address strings, build stacked layers in one buffer, and set whole
   headers in one call (`CNET_PACKET_*_FAST_SET`).
3. **Send them.** A thin backend per platform opens the socket or handle and
   transmits one packet or a batch (`CNET_SOCK`, `CNET_BURST`,
   `CNET_BUFFER_*`).

The same protocol structs are available on every platform. Only the socket /
transmit section of the API differs.

## Conventions used in this documentation

- **Layer 2** means a full Ethernet frame (`AF_PACKET` on Linux, Npcap on
  Windows, raw 802.11 on ESP32).
- **Layer 3** means an IP packet without an Ethernet header
  (`AF_INET` + `IP_HDRINCL` on Linux).
- All multi-byte fields are stored in **network byte order** on the wire.
  Use `CNET_BIG16()` / `CNET_BIG32()` when assigning them manually; the
  `CNET_PACKET_*_FAST_SET()` family already converts.
- Bitfields whose order depends on endianness are guarded in the headers.
  Access them through the `CNET_*_MACROS_*` aliases where provided.
- Code samples assume `headers/` is on the include path and that
  `src/cnet.c` (plus `src/cnet.cpp` on ESP32) is compiled into the program.

## Version

See [VERSION.md](../VERSION.md).

## License

MIT, see [LICENSE](../LICENSE).
