#ifndef __PQ_OSC___
#define __PQ_OSC___

#include <PqCore.h>
#include <MicroOsc.h>
#include <MicroOscSlip.h>
#include <MicroOscUdp.h>

template <const size_t MICRO_OSC_IN_SIZE>
class PqOSCSlip : public Unit, public MicroOscSlip<MICRO_OSC_IN_SIZE>
{
private:
    HardwareSerial &_serial;
    int _baud;

protected:
    void begin()
    {
        _serial.begin(_baud);
    }

    float get()
    {
        return 0;
    }

public:
    // Constructor with default value for _iic_address
    PqOSCSlip(HardwareSerial &serial, int baud)
        : MicroOscSlip<MICRO_OSC_IN_SIZE>(serial), _serial(serial), _baud(baud)
    {
    }

   
};

class PqOscOut : public Unit
{
private:
    MicroOsc &_osc;
    const char * _address;
    unsigned long _start;
    unsigned long _interval;
    float _value;

public:
    PqOscOut(MicroOsc &osc, const char * address, unsigned long interval = 50) : _osc(osc), _address(address), _interval(interval)
    {
    }

    float put(float f) {
        _value = f;
        return get();
    }

    float get()
    {
        return _value;
    }

    void begin() {
        _start = millis();
    }

    void step()
    {
        if ( millis() - _start >= _interval ) {
            _start = millis();

            _osc.sendFloat(_address, _value);
        }
    }
};

#endif