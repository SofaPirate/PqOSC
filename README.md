# PqOsc

Open Sound Control (OSC) library for [Plaquette](https://github.com/SofaPirate/Plaquette)

GitHub repo : https://github.com/SofaPirate/PqOsc

> [!CAUTION]
> Work in progress, API will change!

Here are two examples that show how to receive the OSC Slip data:
- [OSC SLIP dans Pd @ AidE](https://t-o-f.info/aide/#/pd/osc/slip/)
- [Pd : Relais OSC SLIP ⇄ UDP @ AidE](https://t-o-f.info/aide/#/pd/osc/relais/)
- [Max : Relais OSC SLIP ⇄ UDP @ AidE](https://t-o-f.info/aide/#/max/osc/relais/)

## Examples

If you still want to try this library here are some examples. You can test these examples with the Pure Data patch included in the 'extras' directory.

### Output only

This code outputs, every 100 milliseconds, an OSC SLIP message with the OSC address "/wave" and the value of a SINE Wave :
```cpp
#include <Arduino.h>
#include <Plaquette.h>
#include <PqOsc.h>

// The input signal core wave.
Wave wave(SINE);

// Create the input and output node
OscSlip<128> oscSlip(Serial, 115200);

// Link an output address to the node
OscOut oscOutWave(oscSlip, "/wave");

// Used to slow down message transmission
Metronome ticker(0.1f); // 10 Hz

void begin()
{


}

void step()
{

  if (ticker)
  {
    wave >> oscOutWave;

  }
}

```

### Echo (input and output)

This code echoes every 100 milliseconds the value of the OSC message received on the OSC address "/alpha" to the OSC address "/beta" (i.e. /alpha → Arduino → /beta):
```cpp
#include <Arduino.h>
#include <Plaquette.h>
#include <PqOsc.h>

// Create the input and output node
OscSlip<128> oscSlip(Serial, 115200);

// Link an input address to the node
OscIn oscInAlpha(oscSlip, "/alpha");

// Link an output address to the node
OscOut oscOutBeta(oscSlip, "/beta");

// Used to slow down message transmission
Metronome ticker(0.1f); // 10 Hz

void begin()
{


}

void step()
{
  if (ticker)
  {
    oscInAlpha >> oscOutBeta;
  }
}
```