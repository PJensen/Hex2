import os
import sys
import glob
import string

pcxFiles = glob.glob("*.pcx")

for index in pcxFiles:
    os.system("rimg " + str(index) + " " + string.split(index,'.')[0] + str(".hfg"))
              
