FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update

# Installation of general dependencies
RUN apt-get install -y build-essential
RUN apt-get install -y clangd clang-format clang-tidy
RUN apt-get install -y libtool
RUN apt-get install -y git git-lfs
RUN apt-get install -y wget curl vim

# Installation of dependencies to Doxygen
RUN apt-get install -y doxygen graphviz plantuml

# Installation of static code analysis
RUN apt-get install -y cppcheck python python-pygments

# Installatin of dependencies to Bazel
RUN apt-get install -y openjdk-11-jdk openjdk-11-jre

# Installation of Bazel Package
RUN curl https://bazel.build/bazel-release.pub.gpg | apt-key add -
RUN echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list
RUN apt-get update && apt-get install -y bazel
RUN echo "source /etc/bash_completion.d/bazel" >> ~/.bashrc

# Installation of Bazel Tools
RUN wget https://github.com/bazelbuild/buildtools/releases/download/3.5.0/buildifier
RUN chmod +x buildifier
RUN mv buildifier /usr/bin

# Installation of FFMPEG
RUN apt-get install -y libavcodec-dev libavformat-dev libavfilter-dev libavdevice-dev libswresample-dev libswscale-dev ffmpeg

# cleanup
RUN apt-get clean && rm -rf /var/lib/apt/lists/*
RUN apt-get autoremove && apt-get autoclean
