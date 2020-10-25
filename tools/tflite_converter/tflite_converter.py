savedModelDir = "export_dir/0"

import os
import tensorflow as tf

model = tf.saved_model.load(savedModelDir)
print("Num signatures:")
print(len(model.signatures))
concrete_func = model.signatures[tf.saved_model.DEFAULT_SERVING_SIGNATURE_DEF_KEY]

converter = tf.lite.TFLiteConverter.from_concrete_functions([concrete_func])
converter.allow_custom_ops = True
converter.target_ops = set([ tf.lite.OpsSet.TFLITE_BUILTINS, tf.lite.OpsSet.SELECT_TF_OPS ])
converter.target_spec.supported_ops = set([ tf.lite.OpsSet.TFLITE_BUILTINS, tf.lite.OpsSet.SELECT_TF_OPS ])
converter.experimental_new_converter = True
tflite_model = converter.convert()
open("converted_model.tflite", "wb").write(tflite_model)
