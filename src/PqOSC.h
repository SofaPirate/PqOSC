#ifndef __PQ_OSC___
#define __PQ_OSC___

#include <PqCore.h>
#include <MicroOsc.h>
#include <MicroOscSlip.h>
#include <MicroOscUdp.h>

namespace pq
{

    class OscIn; // forward declaration

    // OscIn class ----------------------------------------------------------- |
    class OscIn : public Unit
    {
    private:
        // Shared static container containing all units. Static because it is shared between all OscIn units.
        static HybridArrayList<OscIn*, PLAQUETTE_MAX_UNITS>& oscInList();

    private:
        const char *_address;
        float _value;
        MicroOsc &_microOsc;
        bool _messageReceived : 1;
        bool _valueUpdated : 1;
        uint8_t _unused : 6;

    protected:
        void begin() override
        {
            _messageReceived = _valueUpdated = false;
        }

        void step() override {
            _valueUpdated = _messageReceived;
            _messageReceived = false;
        }

        /// Returns true iff an event of a certain type has been triggered.
        bool eventTriggered(EventType eventType) override {
            switch (eventType) {
            case EVENT_UPDATE: return updated();
            default:           return Unit::eventTriggered(eventType);
            }
        }    

        void receive(float f)
        {
<<<<<<< HEAD
=======
            _value = f;
            _messageReceived = true;
>>>>>>> a57b7287a1db11f3089ab202e8cd376b40c4db29
        }

    public:
        const char *address()
        {
            return _address;
        }

<<<<<<< HEAD
        OscIn(MicroOsc &osc, const char *address, Engine &engine = Engine::primary()) : _microOsc(osc), _address(address), Unit(engine)
=======
        OscIn(MicroOsc &osc, const char *address, Engine &engine = Engine::primary())
            : Unit(engine), _microOsc(osc), _address(address), _valueUpdated(false)
>>>>>>> a57b7287a1db11f3089ab202e8cd376b40c4db29
        {
            oscInList().add(this);
        }

<<<<<<< HEAD
        void set(float f)
        {
            _value = f;
        }

        float get()
=======
        float get() override
>>>>>>> a57b7287a1db11f3089ab202e8cd376b40c4db29
        {
            return _value;
        }

        /// Returns true iff value was changed.
        virtual bool updated() { return _valueUpdated; }

        /// Registers event callback on finish event.
        virtual void onUpdate(EventCallback callback) { onEvent(callback, EVENT_UPDATE); }

        // ----------------------------------------------------------------------- |
        // handleOSCMessageCallback ---------------------------------------------- |
        static void handleOSCMessageCallback(MicroOscMessage &message);
        // ---------------------------------------------------------------------- |

    private:
        // DISABLE THE PUT
        float put(float f) override
        {
            //_value = f;
            return f;
        }

    };
    // ----------------------------------------------------------------------- |

<<<<<<< HEAD
    // _PqOSCMessageCallback ------------------------------------------------- |
    void _PqOSCMessageCallback(MicroOsc & source, MicroOscMessage & message)
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

=======
>>>>>>> a57b7287a1db11f3089ab202e8cd376b40c4db29
    // OscSlip class -------------------------------------------------------- |
    template <const size_t MICRO_OSC_IN_SIZE>
    class OscSlip : public Unit, public MicroOscSlip<MICRO_OSC_IN_SIZE>
    {
    private:
<<<<<<< HEAD

=======
        SerialType* _serial = nullptr;
        unsigned long _baudRate = 0;
>>>>>>> a57b7287a1db11f3089ab202e8cd376b40c4db29

    protected:
        void step() override
        {
            this->onOscMessageReceived(OscIn::handleOSCMessageCallback);
        }

        void begin() override
        {
            if (_serial)
            {
                // Safely start serial.
                beginSerial(*_serial, _baudRate);
            }
        }

        float get() override
        {
            return 0;
        }

    public:
        // Constructor with default value for _iic_address
        OscSlip(Stream &stream, Engine &engine = Engine::primary())
            : Unit(engine), MicroOscSlip<MICRO_OSC_IN_SIZE>(stream)
        {
        }

        OscSlip(SerialType &serial, unsigned long baudRate, Engine &engine = Engine::primary())
            : Unit(engine), MicroOscSlip<MICRO_OSC_IN_SIZE>(serial), _serial(&serial), _baudRate(baudRate)
        {
        }

        OscSlip(unsigned long baudRate, Engine &engine = Engine::primary())
            : OscSlip(PLAQUETTE_DEFAULT_SERIAL, baudRate, engine)
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
            this->onOscMessageReceived(OscIn::handleOSCMessageCallback);
        }

        void begin()
        {
<<<<<<< HEAD
           
=======
>>>>>>> a57b7287a1db11f3089ab202e8cd376b40c4db29
        }

        float get()
        {
            return 0;
        }

    public:
<<<<<<< HEAD
        // Constructor with default value for _iic_address
        OscSlip(Stream &stream, Engine &engine = Engine::primary())
            : MicroOscSlip<MICRO_OSC_IN_SIZE>(stream), Unit(engine)
=======
        OscUdp(UDP &udp, Engine &engine = Engine::primary())
            : Unit(engine), MicroOscUdp<MICRO_OSC_IN_SIZE>(udp)
        {
        }

        OscUdp(UDP &udp, IPAddress destinationIp, unsigned int destinationPort, Engine &engine = Engine::primary())
            : Unit(engine), MicroOscUdp<MICRO_OSC_IN_SIZE>(udp, destinationIp, destinationPort)
>>>>>>> a57b7287a1db11f3089ab202e8cd376b40c4db29
        {
        }
    };
    // ----------------------------------------------------------------------- |

<<<<<<< HEAD
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

=======
>>>>>>> a57b7287a1db11f3089ab202e8cd376b40c4db29
    // OscOut class ---------------------------------------------------------- |
    class OscOut : public Unit
    {
    private:
        MicroOsc &_microOsc;
        const char *_address;
        float _value;

<<<<<<< HEAD
    public:
        OscOut(MicroOsc &osc, const char *address, Engine &engine = Engine::primary()) : _microOsc(osc), _address(address), Unit(engine)
        {
        }
=======
        char _typeTag;
>>>>>>> a57b7287a1db11f3089ab202e8cd376b40c4db29

        bool _needToSend;

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

    protected:
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
    // ----------------------------------------------------------------------- |
}
#endif