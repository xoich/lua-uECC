CPPFLAGS = -Wall -Wextra

all: uECC.so

micro-ecc/libuECC.a: micro-ecc/
	gcc -c -fPIC micro-ecc/uECC.c -o "$@"

uECC.so: src/uECC.c micro-ecc/libuECC.a micro-ecc/uECC.h
	gcc -shared -fPIC $(CPPFLAGS) $(CFLAGS) -I micro-ecc/ -I /usr/include/lua5.3 $< micro-ecc/libuECC.a -o "$@"

check: uECC.so
	lua test.lua

clean:
	rm -f uECC.so micro-ecc/libuECC.a
