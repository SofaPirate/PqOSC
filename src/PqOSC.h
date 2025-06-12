#ifndef __PQ_OSC___
#define __PQ_OSC___

#include <PqCore.h>
#include <MicroOsc.h>
#include <MicroOscSlip.h>
#include <MicroOscUdp.h>

namespace pq
{
    // ===============================================================
    class OSCIn; // forward declaration
    HybridArrayList<OSCIn *, PLAQUETTE_MAX_UNITS> _PqoscInList;

    // ===============================================================
    class OSCIn : public AnalogSource
    {

       // friend class OSCSlip; // Grant access

    private:
        const char *_address;
        float _value;
        MicroOsc &_microOsc;

        protected:
        void begin() {
            _PqoscInList.add(this);
        }

    public:
        const char *address()
        {
            return _address;
        }

        OSCIn(MicroOsc &osc, const char *address) : _microOsc(osc), _address(address)
        {
            
        }

        float put(float f)
        {
            _value = f;
            return get();
        }

        float get()
        {
            return _value;
        }
    };

    // ===============================================================

    void _PqOSCMessageCallback(MicroOscMessage &message)
    {
        // SHOULD EVENTUALLY CHECK THE SOURCE TO DISTINGUISH MESSAGES FROM DIFFERENT MICROOSC INSTANCES
        for (size_t i = 0; i != _PqoscInList.size(); i++)
        {
            if (message.checkOscAddress(_PqoscInList[i]->address()))
            {
                _PqoscInList[i]->put(message.nextAsFloat());
            }
        }
    };

    // ===============================================================
    /*
    class PqOSC : public Unit, protected MicroOsc
    {

        friend class OSCIn;  // Grant access
        friend class OSCOut; // Grant access

    protected:
        MicroOsc &_microOsc;

        PqOSC(MicroOsc &microOsc) : _microOsc(microOsc)
        {
        }

        void _add(OSCIn *component)
        {
            _PqoscInList.add(component);
        }

        void step() override
        {
            _microOsc.onOscMessageReceived(_PqOSCMessageCallback);
        }
    };
*/
    // ===============================================================

    template <const size_t MICRO_OSC_IN_SIZE>
class OSCSlip : public Unit, public MicroOscSlip<MICRO_OSC_IN_SIZE>
{
private:
    HardwareSerial &_serial;
    int _baud;

protected:
  void step() override
    {
            this->onOscMessageReceived(_PqOSCMessageCallback);
    }

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
    OSCSlip(HardwareSerial &serial, int baud)
        : MicroOscSlip<MICRO_OSC_IN_SIZE>(serial), _serial(serial), _baud(baud)
    {
    }

   
};

    // ===============================================================

    // ===============================================================

    class OSCOut : public Unit
    {
    private:
        MicroOsc &_microOsc;
        const char *_address;
        unsigned long _start;
        unsigned long _interval;
        float _value;

    public:
        OSCOut(MicroOsc &osc, const char *address, unsigned long interval = 50) : _microOsc(osc), _address(address), _interval(interval)
        {
        }

        float put(float f)
        {
            _value = f;
            return get();
        }

        float get()
        {
            return _value;
        }

        void begin()
        {
            _start = millis();
        }

        void step()
        {
            if (millis() - _start >= _interval)
            {
                _start = millis();

                _microOsc.sendFloat(_address, _value);
            }
        }
    };
}
#endif