#include "dsp.h"
#include "AudioFile.h"

#define NZEROS 4
#define NPOLES 4
#define GAIN   1.836290157e+05

// This is a four-pole butterworth filter with a corner frequency of 700 Hz.
void butterworth(AudioFile<double>::AudioBuffer& buffer) {
    int nChannels = buffer.size();
    // We assume for convenience that each channel has the same number of samples.
    int nSamples;
    if (nChannels > 0)
        nSamples = buffer[0].size();

    // The coefficients for the following loop were calculated using Dr A.J. Fisher's mkfilter package,
    // which you can access at https://www-users.cs.york.ac.uk/~fisher/mkfilter/. 
    static double xv[NZEROS+1], yv[NPOLES+1];
    for (int channel = 0; channel < nChannels; ++channel) {
        for (int i = 0; i < nSamples; ++i) {
            xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4]; 
            xv[4] = buffer[channel][i] / GAIN;
            yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; yv[3] = yv[4]; 
            yv[4] =   (xv[0] + xv[4]) + 4 * (xv[1] + xv[3]) + 6 * xv[2]
                    + ( -0.7704798179 * yv[0]) + (  3.2826777632 * yv[1])
                    + ( -5.2517221303 * yv[2]) + (  3.7394370527 * yv[3]);
            buffer[channel][i] = yv[4];
        }
    }
    return;
}

// This is the simplest kind of filter from a programming point of view.
// We set y[i] = 0.5 * (x[i] + x[i-1]). 
void simplest(AudioFile<double>::AudioBuffer& buffer) {
    int nChannels = buffer.size();
    // We assume for convenience that each channel has the same number of samples.
    int nSamples;
    if (nChannels > 0)
        nSamples = buffer[0].size();

    const double lambda = 0.5; // arbitrary value between 0 and 1

    for (int channel = 0; channel < nChannels; ++channel) {
        for (int i = 1; i < nSamples; ++i) {
#ifdef __SSE2__
            double* current = &buffer[channel][i];
            double* previous = &buffer[channel][i-1];
            asm ( "movsd (%1), %%xmm0\n\t"
                  "movsd (%2), %%xmm1\n\t"
                  "addsd %%xmm1, %%xmm0\n\t"
                  "movsd (%3), %%xmm1\n\t"
                  "mulsd %%xmm1, %%xmm0\n\t"
                  "movsd %%xmm0, (%0)\n\t"
                : "=r" (current)
                : "r" (current), "r" (previous), "r" (&lambda)
                : "xmm0", "xmm1", "memory"
            );
#else
            buffer[channel][i] = 0.5 * (buffer[channel][i] + buffer[channel][i-1]);
#endif
        }
    }
    return;
}

/* Digital filter designed by mkfilter/mkshape/gencode   A.J. Fisher
   Command line: /www/usr/fisher/helpers/mkfilter -Bu -Lp -o 4 -a 1.3605442177e-02 0.0000000000e+00 -l */
