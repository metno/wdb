.xml.1:
	$(DOCBOOK) man $< -o `dirname $@`

.xml.5:
	$(DOCBOOK) man $< -o `dirname $@`


MAN_DOCS = \
	doc/man/wdb.xml \
	doc/man/wdbConfiguration.xml

man1_MANS = $(MAN_DOCS:.xml=.1)

man5_MANS = doc/man/wdb.conf.5

EXTRA_DIST += $(MAN_DOCS) doc/man/wdb.conf.xml
CLEANFILES += $(MAN_DOCS:.xml=.1) doc/man/wdb.conf.5
