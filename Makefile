## Makefile for hotkeyd
##
## Forked from Benjamin Bolton's keyd
## license: GPLv3

CC="gcc"
DESTDIR=""
VERSION="0.4.1"

all: clean hotkeyd

hotkeyd:
	$(CC) src/hotkeyd.c -o hotkeyd

clean:
	rm -f hotkeyd

install: all
	mkdir $(DESTDIR)/usr/bin -p
	mkdir $(DESTDIR)/etc/rc.d -p
	mkdir $(DESTDIR)/usr/lib/systemd/system -p
	cp hotkeyd         $(DESTDIR)/usr/bin/
	cp hotkeyd.daemon  $(DESTDIR)/etc/rc.d/hotkeyd
	cp hotkeyd.conf    $(DESTDIR)/etc/
	cp hotkeyd.service $(DESTDIR)/usr/lib/systemd/system/

uninstall:
	rm -f $(DESTDIR)/usr/bin/keyd
	rm -f $(DESTDIR)/etc/rc.d/keyd
	rm -f $(DESTDIR)/etc/keyd.conf
	rm -f $(DESTDIR)/usr/lib/systemd/system/keyd.service

recompile: clean all

