# Makefile
# Usage:
# make			# Compile binaries
# make clean    # Remove all binaries and objects

CC=g++


all: libpistache_addons.so

libpistache_addons.so: request_body.o string_util.o
	$(CC) $^ -shared -o $@

request_body.o: src/request_body.cpp src/request_body.hpp src/string_util.hpp
	$(CC) -c $< -fPIC -o $@

string_util.o: src/string_util.cpp src/string_util.hpp
	$(CC) -c $< -fPIC -o $@

clean:
	@rm -rvf libpistache_addons.so request_body.o string_util.o

install: libpistache_addons.so
	mkdir /usr/local/include/pistache_addons/
	cp src/*.hpp /usr/local/include/pistache_addons/
	install libpistache_addons.so /usr/local/lib64/libpistache_addons.so

uninstall: /usr/local/include/pistache_addons/ /usr/local/lib64/libpistache_addons.so
	@rm -rvf /usr/local/include/pistache_addons/ /usr/local/lib64/libpistache_addons.so
