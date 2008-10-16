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

from BaseHTTPServer import BaseHTTPRequestHandler
from htmlPathParser import ModelFilePathParser, PlainFilePathParser
import logging


class WdbLoader(BaseHTTPRequestHandler):
    '''Listens on for html requests on a specific port, and triggers loading of 
    data into wdb, based on the given request.
    
    The request is mapped down to a specific file to be loaded by a specific
    loading program. 
    '''
    
    parsePath = PlainFilePathParser()

    def do_GET(self):
        'The response to a http get query - load the file, and return "ok" if everything went well.'
        
        logger = logging.getLogger('wdb.request')
        
        try:
            logger.info('Got request: ' + self.path)
            loadingProgram, absolutePath = self.parsePath(self.path)
            loadingProgram( absolutePath )
            self.send_response(200, '')
            self.end_headers()
            self.wfile.write('ok - loaded ' + self.path + '\n')
            logger.debug('loaded file ' + str(absolutePath))
        except Exception, e:
            logger.error(self.path + ':\t' + str(e))
            self.send_error(404)
            raise
        
    
def setWdbLoaderPathParser(pathParsingFunction):
    WdbLoader.parsePath = pathParsingFunction

    
 