#-----------------------------------------------------------------------------
# WDB kvalobsLoad Component
#-----------------------------------------------------------------------------

#EXTRA_PROGRAMS +=			kvalobsLoad

if KVALOBS_LOAD
pkglib_PROGRAMS += kvalobsLoad
endif

kvalobsLoad_SOURCES =	src/loadingProgram/kvalobsLoad/main.cpp \
						src/loadingProgram/kvalobsLoad/KvalobsListener.cpp \
						src/loadingProgram/kvalobsLoad/KvalobsListener.h \
						src/loadingProgram/kvalobsLoad/IncomingDataHandler.cpp \
						src/loadingProgram/kvalobsLoad/IncomingDataHandler.h \
						src/loadingProgram/kvalobsLoad/InsertDataTransaction.cpp \
						src/loadingProgram/kvalobsLoad/InsertDataTransaction.h \
						src/loadingProgram/kvalobsLoad/WdbCache.cpp \
						src/loadingProgram/kvalobsLoad/WdbCache.h \
						src/loadingProgram/kvalobsLoad/transactionType.h \
						src/loadingProgram/kvalobsLoad/KvalobsLoadConfiguration.cpp \
						src/loadingProgram/kvalobsLoad/KvalobsLoadConfiguration.h

kvalobsLoad_CPPFLAGS = $(kvalobs_CFLAGS)

kvalobsLoad_LDADD =	-lwdbConfiguration \
					-lwdbException \
					-lwdbLogHandler \
					-lboost_thread

kvalobsLoad_LDFLAGS = $(kvalobs_LIBS)

EXTRA_DIST +=	src/loadingProgram/kvalobsLoad/wdb.mk \
				src/loadingProgram/kvalobsLoad/Makefile.am \
				src/loadingProgram/kvalobsLoad/Makefile.in

DISTCLEANFILES +=		src/loadingProgram/kvalobsLoad/Makefile


# Local Makefile Targets
#-----------------------------------------------------------------------------

src/loadingProgram/kvalobsLoad/all: kvalobsLoad

src/loadingProgram/kvalobsLoad/clean: clean
