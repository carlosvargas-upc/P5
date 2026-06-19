#ifndef INSTRUMENT_SENO_H
#define INSTRUMENT_SENO_H

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {

class InstrumentSeno : public Instrument {
    EnvelopeADSR adsr;

    float phase;
    float step;
    float A;

    std::vector<float> tbl;

public:
    InstrumentSeno(const std::string &param = "");

    void command(long cmd,long note,long velocity=1);

    const std::vector<float> &synthesize();

    bool is_active() const {
        return bActive;
    }
};

}

#endif