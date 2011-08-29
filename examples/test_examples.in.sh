#!/bin/sh

set -f

SQL_EXAMPLES="__WDB_SRCDIR__/examples/sql/exact.sql __WDB_SRCDIR__/examples/sql/getAll.sql __WDB_SRCDIR__/examples/sql/parameter.sql"
CPP_EXAMPLES="exampleGetPoint exampleGetField"

examples/setupExamples.sh

for EXAMPLE in $SQL_EXAMPLES; do
	psql `./wdbConfiguration --psqlArgs` < $EXAMPLE >> /dev/null
done

for EXAMPLE in $CPP_EXAMPLES; do
	./$EXAMPLE `./wdbConfiguration --pqxxArgs` >> /dev/null
done

psql `./wdbConfiguration --psqlArgs` -c "SELECT cleanupdb()" >> /dev/null