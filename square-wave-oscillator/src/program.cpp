#include <iostream>
#include "RtAudio.h"

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
            &GenerateSquareWave,
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
