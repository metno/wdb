#include <postgres.h>
#include <fmgr.h>

// To ensure that a dynamically loaded object file is not loaded into an 
// incompatible server, PostgreSQL checks that the file contains a "magic 
// block" with the appropriate contents. This allows the server to detect 
// obvious incompatibilities, such as code compiled for a different major 
// version of PostgreSQL. 
#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif
