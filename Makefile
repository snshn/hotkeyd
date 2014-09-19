## keyd
##
## developed by benjamin bolton
## https://bennybolton.com/
## license: GPLv3

CC="gcc"
DESTDIR=""
VERSION="0.4.0"

all: keyd

keyd:
	$(CC) keyd.c -o keyd

run: keyd
	./keyd

clean:
	rm -f keyd

install: all
	mkdir $(DESTDIR)/usr/bin -p
	mkdir $(DESTDIR)/etc/rc.d -p
	mkdir $(DESTDIR)/usr/lib/systemd/system -p
	cp keyd $(DESTDIR)/usr/bin/
	cp keyd.daemon $(DESTDIR)/etc/rc.d/keyd
	cp keyd.conf $(DESTDIR)/etc/
	cp keyd.service $(DESTDIR)/usr/lib/systemd/system/

uninstall:
	rm -f $(DESTDIR)/usr/bin/keyd
	rm -f $(DESTDIR)/etc/rc.d/keyd
	rm -f $(DESTDIR)/etc/keyd.conf
	rm -f $(DESTDIR)/usr/lib/systemd/system/keyd.service

package:
	rm -fR "keyd-$(VERSION)"
	mkdir "keyd-$(VERSION)"
	cp keyd.c keys.h log.h input-dev.h Makefile keyd.conf keyd.daemon keyd.service README "keyd-$(VERSION)"
	tar -czf "keyd-$(VERSION).tar.gz" "keyd-$(VERSION)"
	md5sum "keyd-$(VERSION).tar.gz" > md5sum
	rm -fR tarball
	mkdir tarball
	sed "s|pkgver=.*|pkgver=$(VERSION)|;s|md5sums=.*|md5sums=('`md5sum keyd-$(VERSION).tar.gz | awk '{print $$1;}'`')|" PKGBUILD.template > tarball/PKGBUILD
	cp "keyd-$(VERSION).tar.gz" tarball
	cd tarball; makepkg --allsource
	echo -e "cd /srv/ftp/keyd\nmkdir $(VERSION)\ncd $(VERSION)\nput keyd-$(VERSION).tar.gz\nput md5sum\nput tarball/PKGBUILD\nexit" | sftp root@bennybolton.com

recompile: clean all
