#include <iostream>
#include "RtAudio.h"

int GenerateSawtooth(
    void *outputBuffer,
    void *inputBuffer,
    unsigned int bufferFrameCount,
    double streamTime,
    RtAudioStreamStatus status,
    void *userData)
{
    double *buffer = reinterpret_cast<double *>(outputBuffer);
    double *data = reinterpret_cast<double *>(userData);

    static double value = -data[1];
    
    for (unsigned int i = 0; i < bufferFrameCount; i++)
    {
        *buffer++ = *buffer++ = value;
        
        value += (2.0f / data[0]);
        
        if (value >= 1.0)
        {
            value = -1.0f;
        }
    }
    
    return 0;
}

int main(
    int argc,
    char *argv[])
{
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
    
    double frequency = 73.42;
    unsigned int sampleRate = 44100;
    unsigned int bufferFrames = 256;
    double data[2] = { sampleRate / frequency, 1.0 };
    
    try
    {
        dac.openStream(
            &parameters,
            NULL,
            RTAUDIO_FLOAT64,
            sampleRate,
            &bufferFrames,
            &GenerateSawtooth,
            reinterpret_cast<void *>(&data));
            
        dac.startStream();
    }
    catch (RtAudioError& e)
    {
        e.printMessage();
        return 0;
    }

    char input;
    std::cout << "\nPlaying ... press <enter> to quit.\n";
    std::cin.get(input);
    
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
