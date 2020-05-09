#!/usr/bin/env python
# coding: utf8


import numpy as np
import tensorflow as tf


class UNet(tf.keras.Model):
    def __init__(self, output_name='output', output_mask_logit=False):
        super(UNet, self).__init__()

        # First layer.
        self.conv1 = tf.keras.layers.Conv2D(filters=16,
                                            kernel_size=(5, 5),
                                            strides=(2, 2),
                                            padding="same",
                                            kernel_initializer="he_uniform")
        self.batch1 = tf.keras.layers.BatchNormalization(axis=-1)
        self.act1 = tf.keras.layers.Activation(activation="relu")
        # Second layer.
        self.conv2 = tf.keras.layers.Conv2D(filters=32,
                                            kernel_size=(5, 5),
                                            strides=(2, 2),
                                            padding="same",
                                            kernel_initializer="he_uniform")
        self.batch2 = tf.keras.layers.BatchNormalization(axis=-1)
        self.act2 = tf.keras.layers.Activation(activation="relu")
        # Third layer.
        self.conv3 = tf.keras.layers.Conv2D(filters=64,
                                            kernel_size=(5, 5),
                                            strides=(2, 2),
                                            padding="same",
                                            kernel_initializer="he_uniform")
        self.batch3 = tf.keras.layers.BatchNormalization(axis=-1)
        self.act3 = tf.keras.layers.Activation(activation="relu")
        # Fourth layer.
        self.conv4 = tf.keras.layers.Conv2D(filters=128,
                                            kernel_size=(5, 5),
                                            strides=(2, 2),
                                            padding="same",
                                            kernel_initializer="he_uniform")
        self.batch4 = tf.keras.layers.BatchNormalization(axis=-1)
        self.act4 = tf.keras.layers.Activation(activation="relu")
        # Fifth layer.
        self.conv5 = tf.keras.layers.Conv2D(filters=256,
                                            kernel_size=(5, 5),
                                            strides=(2, 2),
                                            padding="same",
                                            kernel_initializer="he_uniform")
        self.batch5 = tf.keras.layers.BatchNormalization(axis=-1)
        self.act5 = tf.keras.layers.Activation(activation="relu")
        # Sixth layer
        self.conv6 = tf.keras.layers.Conv2D(filters=512,
                                            kernel_size=(5, 5),
                                            strides=(2, 2),
                                            padding="same",
                                            kernel_initializer="he_uniform")
        self.batch6 = tf.keras.layers.BatchNormalization(axis=-1)
        self.act6 = tf.keras.layers.Activation(activation="relu")
        #
        #
        #
        self.up1 = tf.keras.layers.Conv2DTranspose(filters=256,
                                                   kernel_size=(5, 5),
                                                   strides=(2, 2),
                                                   activation="relu",
                                                   padding='same',
                                                   kernel_initializer="he_uniform")
        self.batch7 = tf.keras.layers.BatchNormalization(axis=-1)
        self.drop1 = tf.keras.layers.Dropout(0.5)
        #
        self.up2 = tf.keras.layers.Conv2DTranspose(filters=128,
                                                   kernel_size=(5, 5),
                                                   strides=(2, 2),
                                                   activation="relu",
                                                   padding='same',
                                                   kernel_initializer="he_uniform")
        self.batch8 = tf.keras.layers.BatchNormalization(axis=-1)
        self.drop2 = tf.keras.layers.Dropout(0.5)
        #
        self.up3 = tf.keras.layers.Conv2DTranspose(filters=64,
                                                   kernel_size=(5, 5),
                                                   strides=(2, 2),
                                                   activation="relu",
                                                   padding='same',
                                                   kernel_initializer="he_uniform")
        self.batch9 = tf.keras.layers.BatchNormalization(axis=-1)
        self.drop3 = tf.keras.layers.Dropout(0.5)
        #
        self.up4 = tf.keras.layers.Conv2DTranspose(filters=32,
                                                   kernel_size=(5, 5),
                                                   strides=(2, 2),
                                                   activation="relu",
                                                   padding='same',
                                                   kernel_initializer="he_uniform")
        self.batch10 = tf.keras.layers.BatchNormalization(axis=-1)
        #
        self.up5 = tf.keras.layers.Conv2DTranspose(filters=16,
                                                   kernel_size=(5, 5),
                                                   strides=(2, 2),
                                                   activation="relu",
                                                   padding='same',
                                                   kernel_initializer="he_uniform")
        self.batch11 = tf.keras.layers.BatchNormalization(axis=-1)
        #
        self.up6 = tf.keras.layers.Conv2DTranspose(filters=1,
                                                   kernel_size=(5, 5),
                                                   strides=(2, 2),
                                                   activation="relu",
                                                   padding='same',
                                                   kernel_initializer="he_uniform")
        self.batch12 = tf.keras.layers.BatchNormalization(axis=-1)

        # Last layer to ensure initial shape reconstruction.
        self.output_name = output_name
        if not output_mask_logit:
            self.output_mask_logit = False
            self.up7 = tf.keras.layers.Conv2D(filters=2,
                                              kernel_size=(4, 4),
                                              dilation_rate=(2, 2),
                                              activation='sigmoid',
                                              padding='same',
                                              kernel_initializer="he_uniform")
        else:
            self.output_mask_logit = True
            self.logits = tf.keras.layers.Conv2D(filters=2,
                                                 kernel_size=(4, 4),
                                                 dilation_rate=(2, 2),
                                                 padding='same',
                                                 kernel_initializer="he_uniform")

    def call(self, inputs, training=False):
        conv1 = self.conv1(inputs)
        batch1 = self.batch1(conv1)
        act1 = self.act1(batch1)
        # Second layer.
        conv2 = self.conv2(act1)
        batch2 = self.batch2(conv2)
        act2 = self.act2(batch2)
        # Third layer.
        conv3 = self.conv3(act2)
        batch3 = self.batch3(conv3)
        act3 = self.act3(batch3)
        # Fourth layer.
        conv4 = self.conv4(act3)
        batch4 = self.batch4(conv4)
        act4 = self.act4(batch4)
        # Fifth layer.
        conv5 = self.conv5(act4)
        batch5 = self.batch5(conv5)
        act5 = self.act5(batch5)
        # Sixth layer
        conv6 = self.conv6(act5)
        batch6 = self.batch6(conv6)
        _ = self.act6(batch6)
        #
        #
        #
        up1 = self.up1(conv6)
        batch7 = self.batch7(up1)
        drop1 = self.drop1(batch7)
        merge1 = tf.keras.layers.Concatenate(axis=-1)([conv5, drop1])
        #
        up2 = self.up1(merge1)
        batch8 = self.batch8(up2)
        drop2 = self.drop2(batch8)
        merge2 = tf.keras.layers.Concatenate(axis=-1)([conv4, drop2])
        #
        up3 = self.up3(merge2)
        batch9 = self.batch9(up3)
        drop3 = self.drop3(batch9)
        merge3 = tf.keras.layers.Concatenate(axis=-1)([conv3, drop3])
        #
        up4 = self.up4(merge3)
        batch10 = self.batch10(up4)
        merge4 = tf.keras.layers.Concatenate(axis=-1)([conv2, batch10])
        #
        up5 = self.up5(merge4)
        batch11 = self.batch11(up5)
        merge5 = tf.keras.layers.Concatenate(axis=-1)([conv1, batch11])
        #
        up6 = self.up6(merge5)
        batch12 = self.batch12(up6)

        # Last layer to ensure initial shape reconstruction.
        if not self.output_mask_logit:
            up7 = self.up7(batch12)
            return tf.keras.layers.Multiply(name=self.output_name)([up7, input_tensor])
        else:
            return self.logits(batch12)


@tf.function
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
                                          kernel_initializer="he_uniform")(conv6)
    batch7 = tf.keras.layers.BatchNormalization(axis=-1)(up1)
    drop1 = tf.keras.layers.Dropout(0.5)(batch7)
    merge1 = tf.keras.layers.Concatenate(axis=-1)([conv5, drop1])
    #
    up2 = tf.keras.layers.Conv2DTranspose(filters=128,
                                          kernel_size=(5, 5),
                                          strides=(2, 2),
                                          activation="relu",
                                          padding='same',
                                          kernel_initializer="he_uniform")(merge1)
    batch8 = tf.keras.layers.BatchNormalization(axis=-1)(up2)
    drop2 = tf.keras.layers.Dropout(0.5)(batch8)
    merge2 = tf.keras.layers.Concatenate(axis=-1)([conv4, drop2])
    #
    up3 = tf.keras.layers.Conv2DTranspose(filters=64,
                                          kernel_size=(5, 5),
                                          strides=(2, 2),
                                          activation="relu",
                                          padding='same',
                                          kernel_initializer="he_uniform")(merge2)
    batch9 = tf.keras.layers.BatchNormalization(axis=-1)(up3)
    drop3 = tf.keras.layers.Dropout(0.5)(batch9)
    merge3 = tf.keras.layers.Concatenate(axis=-1)([conv3, drop3])
    #
    up4 = tf.keras.layers.Conv2DTranspose(filters=32,
                                          kernel_size=(5, 5),
                                          strides=(2, 2),
                                          activation="relu",
                                          padding='same',
                                          kernel_initializer="he_uniform")(merge3)
    batch10 = tf.keras.layers.BatchNormalization(axis=-1)(up4)
    merge4 = tf.keras.layers.Concatenate(axis=-1)([conv2, batch10])
    #
    up5 = tf.keras.layers.Conv2DTranspose(filters=16,
                                          kernel_size=(5, 5),
                                          strides=(2, 2),
                                          activation="relu",
                                          padding='same',
                                          kernel_initializer="he_uniform")(merge4)
    batch11 = tf.keras.layers.BatchNormalization(axis=-1)(up5)
    merge5 = tf.keras.layers.Concatenate(axis=-1)([conv1, batch11])
    #
    up6 = tf.keras.layers.Conv2DTranspose(filters=1,
                                          kernel_size=(5, 5),
                                          strides=(2, 2),
                                          activation="relu",
                                          padding='same',
                                          kernel_initializer="he_uniform")(merge5)
    batch12 = tf.keras.layers.BatchNormalization(axis=-1)(up6)

    # Last layer to ensure initial shape reconstruction.
    if not output_mask_logit:
        up7 = tf.keras.layers.Conv2D(filters=2,
                                     kernel_size=(4, 4),
                                     dilation_rate=(2, 2),
                                     activation='sigmoid',
                                     padding='same',
                                     kernel_initializer="he_uniform")(batch12)
        output = tf.keras.layers.Multiply(name=output_name)([up7, input_tensor])
        return output

    return tf.keras.layers.Conv2D(filters=2,
                                  kernel_size=(4, 4),
                                  dilation_rate=(2, 2),
                                  padding='same',
                                  kernel_initializer="he_uniform")(batch12)


if __name__ == "__main__":
    input_tensor = np.zeros(shape=(1, 256, 256, 2), dtype=np.float32)

    model = UNet()
    model.trainable = False
    model._set_inputs(input_tensor)
    # model.compile(tf.keras.optimizers.Adam(learning_rate=1e-3), loss=tf.keras.losses.MeanSquaredError())
    # model.build()
    # model.summary()

    tf.saved_model.save(model, export_dir="saved_model")
