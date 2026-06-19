#include <iostream>
#include "instrument_dumb.h"  //Añadir instrumentos aqui con su .h
#include "instrument_saw.h"   
#include "instrument_fm.h"
#include "instrument_seno.h"

/*
  For each new instrument:
  - Add the header in this file
  - Add the call to the constructor in get_instrument() (also in this file)
  - Add the source file to src/meson.build
*/

using namespace std;

namespace upc {
  Instrument * get_instrument(const string &name,
                              const string &parameters) {

    Instrument * pInst = 0;

    if (name == "InstrumentDumb") {
      pInst = new InstrumentDumb(parameters);
    }
    else if (name == "InstrumentSaw") {
      pInst = new InstrumentSaw(parameters);
    }
    else if (name == "InstrumentFM") {
      pInst = new InstrumentFM(parameters);
    }
    else if (name == "InstrumentSeno") {
      pInst = new InstrumentSeno(parameters);
    }

    return pInst;
  }
}