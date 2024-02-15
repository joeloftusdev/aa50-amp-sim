## AA50 AMP SIMULATOR
![aa50](https://github.com/joeloftusdev/aa50-amp-sim/assets/152509645/1fa6c6f4-5c75-49bb-9881-0957f77bba23)

A guitar amp simulator plugin built with C++ & the JUCE Library. Loosely based on a well known 90's "block letter" amp head.

## Features
* Impulse response loader (load in your favourite IRs!)
* Pre & Post Gain
* Waveshaping distortion
* EQ - Bass, Mid & Treble
* Resonance
* Presence
* Output Meter

## How to Build
(Run cmd/git bash/powershell as administrator)
```
git clone <url>
cd aa50-amp-sim
git submodule update --init
mkdir out
cd out
cmake ..
cmake --build .
```
* More info & how to build on MacOs in the [pamplejuce repo](https://github.com/sudara/pamplejuce) 
* You can find some impulse responses if needed [here](https://producelikeapro.com/blog/best-guitar-impulse-responses/)
* Tested in [Reaper](https://www.reaper.fm/) ,[Ableton Live](https://www.ableton.com/) & [Cakewalk](https://www.cakewalk.com/) - works as expected

## Made With
* [C++](https://isocpp.org/)
* [Juce](https://juce.com/)
* [PampleJuce](https://github.com/sudara/pamplejuce)
* [CMake](https://cmake.org/)
