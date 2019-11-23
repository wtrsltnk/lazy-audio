#ifndef ENVELOPE_H 
#define ENVELOPE_H

class Envelope
{
    bool on;
    double value;
    double eventTime;
    double lastStreamTime;
public:
    Envelope();

    void NoteOn();
    void NoteOff();
    void Tick(double streamTime);
    
    bool IsOn() const;
    double Value() const;
    
    double attackTime;
    double decayTime;
    double sustainLevel;
    double releaseTime;
};

#endif // ENVELOPE_H