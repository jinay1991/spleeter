load("@//third_party/audionamix:audionamix.bzl", "audionamix")
load("@//third_party/ffmpeg:ffmpeg.bzl", "ffmpeg")
load("@//third_party/googletest:googletest.bzl", "googletest")
load("@//third_party/models:models.bzl", "models")
load("@//third_party/nlohmann:nlohmann.bzl", "nlohmann")
load("@//third_party/tensorflow:tensorflow.bzl", "tensorflow")

def third_party_dependencies():
    """ Load 3rd Party Dependencies """
    audionamix()
    ffmpeg()
    googletest()
    models()
    nlohmann()
    tensorflow()
