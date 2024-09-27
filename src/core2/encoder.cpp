#include "encoder.h"

Encoder *encoderInstance = nullptr; // Global pointer for ISR access

Encoder::Encoder(int pinA, int pinB)
    : _pinA(pinA), _pinB(pinB), _position(0), _lastA(false), _lastB(false), _enabled(true)
{
    encoderInstance = this; // Set global instance for ISR access
}

void Encoder::begin()
{
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);

    _lastA = digitalRead(_pinA);
    _lastB = digitalRead(_pinB);

    attachInterrupt(digitalPinToInterrupt(_pinA), []()
                    { encoderInstance->handleInterruptA(); }, CHANGE);
}

int32_t Encoder::getPosition()
{
    return _position;
}

void Encoder::setPosition(int32_t position)
{
    _position = position;
}

void Encoder::handleInterruptA()
{
    if (_enabled)
    {
        bool A = digitalRead(_pinA);
        bool B = digitalRead(_pinB);

        if (A != _lastA)
        {
            if (A == B)
            {
                _position++;
            }
            else
            {
                _position--;
            }
        }
        _lastA = A;
    }
}

void Encoder::handleInterruptB()
{
    if (_enabled)
    {
        bool A = digitalRead(_pinA);
        bool B = digitalRead(_pinB);

        if (B != _lastB)
        {
            if (A == B)
            {
                _position--;
            }
            else
            {
                _position++;
            }
        }
        _lastB = B;
    }
}

void Encoder::disable()
{
    _enabled = false;
}
void Encoder::enable()
{
    _enabled = true;
}