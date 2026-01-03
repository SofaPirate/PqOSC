#ifndef __PQ_OSC___
#define __PQ_OSC___

#include <PqCore.h>
#include <MicroOsc.h>
#include <MicroOscSlip.h>
#include <MicroOscUdp.h>

namespace pq
{

    class OscIn; // forward declaration
    HybridArrayList<OscIn *, PLAQUETTE_MAX_UNITS> _PqOscInList;

    // OscIn class ----------------------------------------------------------- |
    class OscIn : public Unit
    {

        // friend class OSCSlip; // Grant access

    private:
        const char *_address;
        float _value;
        MicroOsc &_microOsc;

    protected:
        virtual void begin() override
        {
            _PqOscInList.add(this);
        }

        virtual void step() override
        {
        }

    public:
        const char *address()
        {
            return _address;
        }

        OscIn(MicroOsc &osc, const char *address, Engine &engine = Engine::primary()) : _microOsc(osc), _address(address), Unit(engine)
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
    // ----------------------------------------------------------------------- |

    // _PqOSCMessageCallback ------------------------------------------------- |
    void _PqOSCMessageCallback(MicroOscMessage &message)
    {
        // SHOULD EVENTUALLY CHECK THE SOURCE TO DISTINGUISH MESSAGES FROM DIFFERENT MICROOSC INSTANCES
        for (size_t i = 0; i != _PqOscInList.size(); i++)
        {
            // SHOULD CONVERT INT TO FLOAT
            if (message.checkOscAddress(_PqOscInList[i]->address()))
            {
                _PqOscInList[i]->set(message.nextAsFloat());
            }
        }
    };
    // ---------------------------------------------------------------------- |

    // OscSlip class -------------------------------------------------------- |
    template <const size_t MICRO_OSC_IN_SIZE>
    class OscSlip : public Unit, public MicroOscSlip<MICRO_OSC_IN_SIZE>
    {
    private:


    protected:
        void step() override
        {
            this->onOscMessageReceived(_PqOSCMessageCallback);
        }

        void begin()
        {
           
        }

        float get()
        {
            return 0;
        }

    public:
        // Constructor with default value for _iic_address
        OscSlip(Stream &stream, Engine &engine = Engine::primary())
            : MicroOscSlip<MICRO_OSC_IN_SIZE>(stream), Unit(engine)
        {
        }
    };
    // ----------------------------------------------------------------------- |

    // OscUdp class ---------------------------------------------------------- |
    // OSC UDP Communication
    template <const size_t MICRO_OSC_IN_SIZE>
    class OscUdp : public Unit, public MicroOscUdp<MICRO_OSC_IN_SIZE>
    {
    private:
    protected:
        void step() override
        {
            this->onOscMessageReceived(_PqOSCMessageCallback);
        }

        void begin()
        {
        }

        float get()
        {
            return 0;
        }

    public:
        OscUdp(UDP &udp, Engine &engine = Engine::primary())
            : MicroOscUdp<MICRO_OSC_IN_SIZE>(udp), Unit(engine)
        {
        }

        OscUdp(UDP &udp, IPAddress destinationIp, unsigned int destinationPort, Engine &engine = Engine::primary())
            : MicroOscUdp<MICRO_OSC_IN_SIZE>(udp, destinationIp, destinationPort), Unit(engine)
        {
        }
    };
    // ----------------------------------------------------------------------- |

    // OscOut class ---------------------------------------------------------- |
    class OscOut : public Unit
    {
    private:
        MicroOsc &_microOsc;
        const char *_address;
        unsigned long _start;
        bool _needToSend = false;
        float _value;

    public:
        OscOut(MicroOsc &osc, const char *address, Engine &engine = Engine::primary()) : _microOsc(osc), _address(address), Unit(engine)
        {
        }

        float put(float f)
        {
            _value = f;
            _needToSend = true;
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
            if (_needToSend)
            {
                _microOsc.sendFloat(_address, _value);
                _needToSend = false;
            }
        }
    };
    // ----------------------------------------------------------------------- |
}
#endif