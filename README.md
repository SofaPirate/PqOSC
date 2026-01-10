# PqOsc

Open Sound Control (OSC) library for [Plaquette](https://github.com/SofaPirate/Plaquette)

GitHub repo : https://github.com/SofaPirate/PqOsc

> [!CAUTION]
> Work in progress, API will change!

To receive the data, your software must be able to process OSC SLIP. Here are a few demonstrations that show how to receive the OSC Slip data in Pure Data and Max:
- [OSC SLIP dans Pd @ AidE](https://t-o-f.info/aide/#/pd/osc/slip/)
- [Pd : Relais OSC SLIP ⇄ UDP @ AidE](https://t-o-f.info/aide/#/pd/osc/relais/)
- [Max : Relais OSC SLIP ⇄ UDP @ AidE](https://t-o-f.info/aide/#/max/osc/relais/)
- There is also a Pure Data patch included in the `extras` directory.

## Examples

If you still want to try this library here are some examples. You can test these examples with the Pure Data patch included in the `extras` directory.

### Output only

This code outputs, every 100 milliseconds, an OSC SLIP message with the OSC address "/wave" and the value of a SINE Wave :
```cpp
#include <Arduino.h>
#include <Plaquette.h>
#include <PqOsc.h>

// The input signal core wave.
Wave wave(SINE);

// Create the input and output node
OscSlip<128> oscSlip(115200); // use Serial by default

// Link an output address to the node
OscOut oscOutWave(oscSlip, "/wave");

// Used to slow down message transmission
Metronome ticker(0.1f); // 10 Hz

void begin()
{
  Serial.begin(115200);
}

void step()
{

  if (ticker)
  {
    wave >> oscOutWave;

  }
}
```

### Echo OSC (forward input to output) over SLIP

This code echoes every 100 milliseconds the value of the OSC message received on the OSC address "/alpha" to the OSC address "/beta" (i.e. /alpha → Arduino → /beta):
```cpp
#include <Arduino.h>
#include <Plaquette.h>
#include <PqOsc.h>

// Create the input and output node
OscSlip<128> oscSlip(115200);

// Link an input address to the node
OscIn oscInAlpha(oscSlip, "/alpha");

// Link an output address to the node
OscOut oscOutBeta(oscSlip, "/beta");

// Used to slow down message transmission
Metronome ticker(0.1f); // 10 Hz

void begin()
{
  Serial.begin(115200);
}

void step()
{
  if (ticker)
  {
    oscInAlpha >> oscOutBeta;
  }
}
```

### Echo OSC (forward input to output) over UDP

The following code echoes every 100 milliseconds the value of the OSC message received on the OSC address "/alpha" to the OSC address "/beta" (i.e. /alpha → Arduino → /beta).

The code uses the [MicroNet](https://github.com/thomasfredericks/MicroNet) library to help with networking.

The microcontroller connects to the network with the name "plaquette" and it looks for a destination device on the network called "m3-air". Note that you should append ".local" tho these names when using when referencing them in your software. For example, in Pure Data, you would connect to "plaquette.local".

```cpp
#include <Arduino.h>
#include <Plaquette.h>
#include <PqOsc.h>
#include <MicroNetEthernet.h>
MicroNetEthernet myMicroNet(MicroNetEthernet::Configuration::ATOM_POE_WITH_ATOM_LITE);

EthernetUDP myUdp;
const unsigned int kMyReceivePort = 8888;
const unsigned int kMySendPort = 7777;

// Create the input and output node
OscUdp<128> myOsc(myUdp);
  
// Link an input address to the node
OscIn oscInAlpha(myOsc, "/alpha");

// Link an output address to the node
OscOut oscOutBeta(myOsc, "/beta");

// Used to slow down message transmission
Metronome ticker(0.1f); // 10 Hz, each 100 milliseconds, 10x per second

// Serial monitor.
Monitor monitor(115200);

void begin()
{

  myMicroNet.begin("plaquette");

  myUdp.begin(kMyReceivePort);

  println();
  print("My IP: ");
  print(myMicroNet.getIP());
  println();
  print("My name: ");
  print(myMicroNet.getName());
  println();

  print("Looking for: m3-air");
  println();

  IPAddress destinationIp = myMicroNet.resolveName("m3-air");
  myOsc.setDestination(destinationIp , kMySendPort);

  print("Found it at IP: ");
  print(destinationIp);
  println();
}

void step()
{

  myMicroNet.update();

  if (ticker)
  {
    oscInAlpha >> oscOutBeta;
  }
}
```