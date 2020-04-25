#!/usr/bin/env python
# coding: utf8


import numpy as np
import tensorflow as tf


def apply_unet(
        input_tensor,
        output_name='output',
        output_mask_logit=False):
    """ Apply a convolutionnal U-net to model a single instrument (one U-net
    is used for each instrument).

    :param input_tensor:
    :param output_name: (Optional) , default to 'output'
    :param output_mask_logit: (Optional) , default to False.
    """
    # First layer.
    conv1 = tf.keras.layers.Conv2D(filters=16,
                                   kernel_size=(5, 5),
                                   strides=(2, 2),
                                   padding="same",
                                   kernel_initializer="he_uniform")(input_tensor)
    batch1 = tf.keras.layers.BatchNormalization(axis=-1)(conv1)
    act1 = tf.keras.layers.Activation(activation="relu")(batch1)
    # Second layer.
    conv2 = tf.keras.layers.Conv2D(filters=32,
                                   kernel_size=(5, 5),
                                   strides=(2, 2),
                                   padding="same",
                                   kernel_initializer="he_uniform")(act1)
    batch2 = tf.keras.layers.BatchNormalization(axis=-1)(conv2)
    act2 = tf.keras.layers.Activation(activation="relu")(batch2)
    # Third layer.
    conv3 = tf.keras.layers.Conv2D(filters=64,
                                   kernel_size=(5, 5),
                                   strides=(2, 2),
                                   padding="same",
                                   kernel_initializer="he_uniform")(act2)
    batch3 = tf.keras.layers.BatchNormalization(axis=-1)(conv3)
    act3 = tf.keras.layers.Activation(activation="relu")(batch3)
    # Fourth layer.
    conv4 = tf.keras.layers.Conv2D(filters=128,
                                   kernel_size=(5, 5),
                                   strides=(2, 2),
                                   padding="same",
                                   kernel_initializer="he_uniform")(act3)
    batch4 = tf.keras.layers.BatchNormalization(axis=-1)(conv4)
    act4 = tf.keras.layers.Activation(activation="relu")(batch4)
    # Fifth layer.
    conv5 = tf.keras.layers.Conv2D(filters=256,
                                   kernel_size=(5, 5),
                                   strides=(2, 2),
                                   padding="same",
                                   kernel_initializer="he_uniform")(act4)
    batch5 = tf.keras.layers.BatchNormalization(axis=-1)(conv5)
    act5 = tf.keras.layers.Activation(activation="relu")(batch5)
    # Sixth layer
    conv6 = tf.keras.layers.Conv2D(filters=512,
                                   kernel_size=(5, 5),
                                   strides=(2, 2),
                                   padding="same",
                                   kernel_initializer="he_uniform")(act5)
    batch6 = tf.keras.layers.BatchNormalization(axis=-1)(conv6)
    _ = tf.keras.layers.Activation(activation="relu")(batch6)
    #
    #
    #
    up1 = tf.keras.layers.Conv2DTranspose(filters=256,
                                          kernel_size=(5, 5),
                                          strides=(2, 2),
                                          activation="relu",
                                          padding='same',
                                          kernel_initializer="he_uniform")((conv6))
    batch7 = tf.keras.layers.BatchNormalization(axis=-1)(up1)
    drop1 = tf.keras.layers.Dropout(0.5)(batch7)
    merge1 = tf.keras.layers.Concatenate(axis=-1)([conv5, drop1])
    #
    up2 = tf.keras.layers.Conv2DTranspose(filters=128,
                                          kernel_size=(5, 5),
                                          strides=(2, 2),
                                          activation="relu",
                                          padding='same',
                                          kernel_initializer="he_uniform")((merge1))
    batch8 = tf.keras.layers.BatchNormalization(axis=-1)(up2)
    drop2 = tf.keras.layers.Dropout(0.5)(batch8)
    merge2 = tf.keras.layers.Concatenate(axis=-1)([conv4, drop2])
    #
    up3 = tf.keras.layers.Conv2DTranspose(filters=64,
                                          kernel_size=(5, 5),
                                          strides=(2, 2),
                                          activation="relu",
                                          padding='same',
                                          kernel_initializer="he_uniform")((merge2))
    batch9 = tf.keras.layers.BatchNormalization(axis=-1)(up3)
    drop3 = tf.keras.layers.Dropout(0.5)(batch9)
    merge3 = tf.keras.layers.Concatenate(axis=-1)([conv3, drop3])
    #
    up4 = tf.keras.layers.Conv2DTranspose(filters=32,
                                          kernel_size=(5, 5),
                                          strides=(2, 2),
                                          activation="relu",
                                          padding='same',
                                          kernel_initializer="he_uniform")((merge3))
    batch10 = tf.keras.layers.BatchNormalization(axis=-1)(up4)
    merge4 = tf.keras.layers.Concatenate(axis=-1)([conv2, batch10])
    #
    up5 = tf.keras.layers.Conv2DTranspose(filters=16,
                                          kernel_size=(5, 5),
                                          strides=(2, 2),
                                          activation="relu",
                                          padding='same',
                                          kernel_initializer="he_uniform")((merge4))
    batch11 = tf.keras.layers.BatchNormalization(axis=-1)(up5)
    merge5 = tf.keras.layers.Concatenate(axis=-1)([conv1, batch11])
    #
    up6 = tf.keras.layers.Conv2DTranspose(filters=1,
                                          kernel_size=(5, 5),
                                          strides=(2, 2),
                                          activation="relu",
                                          padding='same',
                                          kernel_initializer="he_uniform")((merge5))
    batch12 = tf.keras.layers.BatchNormalization(axis=-1)(up6)

    # Last layer to ensure initial shape reconstruction.
    if not output_mask_logit:
        up7 = tf.keras.layers.Conv2D(filters=2,
                                     kernel_size=(4, 4),
                                     dilation_rate=(2, 2),
                                     activation='sigmoid',
                                     padding='same',
                                     kernel_initializer="he_uniform")((batch12))
        output = tf.keras.layers.Multiply(name=output_name)([up7, input_tensor])
        return output

    return tf.keras.layers.Conv2D(filters=2,
                                  kernel_size=(4, 4),
                                  dilation_rate=(2, 2),
                                  padding='same',
                                  kernel_initializer="he_uniform")((batch12))


if __name__ == "__main__":
    input_tensor = np.zeros(shape=(1, 1, 479832, 2), dtype=np.float32)
    apply_unet(input_tensor)
