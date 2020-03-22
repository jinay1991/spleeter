import tensorflow as tf
import argparse

import six
from six.moves import range


def get_tensor_name(tensor):
    """Returns name of the input tensor.

    Args:
      tensor: tf.Tensor

    Returns:
      str
    """
    parts = six.ensure_str(tensor.name).split(":")
    if len(parts) > 2:
        raise ValueError("Tensor name invalid. Expect 0 or 1 colon, got {0}".format(
            len(parts) - 1))

    # To be consistent with the tensor naming scheme in tensorflow, we need
    # drop the ':0' suffix for the first tensor.
    if len(parts) > 1 and parts[1] != "0":
        return tensor.name
    return parts[0]


parser = argparse.ArgumentParser()
parser.add_argument("-s", "--checkpoint_dir",
                    help="Saved Model directory", required=True)
args = parser.parse_args()

meta_path = args.checkpoint_dir + '/model.ckpt-2092200.meta'
checkpoint = tf.train.get_checkpoint_state(args.checkpoint_dir)
input_checkpoint = checkpoint.model_checkpoint_path

absolute_model_dir = "/".join(input_checkpoint.split("/")[:-1])
output_graph = absolute_model_dir + "/frozen_graph.pb"

with tf.compat.v1.Session(graph=tf.Graph()) as sess:
    # Restore the graph
    saver = tf.compat.v1.train.import_meta_graph(input_checkpoint + ".meta")

    # Load weights
    saver.restore(sess, input_checkpoint)

    output_node_names = [
        n.name for n in tf.compat.v1.get_default_graph().as_graph_def().node]

    tensor_name_to_tensor = {}
    for op in tf.compat.v1.get_default_graph().get_operations():
        for tensor in op.values():
            tensor_name_to_tensor[get_tensor_name(tensor)] = tensor
    # Get the tensors associated with tensor_names.
    tensors = []
    invalid_tensors = []
    for name in ["waveform"]:
        tensor = tensor_name_to_tensor.get(name)
        if tensor is None:
            invalid_tensors.append(name)
        else:
            tensors.append(tensor)
    import pdb
    pdb.set_trace()
    # Freeze the graph
    frozen_graph_def = tf.compat.v1.graph_util.convert_variables_to_constants(
        sess,
        tf.compat.v1.get_default_graph().as_graph_def(),
        output_node_names)

    # saver.save(sess, "saved_model")
    # Save the frozen graph
    with open('frozen_graph.pb', 'wb') as f:
        f.write(frozen_graph_def.SerializeToString())
    print("%d ops in the final graph." % len(frozen_graph_def.node))


# def model_fn(features, labels, mode, params, config):
#     return tf.estimator.EstimatorSpec(
#         tf.estimator.ModeKeys.PREDICT)


# def get_input_dict_placeholders():
#     shape = (None, 2)
#     features = {
#         'waveform': tf.compat.v1.placeholder(tf.float32, shape=shape, name="waveform"),
#         'audio_id': tf.compat.v1.placeholder(tf.string, name="audio_id")}
#     return features


# def receiver():
#     features = get_input_dict_placeholders()
#     return tf.estimator.export.ServingInputReceiver(features, features)


# session_config = tf.compat.v1.ConfigProto()
# session_config.gpu_options.per_process_gpu_memory_fraction = 0.7
# config = tf.estimator.RunConfig(session_config=session_config)

# estimator = tf.estimator.Estimator(
#     model_fn=model_fn, model_dir="5stems", params=None, config=config)

# estimator.export_saved_model(".", receiver, checkpoint_path=meta_path)
