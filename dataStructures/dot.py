import sys
from graphviz import Source

structure = sys.argv[1]
file = "dotFiles\\" + structure
src = Source.from_file(file, format = 'png')
src.view()
