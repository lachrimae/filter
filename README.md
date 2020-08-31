# Filter
This is a sort of "Hello world" of DSP, C++ and assembly. It compiles into an executable that runs one of two filtering processes. The first one, "butter", runs a 4-pole Butterworth filter with a corner frequency of 700 Hz. The mathematical function required was calculated using A.J. Fisher's mkfilter package, accessible at [this webpage](https://www-users.cs.york.ac.uk/~fisher/mkfilter/).

The second filtering process is arguably the most elementary filter possible of its kind. It sets the present output sample to the average of the two most recent input samples. I used SSE2 assembly instructions to compute this.

My code is mostly in `dsp.cpp`, and I use the [AudioFile](https://github.com/adamstark/AudioFile/) library to manage I/O.

# Build instructions
```bash
mkdir build
cd build
cmake ..
make
```

Run instructions:
```bash
filter butter /path/to/audio.wav
```
Or
```bash
filter simple /path/to/audio.wav
```
