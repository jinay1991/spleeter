# Spleeter

## Dev Environment

To setup developer environment, the project requires following packages.

```
apt-get update && apt-get upgrade -y && apt-get autoremove -y

# Installation of general dependencies
apt-get install -y build-essential gcc g++ lcov make gdb
apt-get install -y openjdk-11-jdk openjdk-11-jre
apt-get install -y libtool clang-format-6.0
apt-get install -y git curl
apt-get install -y wget

# Installation of FFMPEG
apt-get install -y libavcodec-dev libavformat-dev libavfilter-dev libavdevice-dev libswresample-dev libswscale-dev ffmpeg
```

### Build System

This project uses `bazel` build system. To install, run following command or find documentation for installation on office site [here](https://docs.bazel.build/versions/master/install-ubuntu.html#installing-bazel).

For Linux/macOS systems,

```
echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list
curl https://bazel.build/bazel-release.pub.gpg | apt-key add -
apt-get update && apt-get install -y bazel
```

### Docker

One can use docker container to use as dev environment,

Install `docker` tool from official site (here)[https://www.docker.com/products/docker-desktop]

```
docker pull registry.gitlab.com/jinay1991/spleeter
```

## Reference

- Deezer Research - Source Separation Engine Story - deezer.io blog post:
    * [English version](https://deezer.io/releasing-spleeter-deezer-r-d-source-separation-engine-2b88985e797e)
    * [Japanese version](http://dzr.fm/splitterjp)
- [Music Source Separation tool with pre-trained models / ISMIR2019 extended abstract](http://archives.ismir.net/ismir2019/latebreaking/000036.pdf)

If you use **Spleeter** in your work, please cite:

```BibTeX
@misc{spleeter2019,
  title={Spleeter: A Fast And State-of-the Art Music Source Separation Tool With Pre-trained Models},
  author={Romain Hennequin and Anis Khlif and Felix Voituret and Manuel Moussallam},
  howpublished={Late-Breaking/Demo ISMIR 2019},
  month={November},
  note={Deezer Research},
  year={2019}
}
```

## License
The code of **Spleeter** is MIT-licensed.

## Disclaimer
If you plan to use Spleeter on copyrighted material, make sure you get proper authorization from right owners beforehand.

## Note

This repository include a demo audio file `audio_example.mp3` which is an excerpt from

```
Slow Motion Dream by Steven M Bryant
Copyright (c) 2011 Licensed under a Creative Commons Attribution (3.0) license.

http://dig.ccmixter.org/files/stevieb357/34740
Ft: CSoul Alex Beroza & Robert Siek"
```
