# Spleeter

## Dev Environment

Supported OS:

* Ubuntu 18.04
* macOS Catalina v10.15

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


# Installation of python dependencies
apt-get install -y python python-pip
python -m pip install -U pip
python -m pip install -U tensorflow
python -m pip install -U future
python -m pip install -U six

# Installation of python3 dependencies
apt-get install -y python3 python3-pip
python3 -m pip install -U pip
python3 -m pip install -U future
python3 -m pip install -U six
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

Install `docker` tool from official site [here](https://www.docker.com/products/docker-desktop)

```
docker pull registry.gitlab.com/jinay1991/spleeter
```

## Usage

To quickly run the `spleeter` application, run `bazel run //application:spleeter`

To quickly run unit/component tests, run `bazel test //... --test_output=all --cache_test_results=false`

To build package (`*.tar.gz`), run `bazel build //:spleeter-dev`

## Integration

To integrate it external application, please refer `example` directory containing separate `WORKSPACE` uses `spleeter` and it's dependencies.

Please note that `spleeter` has some external dependencies (i.e. `libtensorflow` etc.), which is why `third_party` directory is necessary for downloading all the dependencies and link them together with your application.

Usage on API can be found on [Doxygen](https://jinay1991.gitlab.io/spleeter/doc/html/) documentation.

Example implementation can be found in `example/spleeter_app.cpp` (Snipate below)

```
///
/// @file
/// @copyright Copyright (c) 2020, MIT License
///
#include "spleeter/argument_parser/cli_options.h"
#include "spleeter/spleeter.h"

#include <iostream>
#include <memory>

int main(void)
{
    try
    {
        /// Initialize
        auto cli_options = spleeter::CLIOptions{};
        cli_options.inputs = std::string{"external/audio_example/file/audio_example.wav"};
        cli_options.output_path = std::string{"separated_audio"};
        cli_options.configuration = std::string{"spleeter:5stems"};

        cli_options.audio_adapter = std::string{"audionamix"};
        cli_options.codec = std::string{"wav"};
        cli_options.bitrate = 192000;

        auto spleeter = std::make_unique<spleeter::Spleeter>(cli_options);
        spleeter->Init();

        /// Run
        spleeter->Execute();

        /// Deinitialize
        spleeter->Shutdown();
    }
    catch (std::exception& e)
    {
        std::cerr << "Caught Exception!! " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
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

Converted official checkpoint to TFLite Model using https://github.com/tinoucas/spleeter-tflite-convert

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
