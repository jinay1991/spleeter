#!/usr/bin/python
# Reference: https://github.com/gvne/spleeterpp/blob/master/cmake/export_spleeter_models.py

"""
Requires:

tensorflow==1.14.0
spleeter

"""


import os
import json
import argparse
import tempfile
import shutil

import tensorflow as tf

import spleeter
from spleeter.model import model_fn


SPLEETER_ROOT = os.path.dirname(spleeter.__file__)


def export_model(checkpoint_path, model_name, export_directory):
    # read the json parameters
    param_path = os.path.join(SPLEETER_ROOT, "resources", model_name + ".json")
    with open(param_path) as parameter_file:
        parameters = json.load(parameter_file)
    parameters['MWF'] = False  # default parameter

    # create the estimator
    configuration = tf.estimator.RunConfig(
        session_config=tf.compat.v1.ConfigProto())
    estimator = tf.estimator.Estimator(
        model_fn=model_fn,
        model_dir=os.path.join(checkpoint_path, model_name),
        params=parameters,
        config=configuration
    )

    # convert it to predictor
    def receiver():
        shape = (None, parameters['n_channels'])
        features = {
            'waveform': tf.compat.v1.placeholder(tf.float32, shape=shape),
            'audio_id': tf.compat.v1.placeholder(tf.string)
        }
        return tf.estimator.export.ServingInputReceiver(features, features)
    # export the estimator into a temp directory
    estimator.export_saved_model(export_directory, receiver)


def main():
    parser = argparse.ArgumentParser(description='Export spleeter models')
    parser.add_argument("-c", "checkpoint_path",
                        help="Checkpoints for spleeter model")
    parser.add_argument("-s", "saved_model_dir",
                        help="Output path for saving exported saved_model.")
    args = parser.parse_args()

    os.makedirs(args.saved_model_dir, exist_ok=True)

    for model in os.listdir(args.checkpoint_path):
        # the model is exported under a timestamp. We export in a temp dir,
        # then we move the created folder to the right export path
        destination = os.path.join(args.saved_model_dir, model)
        temp_dir = tempfile.mkdtemp()
        export_model(args.checkpoint_path, model, temp_dir)
        created_dir = os.path.join(temp_dir, os.listdir(temp_dir)[0])
        shutil.move(created_dir, destination)
        shutil.rmtree(temp_dir)  # cleanup


if __name__ == '__main__':
    main()
