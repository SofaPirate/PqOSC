# PqOsc

Open Sound Control (OSC) library for [Plaquette](https://github.com/SofaPirate/Plaquette)

GitHub repo : https://github.com/SofaPirate/PqOsc

> [!CAUTION]
> Work in progress, API will change!

Here are two examples that show how to receive the OSC Slip data:
- [OSC SLIP dans Pd @ AidE](https://t-o-f.info/aide/#/pd/osc/slip/)
- [Pd : Relais OSC SLIP ⇄ UDP @ AidE](https://t-o-f.info/aide/#/pd/osc/relais/)
- [Max : Relais OSC SLIP ⇄ UDP @ AidE](https://t-o-f.info/aide/#/max/osc/relais/)

If you still want to try, here is an example :
```cpp
#include <Arduino.h>
#include <Plaquette.h>
#include <PqOsc.h>

// The input signal core wave.
Wave wave(SINE);

// Create the input and output node
OscSlip<128> oscSlip(Serial, 115200);

// Link an output address to the node
OscOut oscOutWave(oscSlip, "/scaler");

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