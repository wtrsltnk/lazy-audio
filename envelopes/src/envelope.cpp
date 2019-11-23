#include "envelope.h"

Envelope::Envelope() : on(false), value(0.0), eventTime(0.0), lastStreamTime(0.0) {}

void Envelope::NoteOn()
{
    eventTime = lastStreamTime;
    on = true;
}

void Envelope::NoteOff()
{
    eventTime = lastStreamTime;
    on = false;
}

void Envelope::Tick(double streamTime)
{
    lastStreamTime = streamTime;
    
    if (on && lastStreamTime < eventTime + attackTime)
    {
        // We are in attack
        value = (lastStreamTime - eventTime) / attackTime;
    }
    else if (on && lastStreamTime < eventTime + attackTime + decayTime)
    {
        // We are in decay
        auto progress = (lastStreamTime - eventTime - attackTime) / decayTime;
        value = sustainLevel + ((1.0 - sustainLevel) * (1.0 - progress));
    }
    else if (on && lastStreamTime >= eventTime + attackTime + decayTime)
    {
        // We are in sustain
        value = sustainLevel;
    }
    else if (!on && value > 0.0 && lastStreamTime < eventTime + releaseTime)
    {
        // We are in release
        auto progress = (lastStreamTime - eventTime) / releaseTime;
        value = sustainLevel * (1.0 - progress);
    }
    else if (!on)
    {
        value = 0.0;
    }
}

bool Envelope::IsOn() const { return on; }
double Envelope::Value() const { return value / 2.0; }
