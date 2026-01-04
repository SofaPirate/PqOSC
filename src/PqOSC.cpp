#include "PqOSC.h"

namespace pq {
// SINGLE definition
HybridArrayList<OscIn*, PLAQUETTE_MAX_UNITS> _PqOscInList;

void OscIn::_PqOSCMessageCallback(MicroOscMessage &message)
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

void OscOut::begin() {
    switch (_typeTag) {
        case 'i':
        case 'b':
        case 's':
        case 'f':
        case 'd':
          mode(OSC_VALUE);
          break;

        // Triggers.
        case 'T':
        case 'F':
        case 'N':
        case 'I':
        case '\0': // no arguments (backwards compatibility)
        case ' ':
          mode(OSC_EVENT);
          break;

        // Unsupported.
        case 't': // osc timetag
        case 'm':
        case 'h':
        default:;
          mode(OSC_VALUE);
          break;
    }
}

void OscOut::mode(uint8_t mode) {
  // Change mode.
  _mode = constrain(mode, (uint8_t)OSC_VALUE, (uint8_t)OSC_EVENT);
}

void OscOut::_sendMessage() {

    bool isValue   = (_mode == OSC_VALUE);
    bool sendValue = (isValue || analogToDigital(_value));
    
    if (sendValue) {

      switch (_typeTag) {

          // Values.
          case 'i': {
              _microOsc.sendInt(_address, round(_value)); 
              break;
          }
          case 'b': {
              _microOsc.sendBlob(_address, (unsigned char *) &_value, sizeof(_value)); 
              break;
          }
          case 's': {
              char str[32];
              sprintf(str, "%f", _value);
              _microOsc.sendString(_address, (const char *) &_value); 
              break;
          }
          case 'f': {
              _microOsc.sendFloat(_address, _value); 
              break;
          }
          case 'd': {
              _microOsc.sendDouble(_address, _value); 
              break;
          }

          // Triggers.
          case 'T': { // true
              _microOsc.sendTrue(_address); 
              break;
          }
          case 'F': { // false
              _microOsc.sendFalse(_address); 
              break;
          }
          case 'N': { // nil
              _microOsc.sendNull(_address); 
              break;
          }
          case 'I': { // impulse
              _microOsc.sendImpulse(_address); 
              break;
          }
          case '\0': // no arguments (backwards compatibility) - supported but not recommended
          case ' ': { 
              _microOsc.sendMessage(_address, ""); 
              break;
            }

          // Unsupported.
          case 't': // osc timetag
          case 'm':
          case 'h':
          default:;
      }
    }

    // if (!isValue) {
    //     _value = 0.0;
    // }
}

}
