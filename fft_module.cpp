#include <iostream>
#include <fstream>
using namespace std;

int main(){
    ifstream audioFile;
    audioFile.open("Waveform.wav", ios::binary);

    vector<vector<float>> samples;
    int16_t sample;

    while(audioFile.read(reinterpret_cast<char*> (&sample), sizeof(sample))){
        samples.push_back({sample/32768.0f,0});
    }
}