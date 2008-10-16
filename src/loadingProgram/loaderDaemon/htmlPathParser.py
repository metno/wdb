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

#import wdbInstallation
import loaders


class PlainFilePathParser:
    '''Decides what files to load, and how, based on a given symbolic path.
    In this case the symbolic path equals the real path. Each path has an 
    associated loader. 
    '''
    
    loaders = {'/opdata/hirlam': loaders.gribLoad, 
               '/opdata/proff': loaders.feltLoad,
               '/opdata/ec': loaders.feltLoad
               }
    def __call__(self, symbolicPath):
        for key in self.loaders:
            if symbolicPath.startswith(key):
                if symbolicPath == key: # we need more than just the path
                    break
                return self.loaders[key], [symbolicPath]
        raise Exception('Invalid symbolicPath: ' + symbolicPath )
            


class ModelFilePathParser: 
    'Decides what files to load, and how, based on a given symbolic path.'

    # Mapping from data types in URL to loader and path to find files:
    loaders = {'hirlam8': (loaders.gribLoad, '/opdata/hirlam8/grib'),
              'hirlam10': (loaders.gribLoad, '/opdata/hirlam10/grib'),
              'hirlam12': (loaders.gribLoad, '/opdata/hirlam12/grib'),
              'hirlam20': (loaders.gribLoad, '/opdata/hirlam20/grib')
              }
    
    def __call__(self, symbolicPath):
        '''Path parser function. Decides what loader to use and what 
        absolute file name to load, based on the http request.
        
        expected input is <model>/<time>, for example hirlam8/fc00.grb13p4'''
    
        # remove '/' before and after string. After that split into model/file
        strippedPath = symbolicPath.strip('/')
        spec = strippedPath.split('/')
        if len(spec) != 2:
            raise Exception('Invalid symbolicPath: ' + symbolicPath )
        handler = self.loaders[spec[0]]
        path = handler[1] + '/fc' + spec[1] + '.grb*.p4'
        
        loadingProgram = handler[0]
        
        return loadingProgram, [path]  


class GribFilePathParser:
    'Assumes all files are GRIB files, and tries to load them'
    
    def __call__(self, path):
        return loaders.gribLoad, [path]
    