#!/bin/bash

if command -v gcc >/dev/null 2>&1; then
    N_DIR=$(find / -path */CNET/installer 2>/dev/null)

    if [ -z "$N_DIR" ]; then
        echo "Not found Path CNET/installer -> error/exit"
        exit 1
    fi

    echo "CNET-install-script -> started ($N_DIR/cnetsh.sh)"
    cd "$N_DIR"
    cd ..
    cd ./src
    gcc -shared -fPIC -O2 -DNDEBUG cnet.c -o /usr/local/lib/cnet.so
    echo "Compiled cnet.c -> cnet.so and moved into /usr/local/lib/"
    cd ..
    cd ./headers
    mv ./* /usr/local/include/
    LD_PATHBIN=$(find / -name ldconfig 2>/dev/null)
    LD_CONFBIN="/etc/ld.so.conf.d/cnet.conf"

    if [ -f "$LD_CONFBIN" ]; then
        echo "ldconfig-conf -> found ($N_DIR/cnetsh.sh)"
    else
        echo "ldconfig-conf -> not found -> start create ($N_DIR/cnetsh.sh)"
        echo "/usr/local/lib" | tee "$LD_CONFBIN" 2>/dev/null
    fi

    if [ -f "$LD_PATHBIN" ]; then
        echo "ldconfig -> successful ($N_DIR/cnetsh.sh)"
        "$LD_PATHBIN"
    else
        echo "ldconfig -> error ($N_DIR/cnetsh.sh)"
    fi
fi