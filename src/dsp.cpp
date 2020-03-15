#include "dsp.h"
#include "AudioFile.h"

void butterworth(AudioFile<double>::AudioBuffer& buffer) {
    int nChannels = buffer.size();
    // We assume for convenience that each channel has the same number of samples.
    int nSamples;
    if (nChannels > 0)
        nSamples = buffer[0].size();
    for (int channel = 0; channel < nChannels; ++channel) {
        for (int i = 1; i < nSamples; ++i) {
            double current = buffer[channel][i];
            double previous = buffer[channel][i-1];
            double* currentLoc = &current;
            const double lambda = 0.5;
            asm ( "movsd (%1), %%xmm0\n\t"
                  "movsd (%2), %%xmm1\n\t"
                  "addsd %%xmm1, %%xmm0\n\t"
                  "movsd (%3), %%xmm1\n\t"
                  "mulsd %%xmm1, %%xmm0\n\t"
                  "movsd %%xmm0, (%0)\n\t"
                : "=r" (currentLoc)
                : "r" (currentLoc), "r" (&previous), "r" (&lambda)
                : "xmm0", "xmm1", "memory"
            );
            buffer[channel][i] = current;
        }
    }
    return;
}

void simplest(AudioFile<double>::AudioBuffer& buffer) {
    int nChannels = buffer.size();
    // We assume for convenience that each channel has the same number of samples.
    int nSamples;
    if (nChannels > 0)
        nSamples = buffer[0].size();
    for (int channel = 0; channel < nChannels; ++channel) {
        for (int i = 1; i < nSamples; ++i) {
            double current = buffer[channel][i];
            double previous = buffer[channel][i-1];
            double* currentLoc = &current;
            const double lambda = 0.5;
#ifdef __FMA__
            cout << "FMA" << endl;
            asm ( "fldq %1;"
                  "fldq %2;"
                  "faddp;"
                  "fldq $0x00000000;" // Hardcoded float = 0.5
                  "fmul;"
                : "=r" (current)
                : "r" (buffer[channel][i - 1]), "r" (buffer[channel][i])
            );
#else
            asm ( "movsd (%1), %%xmm0\n\t"
                  "movsd (%2), %%xmm1\n\t"
                  "addsd %%xmm1, %%xmm0\n\t"
                  "movsd (%3), %%xmm1\n\t"
                  "mulsd %%xmm1, %%xmm0\n\t"
                  "movsd %%xmm0, (%0)\n\t"
                : "=r" (currentLoc)
                : "r" (currentLoc), "r" (&previous), "r" (&lambda)
                : "xmm0", "xmm1", "memory"
            );
#endif
            buffer[channel][i] = current;
        }
    }
    return;
}
