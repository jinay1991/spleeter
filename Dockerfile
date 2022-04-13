FROM ubuntu:20.04

ARG TARGETOS
ARG TARGETARCH

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update

# Installation of dev environment dependencies
RUN apt-get install -y \
    gcc g++ clang-format clang-tidy lcov \
    wget git git-lfs \
    openjdk-11-jdk openjdk-11-jre \
    libavcodec-dev libavformat-dev libavfilter-dev libavdevice-dev libswresample-dev libswscale-dev ffmpeg

# Installation of Bazel Package
RUN wget https://github.com/bazelbuild/bazelisk/releases/download/v1.11.0/bazelisk-${TARGETOS}-${TARGETARCH} && \
    chmod +x bazelisk-${TARGETOS}-${TARGETARCH} && \
    mv bazelisk-${TARGETOS}-${TARGETARCH} /usr/bin/bazel

# Installation of Bazel Tools
RUN wget https://github.com/bazelbuild/buildtools/releases/download/5.0.1/buildifier-${TARGETOS}-${TARGETARCH} && \
    chmod +x buildifier-${TARGETOS}-${TARGETARCH} && \
    mv buildifier-${TARGETOS}-${TARGETARCH} /usr/bin/buildifier

# cleanup
RUN apt-get clean && rm -rf /var/lib/apt/lists/* && \
    apt-get autoremove && apt-get autoclean
