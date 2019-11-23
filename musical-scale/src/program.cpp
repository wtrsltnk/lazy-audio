#include <iostream>
#include <thread>
#include <chrono>
#include "RtAudio.h"

struct Note
{
    double frequency;
    char const *name;
};

struct Note notes[] = {
    { 65.41, "c2", },
    { 69.30, "c#2", },
    { 73.42, "D2", },
    { 77.78, "D#2", },
    { 82.41, "E2", },
    { 87.31, "F2", },
    { 92.50, "F#2", },
    { 98.00, "G2", },
    { 103.83, "G#2", },
    { 110.00, "A2", },
    { 116.54, "A#2", },
    { 123.47, "B2", },
};

int GenerateSquareWave(
    void *outputBuffer,
    void *inputBuffer,
    unsigned int bufferFrameCount,
    double streamTime,
    RtAudioStreamStatus status,
    void *userData)
{
    double *buffer = reinterpret_cast<double *>(outputBuffer);
    double *data = reinterpret_cast<double *>(userData);

    static int frameCounter = 0;

    for (unsigned int i = 0; i < bufferFrameCount; i++)
    {
        *buffer++ = *buffer++ = frameCounter >= 0 ? data[1] : -data[1];
        
        if (++frameCounter > (data[0] / 2.0))
        {
            frameCounter = -frameCounter;
        }
    }
    
    return 0;
}

int main(
    int argc,
    char *argv[])
{
    unsigned int sampleRate = 44100;
    unsigned int bufferFrames = 256;
    
    RtAudio dac;
    RtAudio::StreamParameters parameters;
    
    try
    {
        parameters.deviceId = dac.getDefaultOutputDevice();
        parameters.nChannels = 2;
        parameters.firstChannel = 0;
    }
    catch (RtAudioError& e)
    {
        e.printMessage();
        return 0;
    }
    
    double data[2] = { sampleRate / notes[0], 1.0 };
    
    try
    {
        dac.openStream(
            &parameters,
            NULL,
            RTAUDIO_FLOAT64,
            sampleRate,
            &bufferFrames,
            &GenerateSquareWave,
            reinterpret_cast<void *>(&data));
            
        dac.startStream();
    }
    catch (RtAudioError& e)
    {
        e.printMessage();
        return 0;
    }

    for (int i = 0; i < 12; i++)
    {
        data[0] = sampleRate / notes[i].frequency;
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    
    try
    {
        dac.stopStream();
    }
    catch (RtAudioError& e)
    {
        e.printMessage();
    }
    
    if (dac.isStreamOpen())
    {
        dac.closeStream();
    }
    
    return 0;
}
