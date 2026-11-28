file tcp.c
compiling: gcc test.c -o test -L/usr/local/lib -lcnet -l:cnet.so -Wl,-rpath,/usr/local/lib

IP checksum: good (correct)
TCP checksum: good (correct)