SHELL=/bin/sh


36 2 * * * postgres psql -d`/usr/bin/wdbConfiguration --database` -p`/usr/bin/wdbConfiguration --port` -c"SELECT cleanupdb()"

