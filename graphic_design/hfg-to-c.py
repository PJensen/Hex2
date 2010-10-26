import os
import sys
import string
import glob

fileName = raw_input("Enter HFG filename to convert to C-data:")


fp = open(fileName, 'r')
data = fp.read()

data = data.split('\n')

width = int(data[0].split(' ')[0])
height = int(data[0].split(' ')[1])

print "WIDTH & HEIGHT:", width, height

os.system("pause")
outFile = open('out.c.dat', 'w+')

os.system("pause")
# The opening brace
outFile.write('{\n')


for yIndex in range(1, height + 1):
	outFile.write('\t{')
	
	for xIndex in range(1, width):

		outFile.write(str(data[yIndex].split(' ')[xIndex]))
		if (xIndex < width - 1):
			outFile.write(', ')

	outFile.write('}')

	if (yIndex <> height):
		outFile.write(',\n')

outFile.write('};')

