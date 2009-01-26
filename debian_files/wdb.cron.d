SHELL=/bin/sh


15 4 * * * su postgres -c "psql -d`/usr/bin/wdbConfiguration --database` -p`/usr/bin/wdbConfiguration --port` -c\"SELECT cleanupdb()\""
