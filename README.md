# PqOSC
Open Sound Control (OSC) library for Plaquette

> [!CAUTION]
> Work in progress — not ready for use.

If you don't care, here is the basic usage :
```cpp
#include <Arduino.h>
#include <Plaquette.h>
#include <PqOSC.h>

// The input signal core wave.
Wave wave(SINE);

// Create the input and output node
OSCSSLIP<128> oscSlip(Serial, 115200);

// Link an output address to the node
OSCOut oscOutWave(oscSlip, "/scaler");

// Used to slow down message transmission
Metronome ticker(0.1f); // 10 Hz

void begin()
{
  robustScalerMaxSpan.span(1.0f);

}

void step()
{

  if (ticker)
  {
    wave >> oscOutWave;

  }
}

```