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
        int frequency;
        int amplitude;
        float angle = 0.0f;
        float offset = 0.0f;
    public:
        SineOscillator(int freq, int amp) : frequency(freq), amplitude(amp) {
            offset = 2 * M_PI * frequency / sampleRate;
        };

        float Process() {
            auto smaple = amplitude * sin(angle);
            angle += offset;
        }
};

int main(){
    int x;
}