from graphviz import Source

file = r'dotFiles\rbtree.dot'
src = Source.from_file(file, format = 'png')
src.view()
