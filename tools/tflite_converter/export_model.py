import os
import tensorflow as tf

trained_checkpoint_prefix = '/workspace/5stems/model'
export_dir = os.path.join('export_dir', '0')

graph = tf.Graph()
with tf.compat.v1.Session(graph=graph) as sess:
    # Restore from checkpoint
    loader = tf.compat.v1.train.import_meta_graph(trained_checkpoint_prefix + '.meta')
    loader.restore(sess, trained_checkpoint_prefix)

    waveform_tensor = tf.compat.v1.get_default_graph().get_tensor_by_name("waveform:0")
    waveform_tensor_info = tf.compat.v1.saved_model.utils.build_tensor_info(waveform_tensor)

    bass_tensor = tf.compat.v1.get_default_graph().get_tensor_by_name("strided_slice_48:0")
    bass_tensor_info = tf.compat.v1.saved_model.utils.build_tensor_info(bass_tensor)

    drums_tensor = tf.compat.v1.get_default_graph().get_tensor_by_name("strided_slice_38:0")
    drums_tensor_info = tf.compat.v1.saved_model.utils.build_tensor_info(drums_tensor)

    other_tensor = tf.compat.v1.get_default_graph().get_tensor_by_name("strided_slice_58:0")
    other_tensor_info = tf.compat.v1.saved_model.utils.build_tensor_info(other_tensor)

    piano_tensor = tf.compat.v1.get_default_graph().get_tensor_by_name("strided_slice_28:0")
    piano_tensor_info = tf.compat.v1.saved_model.utils.build_tensor_info(piano_tensor)

    vocals_tensor = tf.compat.v1.get_default_graph().get_tensor_by_name("strided_slice_18:0")
    vocals_tensor_info = tf.compat.v1.saved_model.utils.build_tensor_info(vocals_tensor)

    current_graph = tf.compat.v1.get_default_graph()

    print(waveform_tensor_info)
    print(waveform_tensor)

    separate_signature = (
        tf.compat.v1.saved_model.signature_def_utils.build_signature_def(
            inputs={ 'waveform': waveform_tensor_info },
            outputs={ 'bass': bass_tensor_info,
                      'drums': drums_tensor_info,
                      'other': other_tensor_info,
                      'vocals': vocals_tensor_info,
                      'piano': piano_tensor_info },
            method_name=tf.compat.v1.saved_model.signature_constants
          .PREDICT_METHOD_NAME))

    # Export checkpoint to SavedModel
    builder = tf.compat.v1.saved_model.builder.SavedModelBuilder(export_dir)
    builder.add_meta_graph_and_variables(sess, [tf.compat.v1.saved_model.tag_constants.SERVING],
                                        signature_def_map={
                                            tf.compat.v1.saved_model.signature_constants.DEFAULT_SERVING_SIGNATURE_DEF_KEY:
                                                separate_signature,
                                            },
                                         strip_default_attrs=True)
    builder.save()
