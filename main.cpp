#include <iostream>
#include <memory>
#include "libs/AudioFile.h"

using namespace std;

int CUTOFF_FREQ = 500.0;
int FILTER_ORD  = 4;

void butterworth(int nSamples, AudioFile<double>::AudioBuffer& buffer) {
    for (auto& channel : buffer) {
        for (int i = 1; i < nSamples; i++) {
            channel[i] = 0.5 * channel[i] + channel[i-1];
        }
    }
    return;
}

int errorMsg() {
    cout << "Usage: specify a .wav file and I will filter it, outputting .old.wav" << endl;
    return 1;
}

// Presumes that the input length is at least 4.
string suffix(string fileName) {
    int fLast = fileName.length() - 1;
    return fileName.substr(fLast - 3, 4);
}

// Presumes that the input length is at least 4.
string prefix(string fileName) {
    return fileName.substr(0, fileName.length() - 4);
}

void copyFromFile(AudioFile<double>& fileObj, string& fileLoc, AudioFile<double>::AudioBuffer& buffer) {
    fileObj.load(fileLoc);

    // format the buffer
    int nChannels = fileObj.getNumChannels();
    int nSamples  = fileObj.getNumSamplesPerChannel();
    buffer.resize(nChannels);
    for (auto& channel : buffer) {
        channel.resize(nSamples);
    }

    // Fill the buffer
    for (int i = 0; i < nSamples; i++) {
        for (int channel = 0; channel < nChannels; channel++) {
            buffer[channel][i] = fileObj.samples[channel][i];
        }
    }
    return;
}


int main(int argc, char** argv)
{
    cout << "I am the simplest filter!" << endl;
    if (argc != 2)
        return errorMsg();

    string inputName = argv[1];
    if (inputName.length() < 4)
        return errorMsg();
    if (suffix(inputName) != ".wav")
        return errorMsg();

    string outputName = prefix(inputName) + ".new.wav";
    AudioFile<double> fileInterface;

    // Initialize the buffer
    //auto buffer = make_shared<AudioFile<double>::AudioBuffer>;
    AudioFile<double>::AudioBuffer* buffer;
    buffer = new AudioFile<double>::AudioBuffer;
    copyFromFile(fileInterface, inputName, *buffer);

    // Run each channel through a Butterworth filter
    butterworth
    (
        fileInterface.getNumSamplesPerChannel(),
        *buffer
    );

    // Finish our work with the buffer
    bool ok = fileInterface.setAudioBuffer(*buffer);
    delete buffer;
    // Time to terminate the program
    if (ok) {
        fileInterface.save(outputName, AudioFileFormat::Wave);
        cout << "Complete!";
        return 0;
    }
    else {
        cerr << "Something went wrong when I tried to save the audio buffer!";
        return 1;
    }
}
