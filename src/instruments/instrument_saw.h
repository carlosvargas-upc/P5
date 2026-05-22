#ifndef INSTRUMENT_SAW
#define INSTRUMENT_SAW

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentSaw: public upc::Instrument {
    EnvelopeADSR adsr;
    float phase, step;
    float A;
    std::vector<float> tbl;
    float last_sample;
  public:
    InstrumentSaw(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif