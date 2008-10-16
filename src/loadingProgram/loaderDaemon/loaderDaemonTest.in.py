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


import sys
sys.path.insert(0, '__PKGPYTHONDIR__')

import unittest
import htmlPathParser

class FakeLoader:
    'Does no loading - only stores the list of files which it should have loaded'
    def __init__(self):
        self.fileList = []
    def __call__(self, fileList):
        self.fileList = fileList



class ModelFilePathParserTest(unittest.TestCase):
    'Unit tests for ModelFilePathParser'
    
    def setUp(self):
        self.parse = htmlPathParser.ModelFilePathParser()
        self.parse.loaders['model_A'] = (FakeLoader(), '/my/path/a') 
        
    def testValidModelName(self):
        loaders, paths = self.parse('/model_A/12')
        self.assertEqual(FakeLoader, loaders.__class__)
        self.assertEqual(1, len(paths))
        self.assertEqual('/my/path/a/fc12.grb*.p4', paths[0])
            
    def testInvalidModelName(self):
        self.assertRaises(KeyError, self.parse, '/invalidModelName/12')
        
    def testTooManyElementsInPath(self):
        self.assertRaises(Exception, self.parse, '/model_A/more_stuff/12')
        
    def testTooFewElementsInPath(self):
        self.assertRaises(Exception, self.parse, 'model_A')
        
    def testEmptyPath(self):
        self.assertRaises(Exception, self.parse, '')
        
        
class PlainFilePathParserTest(unittest.TestCase):
    'Unit tests for PlainFilePathParser'
    
    def setUp(self):
        self.parse = htmlPathParser.PlainFilePathParser()
        self.parse.loaders['/some_path/'] = FakeLoader() 
        
    def testValidModelName(self):
        loaders, paths = self.parse('/some_path/12')
        self.assertEqual(FakeLoader, loaders.__class__)
        self.assertEqual(1, len(paths))
        self.assertEqual('/some_path/12', paths[0])
        
    def testCorrectPathWithoutFileSpec(self):
        self.assertRaises(Exception, self.parse, '/some_path/')
            
    def testWrongPath(self):
        self.assertRaises(Exception, self.parse, '/invalidPath/12')
        
    def testEmptyPath(self):
        self.assertRaises(Exception, self.parse, '')

        
if __name__ == '__main__':
    unittest.main()
    