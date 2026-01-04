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

    // OscIn class ----------------------------------------------------------- |
    class OscIn : public Unit
    {

        // friend class OSCSlip; // Grant access
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
            _value = f;
            _messageReceived = true;
        }

    public:
        const char *address()
        {
            return _address;
        }

        OscIn(MicroOsc &osc, const char *address, Engine &engine = Engine::primary())
            : Unit(engine), _microOsc(osc), _address(address), _valueUpdated(false)
        {
            oscInList().add(this);
        }


        float get() override
        {
            return _value;
        }

        /// Returns true iff value was changed.
        virtual bool updated() { return _valueUpdated; }

        /// Registers event callback on finish event.
        virtual void onUpdate(EventCallback callback) { onEvent(callback, EVENT_UPDATE); }

        // ----------------------------------------------------------------------- |
        // _PqOSCMessageCallback ------------------------------------------------- |
        static void _PqOSCMessageCallback(MicroOscMessage &message);
        // ---------------------------------------------------------------------- |

    private:
        // DISABLE THE PUT
        float put(float f) override
        {
            //_value = f;
            return f;
        }

    };

    // OscSlip class -------------------------------------------------------- |
    template <const size_t MICRO_OSC_IN_SIZE>
    class OscSlip : public Unit, public MicroOscSlip<MICRO_OSC_IN_SIZE>
    {
    private:


    protected:
        void step() override
        {
            this->onOscMessageReceived(OscIn::_PqOSCMessageCallback);
        }

        void begin() override
        {
           
        }

        float get() override
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
            this->onOscMessageReceived(OscIn::_PqOSCMessageCallback);
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
    // ----------------------------------------------------------------------- |
}
#endif