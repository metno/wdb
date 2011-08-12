datatransferincludedir = $(pkgincludedir)/data_transfer

datatransferinclude_HEADERS = \
	src/common/data_transfer/convert_to_host_order.h \
	src/common/data_transfer/convert_to_network_order.h

EXTRA_DIST +=				src/common/data_transfer/wdb.mk \
							src/common/data_transfer/Makefile.am \
							src/common/data_transfer/Makefile.in

DISTCLEANFILES +=			src/common/data_transfer/Makefile
	