#include <stdlib.h>
#include "AudioFile.h"

float* reverseArray(float* arr, int N) {
    float* rev = new float[N];
    for (int i = 0; i < N; i++) {
        rev[N-i-1] = arr[i];
    }
    return rev;
}

int main(int argc, char* argv[]) {
    AudioFile<double> file;
    file.load("hiddensound.wav");
    AudioFile<double>::AudioBuffer buffer;
    buffer.resize(1);
    size_t N = file.getNumSamplesPerChannel();
    buffer[0].resize(N);
    for (size_t i = 0; i < N; i++) {
        buffer[0][i] = file.samples[0][N-i-1];
    }
    file.setAudioBuffer(buffer);
    file.save("mysterysound.wav");
    return 0;
}
