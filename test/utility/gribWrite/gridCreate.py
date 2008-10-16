#!/usr/bin/python

class Find:
    def __init__(self, pos, func):
        self.pos = pos
        self.val = None
        self.func = func
    def __call__(self, data):
        thisVal = data[self.pos]
        if self.val == None:
            self.val = thisVal
        else:
            self.val = self.func((self.val, thisVal))

def expandField(fieldin, expandValue = 9):
    field = {}
    for point in fieldin:
        x = point[0]
        y = point[1]
        for xc in range(x-1,x+2):
            for yc in range(y-1, y+2):
                try:
                    val = field[(xc, yc)]
                    if val == fieldin[(xc,yc)]:
                        continue
                except:
                    pass
                if xc == x and yc == y:
                    field[(xc, yc)] = fieldin[(xc,yc)]
                else:
                    field[(xc, yc)] = expandValue
    return field
       
       
def getMinMax(field):
    extremes = ([],[])
    
    for i in 0,1:
        funcs = (Find(i, min), Find(i, max))
        for f in funcs:
            for point in field:
                f(point)
            extremes[i].insert(9, f.val)
            
    return extremes
    
    
def printMap(field):
    extremes = getMinMax(field)
    
    for y in range(extremes[1][0], extremes[1][1] +1):
        for x in range(extremes[0][0], extremes[0][1] +1):
            try:
                val = field[(x,y)]
                print val,
            except:
                print '-',
        print


def parseFields(fieldList):
    import re
    r = re.compile('-P(\\d+),(\\d+)(=(\\d+\\.?\\d*))?')

    fields = {}
    for point in fieldList:
        m = r.match(point)
        if not m:
            raise Exception('Invalid syntax: ' + point)
        group = m.groups()
        if group[3] == None:
            fields[(int(group[0]), int(group[1]))] = 1
        else:
            fields[(int(group[0]), int(group[1]))] = int(group[3])
    return fields

def recreateArgs(field):
    for f in field.items():
        if f[1] != 1:
            print '-P%d,%d=%d' % (f[0][0], f[0][1], f[1]),
        else:
            print '-P%d,%d' % (f[0][0], f[0][1]),
    print
    
if __name__ == '__main__':
    import sys
    
    try:
        field = parseFields(sys.argv[1:])
        for i in range(9,10):
            field = expandField(field, i)
        printMap(field)
        recreateArgs(field)
        
    except Exception, msg:
        print msg
        sys.exit(1)
        