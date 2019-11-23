#include <iostream>
#include <conio.h>
#include "RtAudio.h"
#include "envelope.h"

static Envelope envelope;

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

    envelope.Tick(streamTime);
    
    for (unsigned int i = 0; i < bufferFrameCount; i++)
    {
        *buffer++ = *buffer++ = frameCounter >= 0 ? envelope.Value() : -envelope.Value();
        
        if (++frameCounter > (data[0] / 2.0))
        {
            frameCounter = -frameCounter;
        }
    }
    
    return 0;
}

int main()
{
    double frequency = 73.42;
    unsigned int sampleRate = 44100;
    unsigned int bufferFrames = 256;
    
    envelope.attackTime = 0.5;
    envelope.decayTime = 0.1;
    envelope.sustainLevel = 0.8;
    envelope.releaseTime = 1.0;
    
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
    
    double data[2] = { sampleRate / frequency, 0.0 };
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

    std::cout << "\nPress <spacebar> to toggle play ... press <q> to quit.\n";
    
    while (getch() != 'q')
    {
        if (envelope.IsOn())
        {
            envelope.NoteOff();
        }
        else
        {
            envelope.NoteOn();
        }
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
