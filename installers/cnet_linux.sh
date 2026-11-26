#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
BASE_DIR="$(cd -- "$SCRIPT_DIR/.." && pwd)"

HEADERS_DIR="$BASE_DIR/headers"
SRC_FILE="$BASE_DIR/src/cnet.c"

PREFIX="${PREFIX:-/usr/local}"
INC_DIR="$PREFIX/include"
LIB_DIR="$PREFIX/lib"

LDCONF_DIR="/etc/ld.so.conf.d"
LDCONF_FILE="$LDCONF_DIR/cnet.conf"

die() {
    echo "CNET-install: $*" >&2
    exit 1
}

echo "CNET-install: repository = $BASE_DIR"

[ -d "$HEADERS_DIR" ] || die "headers directory not found: $HEADERS_DIR"
[ -f "$HEADERS_DIR/cnet.h" ] || die "cnet.h not found in $HEADERS_DIR"
[ -f "$SRC_FILE" ] || die "source not found: $SRC_FILE"

command -v gcc >/dev/null 2>&1 || \
    die "GCC not found. Install it first (for example: sudo apt install build-essential)."

mkdir -p "$INC_DIR" "$LIB_DIR" 2>/dev/null || true

if [ ! -w "$INC_DIR" ] || [ ! -w "$LIB_DIR" ]; then
    die "no write access to $PREFIX. Re-run with sudo: sudo $0"
fi

cp -r "$HEADERS_DIR/." "$INC_DIR/"
echo "CNET-install: headers installed into $INC_DIR"

gcc -shared -fPIC -O2 -DNDEBUG -I"$HEADERS_DIR" "$SRC_FILE" -o "$LIB_DIR/libcnet.so"

ln -sf libcnet.so "$LIB_DIR/cnet.so"

echo "CNET-install: library installed into $LIB_DIR/libcnet.so"

if [ -d "$LDCONF_DIR" ] && [ -w "$LDCONF_DIR" ]; then
    printf '%s\n' "$LIB_DIR" > "$LDCONF_FILE"
elif [ "$LIB_DIR" != "/usr/local/lib" ]; then
    echo "CNET-install: warning: cannot write $LDCONF_FILE." >&2
    echo "CNET-install: add $LIB_DIR to the loader configuration manually." >&2
fi

LDCONFIG_BIN="$(command -v ldconfig 2>/dev/null || true)"
if [ -z "$LDCONFIG_BIN" ]; then
    for candidate in /sbin/ldconfig /usr/sbin/ldconfig /usr/local/sbin/ldconfig; do
        if [ -x "$candidate" ]; then
            LDCONFIG_BIN="$candidate"
            break
        fi
    done
fi

if [ -n "$LDCONFIG_BIN" ]; then
    if "$LDCONFIG_BIN" 2>/dev/null; then
        echo "CNET-install: ldconfig updated"
    else
        echo "CNET-install: warning: ldconfig failed (run as root to update the cache)." >&2
    fi
else
    echo "CNET-install: warning: ldconfig not found; add $LIB_DIR to the loader path manually." >&2
fi

echo "CNET-install: done"