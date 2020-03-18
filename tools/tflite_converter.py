#!/usr/bin/python
# Copyright (c) 2020, MIT License

import argparse

import tensorflow as tf

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("-s", "--saved_model_dir",
                        help="Saved Model directory", required=True)
    args = parser.parse_args()

    converter = tf.lite.TFLiteConverter.from_saved_model(args.saved_model_dir)
    tflite_model = converter.convert()
    open("converted_model.tflite", "wb").write(tflite_model)
