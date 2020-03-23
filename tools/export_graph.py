
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import tensorflow as tf
from tensorflow.python.framework import graph_util

tf.compat.v1.disable_eager_execution()

# create a session
sess = tf.compat.v1.Session()

# import best model
saver = tf.compat.v1.train.import_meta_graph(
    '/workspaces/spleeter/model/checkpoint/model.meta')  # graph
saver.restore(sess, tf.compat.v1.train.get_checkpoint_state(
    '/workspaces/spleeter/model/checkpoint').model_checkpoint_path)  # variables


# get graph definition
gd = sess.graph.as_graph_def()

# # fix batch norm nodes
for node in gd.node:
    if node.op == 'RefSwitch':
        node.op = 'Switch'
        for index in xrange(len(node.input)):
            if 'moving_' in node.input[index]:
                node.input[index] = node.input[index] + '/read'
    elif node.op == 'AssignSub':
        node.op = 'Sub'
    if 'use_locking' in node.attr:
        del node.attr['use_locking']
    if "dilations" in node.attr:
        del node.attr["dilations"]
    if "index_type" in node.attr:
        del node.attr["index_type"]

# generate protobuf
converted_graph_def = tf.compat.v1.graph_util.convert_variables_to_constants(sess, gd, [
    "strided_slice_23", "strided_slice_13"], )
tf.io.write_graph(converted_graph_def, '/workspaces/spleeter',
                  'model.pb', as_text=False)
