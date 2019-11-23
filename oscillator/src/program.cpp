#include <iostream>
#include "RtAudio.h"

#define PULSE_WIDTH 256

int GenerateAudio(
    void *outputBuffer,
    void *inputBuffer,
    unsigned int bufferFrameCount,
    double streamTime,
    RtAudioStreamStatus status,
    void *userData)
{
    double *buffer = reinterpret_cast<double *>(outputBuffer);

    static int frameCounter = 0;

    double val = 0.0;
    for (unsigned int i = 0; i < bufferFrameCount; i++)
    {
        val = frameCounter >= 0 ? 1.0 : -1.0;
        
        *buffer++ = val;
        *buffer++ = val;
        
        if (++frameCounter > PULSE_WIDTH)
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
    
    unsigned int sampleRate = 44100;
    unsigned int bufferFrames = 256;
    double data[2];
    
    try
    {
        dac.openStream(
            &parameters,
            NULL,
            RTAUDIO_FLOAT64,
            sampleRate,
            &bufferFrames,
            &GenerateAudio,
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
