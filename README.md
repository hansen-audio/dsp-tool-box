# DSP Tool Box

## Motivation

The ```dsp-tool-box``` contains very basic DSP algorithms. All algorithms can operate on one frame (4 channels) at a time. Like this an update of a parameter can be done any time while processing.

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

## Algorithms

Currently the following algorithms are available:

* one pole filter
* modulation phase

## Using the algorithms

All algorithm classes in this library contain a ```context``` and ```static``` methods in order to modify the ```context```. Like this the data and the algorithm are separated and allow a usage in a multithreaded environment.

### Setting parameters of the context

Use the ```create``` method of the ```phase``` class in order to get a valid ```context```. Afterwards use the ```static``` methods like ```set_tempo``` to modify the ```context```.

```
auto phase_context = ha::dtb::modulation::phase::create();
ha::dtb::modulation::phase::set_tempo(phase_context, 120.);
```

### Processing a module

After setting all parameters of the ```context``` the  ```phase``` can ```advance``` a ```value``` by the specified amount of samples, in this case ```32```. The ```overflow``` boolean flags an overflow.

```
real value = 0;
bool overflow = ha::dtb::modulation::phase::advance(context const& cx, mut_real& value, 32);
...
```

## License

Copyright 2021 Hansen Audio

Licensed under the MIT: https://mit-license.org/
