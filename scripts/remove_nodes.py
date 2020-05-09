import tensorflow as tf

from tensorflow.python.platform import gfile
model_path = "frozen_graph.pb"

# read graph definition
f = gfile.FastGFile(model_path, "rb")
gd = graph_def = tf.compat.v1.GraphDef()
graph_def.ParseFromString(f.read())

# fix nodes
for node in graph_def.node:
    if node.op == 'RefSwitch':
        node.op = 'Switch'
        for index in xrange(len(node.input)):
            if 'moving_' in node.input[index]:
                node.input[index] = node.input[index] + '/read'
    elif node.op == 'AssignSub':
        node.op = 'Sub'
        if 'use_locking' in node.attr:
            del node.attr['use_locking']

# import graph into session
tf.compat.v1.import_graph_def(graph_def, name='')
tf.io.write_graph(graph_def, './', 'good_frozen.pb', as_text=False)
tf.io.write_graph(graph_def, './', 'good_frozen.pbtxt', as_text=True)
