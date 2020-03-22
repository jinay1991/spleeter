FROM ubuntu:18.04

RUN apt-get update && apt-get upgrade -y && apt-get autoremove -y

# Installation of general dependencies
RUN apt-get install -y build-essential gcc g++ lcov make gdb
RUN apt-get install -y openjdk-11-jdk openjdk-11-jre
RUN apt-get install -y libtool clang-format-6.0
RUN apt-get install -y git git-lfs vim
RUN apt-get install -y wget curl

# Installation of Bazel Package
RUN echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list
RUN curl https://bazel.build/bazel-release.pub.gpg | apt-key add -
RUN apt-get update && apt-get install -y bazel

# Installation of Bazel Tools
RUN wget https://github.com/bazelbuild/buildtools/releases/download/0.29.0/buildifier
RUN chmod +x buildifier
RUN mv buildifier /usr/bin

# Installation of FFMPEG
RUN apt-get install -y libavcodec-dev libavformat-dev libavfilter-dev libavdevice-dev libswresample-dev libswscale-dev ffmpeg

# Installation of dependencies to Doxygen
RUN apt-get install -y doxygen graphviz plantuml

# Installation of static code analysis
RUN apt-get install -y cppcheck python python-pygments

# Installation of tensorflow python dependencies
RUN apt-get install -y python python-pip
RUN python -m pip install -U pip
RUN python -m pip install -U tensorflow
RUN python -m pip install -U future

# cleanup
RUN apt-get clean && rm -rf /var/lib/apt/lists/*
RUN apt-get autoremove && apt-get autoclean
