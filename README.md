# ci-plugin-demo

A demo project that was used for ADC'21 Workshop titled `Introduction To CI Setup In The Cloud`. This repository contains some example Jenkins pipeline files that can be used with Projucer/cmake style JUCE plugin projects. The Jenkins files are reusable with other projects by simply changing the variables at the start of the file. 

## Building

To build from scratch using cmake:

```bash
# Clone the repository
git clone --recurse-submodules git@github.com:Thrifleganger/ci-plugin-demo.git
cd ci-plugin-demo

# build with CMake
cmake -BBuilds
cmake --build Builds --config Release
```
