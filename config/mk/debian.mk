
#-----------------------------------------------------------------------------
# Common Debian Package
#-----------------------------------------------------------------------------

PKG_DIR = $(PACKAGE)-$(VERSION)
DEBIAN_DIR = $(PKG_DIR)/debian
DEBIAN_PACKAGE = `head -n1 $(top_srcdir)/NEWS | sed "s/ (/_/" | sed "s/\(.*\)-.*/\1/"`
ARCH = `dpkg-architecture -qDEB_HOST_ARCH_CPU`
DEBIAN_PACKAGE_NAME_BASE = `head -n1 $(top_srcdir)/NEWS | sed "s/ (/_/" | sed "s/).*//"`
DEBIAN_PACKAGE_NAME = $(DEBIAN_PACKAGE_NAME_BASE)_$(ARCH).deb
DEBIAN_SOURCE_PACKAGE_NAME = $(DEBIAN_PACKAGE_NAME_BASE).dsc

dist-debian: dist clean-debian
	tar xvzf $(PKG_DIR).tar.gz
	cp $(PKG_DIR).tar.gz $(DEBIAN_PACKAGE).orig.tar.gz

prepare-debian:
	rm -rf $(DEBIAN_DIR)
	mkdir -p $(DEBIAN_DIR)
	cp -r $(top_srcdir)/debian_files/* $(DEBIAN_DIR)
	cp $(top_srcdir)/NEWS $(DEBIAN_DIR)/changelog
	chmod 774 $(DEBIAN_DIR)/rules
	if [ -e $(DEBIAN_DIR)/templates ]; then  debconf-updatepo --podir=$(DEBIAN_DIR)/po; fi

update-debian: prepare-debian
	cd $(PKG_DIR) && dpkg-buildpackage -rfakeroot -us -uc -nc
	lintian $(DEBIAN_PACKAGE_NAME) $(DEBIAN_SOURCE_PACKAGE_NAME)

build-debian:
	cd $(PKG_DIR) && dpkg-buildpackage -rfakeroot -us -uc -sa -i.svn
	lintian $(DEBIAN_PACKAGE_NAME) $(DEBIAN_SOURCE_PACKAGE_NAME)

common-debian: dist-debian prepare-debian build-debian

clean-debian:
	debclean
	rm -rf $(PKG_DIR) $(DEBIAN_PACKAGE)*
