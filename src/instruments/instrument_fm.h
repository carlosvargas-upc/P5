#ifndef INSTRUMENT_FM_H
#define INSTRUMENT_FM_H

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentFM: public upc::Instrument {
    EnvelopeADSR adsr;      // Envolvente principal para la amplitud
    float phase_carrier;    // Fase de la señal portadora
    float phase_modulator;  // Fase de la señal moduladora
    float step_carrier;     // Incremento de fase de la portadora
    float step_modulator;   // Incremento de fase de la moduladora
    float A;                // Amplitud según la velocidad MIDI
    float last_sample;     // Para evitar clicks al soltar la nota
    
    // Parámetros de Síntesis FM
    float c_m_ratio;        // Relación de frecuencias Carrier/Modulator (fc/fm)
    float I;                // Índice de modulación máximo (brillo en el ataque)

  public:
    InstrumentFM(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const { return bActive; } 
  };
}

#endif