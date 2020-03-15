#include <iostream>
#include <memory>
#include "AudioFile.h"
#include "dsp.h"

using namespace std;

int errorMsg();
string suffix(string fileName);
string prefix(string fileName);

void copyFromFile(AudioFile<double>& fileObj, string fileLoc, AudioFile<double>::AudioBuffer& buffer);

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
    AudioFile<double>::AudioBuffer buffer;
    copyFromFile(fileInterface, inputName, buffer);

    // Run each channel through a Butterworth filter
    butterworth(buffer);

    // Finish our work with the buffer
    bool ok = fileInterface.setAudioBuffer(buffer);
    // Time to terminate the program
    if (ok) {
        fileInterface.save(outputName, AudioFileFormat::Wave);
        cout << "Complete!" << endl;
        return 0;
    }
    else {
        cerr << "Something went wrong when I tried to save the audio buffer!" << endl;
        return 1;
    }
}

int errorMsg() {
    cerr << "Usage: specify a .wav file and I will filter it, outputting .new.wav" << endl;
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

void copyFromFile(AudioFile<double>& fileObj, string fileLoc, AudioFile<double>::AudioBuffer& buffer) {
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
