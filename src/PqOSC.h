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
    HybridArrayList<OSCIn *, PLAQUETTE_MAX_UNITS> _PqOSCInList;

    // ===============================================================
    class OSCIn : public Unit
    {

        // friend class OSCSlip; // Grant access

    private:
        const char * _address;
        float _value;
        MicroOsc &_microOsc;

    protected:
        virtual void begin() override
        {
            _PqOSCInList.add(this);
        }

        virtual void step() override
        {
            
        }

    public:
        const char *address()
        {
            return _address;
        }

        OSCIn(MicroOsc &osc, const char *address, Engine& engine = Engine::primary()) : _microOsc(osc), _address(address), Unit(engine) 
        {
        }

         void set(float f)
        {
            _value = f;
        }

        float get()
        {
            return _value;
        }

    private:
        // DISABLE THE PUT
        float put(float f)
        {
            //_value = f;
            return f;
        }
    };

    // ===============================================================

    void _PqOSCMessageCallback(MicroOscMessage &message)
    {
        // SHOULD EVENTUALLY CHECK THE SOURCE TO DISTINGUISH MESSAGES FROM DIFFERENT MICROOSC INSTANCES
        for (size_t i = 0; i != _PqOSCInList.size(); i++)
        {
            // SHOULD CONVERT INT TO FLOAT
            if (message.checkOscAddress(_PqOSCInList[i]->address()))
            {
                _PqOSCInList[i]->set(message.nextAsFloat());
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
    class OSCSSLIP : public Unit, public MicroOscSlip<MICRO_OSC_IN_SIZE>
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
        OSCSSLIP(HardwareSerial &serial, int baud)
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
        OSCOut(MicroOsc &osc, const char *address, float minimumInterval = 0.05,  Engine& engine = Engine::primary()) : _microOsc(osc), _address(address), Unit(engine) 
        {
            _interval = floor(minimumInterval * 1000.0f);
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