## hotkeyd
##
## Forked from Benjamin Bolton's keyd
## https://bennybolton.com
## license: GPLv3

CC="gcc"
VERSION="0.4.1"

all: clean hotkeyd

hotkeyd:
	$(CC) hotkeyd.c -o hotkeyd

clean:
	rm -f hotkeyd

install: all
	mkdir /usr/bin -p
	mkdir /etc/rc.d -p
	mkdir /usr/lib/systemd/system -p
	cp hotkeyd         /usr/bin/
	cp hotkeyd.daemon  /etc/rc.d/hotkeyd
#	cp hotkeyd.conf    /etc/
	cp hotkeyd.service /usr/lib/systemd/system/

uninstall:
	rm -f /usr/bin/hotkeyd
	rm -f /etc/rc.d/hotkeyd
#	rm -f /etc/hotkeyd.conf
	rm -f /usr/lib/systemd/system/keyd.service

recompile: clean all

