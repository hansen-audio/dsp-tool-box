# DSP Tool Box

## Motivation

The ```dsp-tool-box``` contains very basic DSP algorithms like e.g. filters or a modulation phase.

## Building the project

Execute the following commands on cli.

```
git clone https://www.github.com/hansen-audio/dsp-tool-box.git
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ../dsp-tool-box
cmake --build .
```

### CMake Generators

CMake geneartors for all platforms.

* Linux: ```cmake -DCMAKE_BUILD_TYPE=[Debug|Release] ...```
* macOS: ```cmake -GXcode ...```
* Windows 10: ```cmake -G"Visual Studio 16 2019" -A x64 ...```

## Using the algorithms

All algorithm classes in this library contain a ```context``` and ```static``` methods in order to modify the ```context```. Like this the data and the algorithm are separated and allow a usage in a multithreaded environment.


### Setting parameters of the context

Use the ```create``` method of the ```phase``` class in order to get a valid ```context```. Afterwards use the ```static``` methods like ```set_tempo``` to modify the ```context```.

```
auto phase_context = ha::dtb::modulation::phase::create();
ha::dtb::modulation::phase::set_tempo(phase_context, 120.);
```

## License

Copyright 2021 Hansen Audio

Licensed under the MIT: https://mit-license.org/
