## Makefile for hotkeyd
##
## Forked from Benjamin Bolton's keyd
## license: GPLv3

CC         = gcc
CFLAGS     = -Wall -pedantic
CFLAGS_OPT = -O2
DESTDIR    = ""
VERSION    = "0.4.3"

all: clean hotkeyd

hotkeyd:
	$(CC) $(CFLAGS) $(CFLAGS_OPT) src/hotkeyd.c -o hotkeyd

clean:
	rm -f hotkeyd

install: all
	mkdir -p $(DESTDIR)/usr/bin/
	mkdir -p $(DESTDIR)/etc/rc.d/
	install -m755 -b -D hotkeyd $(DESTDIR)/usr/bin/
	cp hotkeyd.daemon $(DESTDIR)/etc/rc.d/hotkeyd
	yes n | cp -i hotkeyd.conf $(DESTDIR)/etc/

uninstall:
	rm -f $(DESTDIR)/usr/bin/hotkeyd
	rm -f $(DESTDIR)/etc/rc.d/hotkeyd
	# rm -f $(DESTDIR)/etc/hotkeyd.conf

recompile: clean all
