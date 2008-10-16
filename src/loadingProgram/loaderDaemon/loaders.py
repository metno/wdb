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


from wdbInstallation import loaderBaseDirectory
import os

def load(loader, fileList):
    'Invokes the loader program on each file in fileList.' 

    if len(fileList) == 0:
        raise exception('Cannot load empty file list')
    files = ''
    for element in fileList:
        files += ' ' + element
        if not os.path.exists(element):
            raise IOError('Unable to find file ' + element)
    cmd = loader + ' --loadPlaceDefinition --logfile=feltLoad.'+ fileList[0].split('/')[-1] +'.log ' + files
    print cmd
    status = os.system(cmd)
    if status != 0:
        raise IOError('Error when loading data ' + str(fileList))

def gribLoad(fileList):
    return load(loaderBaseDirectory + '/gribLoad', fileList)

def feltLoad(fileList):
    return load(loaderBaseDirectory + '/feltLoad', fileList)
