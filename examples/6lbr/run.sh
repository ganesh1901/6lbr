#!/bin/sh
DESTDIR=''
install -d $(DESTDIR)/etc/6lbr
cp -r package/etc/6lbr $(DESTDIR)/etc
install -d $(DESTDIR)/etc/init.d
install package/etc/init.d/* $(DESTDIR)/etc/init.d
install -d $(DESTDIR)/usr/bin
install package/usr/bin/* $(DESTDIR)/usr/bin
install -d $(DESTDIR)/usr/lib/6lbr
cp -r package/usr/lib/6lbr $(DESTDIR)/usr/lib
install -d $(DESTDIR)/usr/lib/6lbr/bin
install bin/* $(DESTDIR)/usr/lib/6lbr/bin
install tools/nvm_tool $(DESTDIR)/usr/lib/6lbr/bin
