#ifndef __PQ_OSC___
#define __PQ_OSC___

#include <PqCore.h>
#include <MicroOsc.h>
#include <MicroOscSlip.h>
#include <MicroOscUdp.h>

namespace pq
{

    enum {
        OSC_VALUE,
        OSC_EVENT
    };

    // ===============================================================
    class OscIn; // forward declaration
    extern HybridArrayList<OscIn *, PLAQUETTE_MAX_UNITS> _PqOscInList;

    // ===============================================================
    class OscIn : public Unit
    {

        // friend class OSCSlip; // Grant access

    private:
        const char * _address;
        float _value;
        MicroOsc &_microOsc;

    protected:
        void begin() override
        {
            _PqOscInList.add(this);
        }

        void step() override
        {
            
        }

    public:
        const char *address()
        {
            return _address;
        }

        OscIn(MicroOsc &osc, const char *address, Engine& engine = Engine::primary()) : _microOsc(osc), _address(address), Unit(engine) 
        {
        }

        void set(float f)
        {
            _value = f;
        }

        float get() override
        {
            return _value;
        }
    private:
        // DISABLE THE PUT
        float put(float f) override
        {
            //_value = f;
            return f;
        }
    };

    // ===============================================================

    void _PqOSCMessageCallback(MicroOscMessage &message);

    // ===============================================================
    /*
    class PqOSC : public Unit, protected MicroOsc
    {

        friend class OscIn;  // Grant access
        friend class OscOut; // Grant access

    protected:
        MicroOsc &_microOsc;

        PqOSC(MicroOsc &microOsc) : _microOsc(microOsc)
        {
        }

        void _add(OscIn *component)
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
    class OscSlip : public Unit, public MicroOscSlip<MICRO_OSC_IN_SIZE>
    {
    private:
        HardwareSerial &_serial;
        int _baud;

    protected:
        void step() override
        {
            this->onOscMessageReceived(_PqOSCMessageCallback);
        }

        void begin() override
        {
            _serial.begin(_baud);
        }

        float get() override
        {
            return 0;
        }

    public:
        // Constructor with default value for _iic_address
        OscSlip(HardwareSerial &serial, int baud, Engine& engine = Engine::primary())
            : MicroOscSlip<MICRO_OSC_IN_SIZE>(serial), _serial(serial), _baud(baud), Unit(engine)
        {
        }
    };

    // ===============================================================

    // ===============================================================

    class OscOut : public Unit
    {
    private:
        MicroOsc &_microOsc;
        const char *_address;
        float _value;

        char _typeTag;

        bool _needToSend  : 1;
        uint8_t _mode     : 1;
        uint8_t _unused   : 6;

    public:
        OscOut(MicroOsc &osc, const char *address, Engine& engine = Engine::primary()) 
            : OscOut(osc, address, 'f', engine)
        {}

        OscOut(MicroOsc &osc, const char *address, char typeTag, Engine& engine = Engine::primary()) 
            : Unit(engine), _microOsc(osc), _address(address), _typeTag(typeTag), _needToSend(false)
        {}

        float put(float f) override
        {
            _value = f;
            _needToSend = true;
            return get();
        }

        float get() override
        {
            return _value;
        }

        virtual void mode(uint8_t mode);

        virtual uint8_t mode() const { return _mode; }

    protected:
        void begin() override;

        void step() override
        {
            if (_needToSend)
            {
                _sendMessage();
                _needToSend = false;
            }
        }

        void _sendMessage();

    };
}
#endif