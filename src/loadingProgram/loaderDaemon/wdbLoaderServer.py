#__PYTHON__
## - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## 
## wdb - weather and water data storage
##
## Copyright (C) 2007 met.no
##
##  Contact information:
##  Norwegian Meteorological Institute
##  Box 43 Blindern
##  0313 OSLO
##  NORWAY
##  E-mail: wdb@met.no
##
##  This is free software; you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation; either version 2 of the License, or
##  (at your option) any later version.
##
## - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

'''Daemon listening for signals to load data into wdb.

Starting this program, will create a data loading "daemon" listening for http
requests to load a data into wdb.

By default, a simple http server will be started, listening on port 7894. When
a request arrives at that port the http request should map down to a file, or 
set of files which will be loaded into the database.

Note that this program does not spawn a daemon automatically. In order to get
a daemon up and running, you must write something like this:

loaderDaemon 2>&1 >/dev/null &
'''

import sys


# __PKGPYTHONDIR__ is the python package installation path.
sys.path.insert(0, '__PKGPYTHONDIR__')

from WdbLoader import WdbLoader
import wdbInstallation
from BaseHTTPServer import HTTPServer
from SocketServer import ThreadingMixIn
from getopt import getopt


class WdbLoaderServer(ThreadingMixIn, HTTPServer):
    def __init__(self, address):
        HTTPServer.__init__(self, address, WdbLoader)
        ThreadingMixIn.daemon_threads = False

def serve(port):
    'Run the daemon, listening to the given port'
    server = WdbLoaderServer(('',port))
    server.serve_forever()

def version(out):
    'Send version information to <out>, which must be a file of some sort'
    out.write(wdbInstallation.baseVersionString % ('loaderDaemon',) + '\n')

def help(out):
    'Send a help string to <out>, which must be a file of some sort'
    version(out)
    out.write('\n')
    out.write(__doc__)
    out.write('\n')
    out.write('Command line options:\n')
    out.write('\n')
    out.write('Options:\n')
    out.write('\n')
    out.write('  --port arg=7894    Specify listening port for http server.\n')
    out.write('  --help             Display this help message\n')
    out.write('  --version          Display version information\n')
    out.write('\n')


if __name__ == '__main__':
    import sys
    import logging
    from logging.handlers import RotatingFileHandler
    import exceptions

    # Parse command line
    port = 7894 # default value
    logFileName = 'wdbLoaderServer.log'
    logLevel = 1 # debug
    try:
        args = getopt( sys.argv[1:], '', ['help', 'version', 'port=', 'logfile=', 'loglevel='])
    except:
        print 'Invalid arguments - rerun with --help for usage information'
        sys.exit(1)
    for optionName, value in args[0]:
        if optionName == '--help':
            help(sys.stdout)
            sys.exit(0)
        elif optionName == '--version':
            version(sys.stdout)
            sys.exit(0)
        elif optionName == '--port':
            port = int(value)
        elif optionName == '--logfile':
            logFileName = value
        elif optionName == '--loglevel':
            logLevel = int(value)
            if value not in range(1,5):
                print 'log level must be in range 1-5.'
                sys.exit(1)
            # Adapt to python's log level ranges:
            logLevel *= 10 
        else:
            print 'Unrecognized argument: ' + optionName + ' - rerun with --help for usage information'
            sys.exit(1)
    
    # Logging
    logger = logging.getLogger('wdb')
    handler = RotatingFileHandler(logFileName, 'a', 1024*64, 2)
    handler.setFormatter(logging.Formatter('%(asctime)s %(levelname)s %(message)s'))
    logger.addHandler(handler)
    logger.setLevel(logLevel)

    
    logger.info('Start loaderDaemon at port ' + str(port))
    
    try:
        serve(port)
    except exceptions.KeyboardInterrupt:
        pass
    except Exception, e:
        logger.critical(e)
        sys.exit(2)
        
    logger.info('Stopped loaderDaemon')
