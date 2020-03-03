#include <iostream>
#include "libs/AudioFile.h"

using namespace std;

void process(int nSamples, int nChannels, AudioFile<double>::AudioBuffer &input) {
    for (int i = 1; i < nSamples; i++) {
        for (int channel = 0; channel < nChannels; channel++) {
            input[channel][i] = 0.5 * (input[channel][i] + input[channel][i-1]);
        }
    }
    return;
}

int main(int argc, char** argv)
{
    cout << "I will be a filter soon!" << endl;
    if (argc != 2) {
        cout << "Usage: specify a .wav file and I will filter it, outputting .old.wav" << endl;
        return 1;
    }

    std::string f = argv[1];
    if (f.length() < 4) {
        cout << "Usage: specify a .wav file and I will filter it, outputting .old.wav" << endl;
        return 1;
    }

    int fLast = f.length() - 1;
    std::string suffix = f.substr(fLast - 3, 4);
    std::string prefix = f.substr(0, f.length() - 4);
    std::string finalName = prefix + ".new.wav";
    if (suffix != ".wav") {
        cout << "Usage: specify a .wav file and I will filter it, outputting .old.wav" << endl;
        return 1;
    }

    AudioFile<double> audioFile;
    audioFile.load(f);

    AudioFile<double>::AudioBuffer finalAudio;
    int nChannels = audioFile.getNumChannels();
    int nSamples  = audioFile.getNumSamplesPerChannel();
    finalAudio.resize(nChannels);
    for (int i = 0; i < nChannels; i++) {
        finalAudio[i].resize(nSamples);
    }

    for (int i = 0; i < nSamples; i++) {
        for (int channel = 0; channel < nChannels; channel++) {
            finalAudio[channel][i] = audioFile.samples[channel][i];
        }
    }

    process(nSamples, nChannels, finalAudio);

    bool ok = audioFile.setAudioBuffer(finalAudio);

    if (ok) {
        audioFile.save(finalName, AudioFileFormat::Wave);
        return 0;
    }
    else {
        return 1;
    }
}
