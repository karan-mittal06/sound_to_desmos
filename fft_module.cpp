#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include "kiss_fft.h"
using namespace std;

const int sampleRate = 44100;
const int window_size = sampleRate / 5; //0.2s per window
const int N_fft = 8192;
const int hop = 2205; //window_size/2 of advance per window

int main(){
    ifstream audioFile;
    audioFile.open("Waveform.wav", ios::binary);
    audioFile.seekg(44);

    ofstream outputFile;
    outputFile.open("Final_equation.txt");

    vector<float> samples;
    int16_t sample;

    while(audioFile.read(reinterpret_cast<char*> (&sample), sizeof(sample))){
        samples.push_back(sample/32768.0f);
    }

    int duration = samples.size()/44100;

    kiss_fft_cfg cfg = kiss_fft_alloc(N_fft, 0, nullptr, nullptr);

    for (int i = 0; i < duration * 5; i++) {
    int start = i * hop;

    // 1. Extract frame
    vector<float> frame(samples.begin() + start, samples.begin() + start + window_size);

    // 2. Apply Hann
    for (int n = 0; n < window_size; n++) {
        float w = 0.5f * (1 - cos(2*M_PI*n / (window_size-1)));
        frame[n] *= w;
    }

    // 3. Copy to FFT input (zero-padded)
    vector<kiss_fft_cpx> fft_in(N_fft), fft_out(N_fft);
    for (int n = 0; n < min(window_size, N_fft); n++) {
        fft_in[n].r = (n < window_size) ? frame[n] : 0.0f;
        fft_in[n].i = 0.0f;
    }

    // 4. Run FFT (using FFT library, not std::fft!)
    kiss_fft(cfg, fft_in.data(), fft_out.data());

    // 5. Compute magnitude spectrum
    vector<float> mag(N_fft/2);
    for(int k = 0; k < N_fft/2; k++){
        mag[k] = sqrt(fft_out[k].r*fft_out[k].r + fft_out[k].i*fft_out[k].i);
    }

    // 6. Find top peaks + convert to (freq, amp)
    int numPeaks = 5;
    string desmos = "f" + to_string(i) + "(t) = ";

    for(int peak = 0; peak < numPeaks; peak++){
    int idx = max_element(mag.begin(), mag.end()) - mag.begin();

    float freq = (idx * sampleRate) / (float)N_fft;

    float amp = mag[idx] / (N_fft/2);

    float phase = atan2(fft_out[idx].i, fft_out[idx].r);

    desmos += to_string(amp) + "*sin(2* pi *" + to_string(freq) + "*t + " + to_string(phase) + ")";
    if(peak < numPeaks-1) desmos += " + ";

    mag[idx] = 0;
}

outputFile << desmos << endl;

}


free(cfg);
audioFile.close();
outputFile.close();

return 0;
}