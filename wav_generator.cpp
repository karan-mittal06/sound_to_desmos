/*
Take a short audio clip (say 0.1–0.5 seconds), do a Fourier Transform, keep only the top terms, and paste into Desmos as a sum of sines.
*/

#include<iostream>
#include<cmath>
#include<fstream>
using namespace std;

const int sampleRate = 44100;

class SineOscillator{
    private:
        float frequency;
        float amplitude;
        float angle = 0.0f;
        float offset = 0.0f;
    public:
        SineOscillator(float freq, float amp) : frequency(freq), amplitude(amp) {
            offset = 2 * M_PI * frequency / sampleRate;
        };

        float Process() {
            auto sample = amplitude * sin(angle);
            angle += offset;
            return sample;
        }
};

int main(){
    int duration = 2; //number of seconds of audio
    int bitDepth = 16;
    auto maxAmplitude = pow(2, bitDepth - 1) - 1;


    SineOscillator sineOscillator1(440, 0.5);
    SineOscillator sineOscillator2(397, 0.5);

    ofstream audioFile;
    audioFile.open("Waveform.wav", ios::binary);

    for(int i = 0; i < sampleRate * duration; i++){
        auto sample =  sineOscillator1.Process() + sineOscillator2.Process();
        int intSample = static_cast<int> (sample * maxAmplitude);
        audioFile.write(reinterpret_cast<char *> (&intSample), 2);
    }

    return 0;
}