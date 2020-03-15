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
    // Work through guards to make sure our input is correct.
    if (argc != 3)
        return errorMsg();

    string filterType = argv[1];
    bool doSimple = false;
    if (filterType == "simple")
        doSimple = true;
    else if (filterType == "butter")
        doSimple = false;
    else
        return errorMsg();

    // In a real codebase, one might consider checking
    // that the input file is properly formatted as 
    // a .wav file, rather than just checking for 
    // the .wav extension.
    string inputName = argv[2];
    if (inputName.length() < 4)
        return errorMsg();
    if (suffix(inputName) != ".wav")
        return errorMsg();

    string outputName = prefix(inputName) + ".new.wav";
    AudioFile<double> fileInterface;

    AudioFile<double>::AudioBuffer buffer;
    copyFromFile(fileInterface, inputName, buffer);

    if (doSimple)
        simplest(buffer);
    else
        butterworth(buffer);

    bool ok = fileInterface.setAudioBuffer(buffer);
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
    cerr << "Usage: filter [simple|butter] [file]\n"
         << "Specify a .wav file and I will filter it, outputting .new.wav"
         << endl;
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

// load the file contents into the buffer.
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
