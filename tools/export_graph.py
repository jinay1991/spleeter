import tensorflow as tf
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-s", "--checkpoint_dir",
                    help="Saved Model directory", required=True)
args = parser.parse_args()

meta_path = args.checkpoint_dir + '/model.ckpt-2092200.meta'

with tf.compat.v1.Session() as sess:
    # Restore the graph
    saver = tf.compat.v1.train.import_meta_graph(meta_path)

    # Load weights
    saver.restore(sess, tf.train.latest_checkpoint(args.checkpoint_dir))

    output_node_names = [
        n.name for n in tf.compat.v1.get_default_graph().as_graph_def().node]

    # Freeze the graph
    frozen_graph_def = tf.compat.v1.graph_util.convert_variables_to_constants(
        sess,
        sess.graph_def,
        output_node_names)

    # saver.save(sess, "saved_model")
    # Save the frozen graph
    with open('output_graph.pb', 'wb') as f:
        f.write(frozen_graph_def.SerializeToString())
