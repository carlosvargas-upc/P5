#include <iostream>
#include <math.h>
#include "instrument_fm.h"
#include "keyvalue.h"
#include <stdlib.h>

using namespace upc;
using namespace std;

InstrumentFM::InstrumentFM(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  KeyValue kv(param);
  
  // Leemos los parámetros FM desde el .ins. Si no están, ponemos valores para un bajo
  if (!kv.to_float("c_m_ratio", c_m_ratio))
    c_m_ratio = 1.0f; // Relación 1:1 es perfecta para bajos profundos
    
  if (!kv.to_float("I", I))
    I = 3.5f; // Un índice de 3.5 da un golpe brillante en el ataque

  phase_carrier = 0.0f;
  phase_modulator = 0.0f;
}

void InstrumentFM::command(long cmd, long note, long vel) {
  if (cmd == 9) { // Nota pulsada (Key On)
    last_sample = 0.0f;
    bActive = true;
    adsr.start();
    A = vel / 127.0f;

    // Calcular la frecuencia fundamental de la nota MIDI
    float f0 = 440.0f * pow(2.0f, (note - 69.0f) / 12.0f);

    // La portadora va a la frecuencia fundamental (fc)
    float frequency_carrier = f0;
    // El modulador se calcula usando la relación (fm = fc / ratio)
    float frequency_modulator = frequency_carrier / c_m_ratio;

    // Pasos de fase necesarios para cada oscilador por muestra
    step_carrier = 2.0f * M_PI * frequency_carrier / (float)SamplingRate;
    step_modulator = 2.0f * M_PI * frequency_modulator / (float)SamplingRate;
  }
  else if (cmd == 8) { // Nota soltada (Key Off)
    adsr.stop();
  }
  else if (cmd == 0) { // Extinción rápida
    adsr.end();
  }
}

const std::vector<float> & InstrumentFM::synthesize() {
  // Comprobamos si el ADSR sigue activo (igual que en tu InstrumentSaw)
  if (not adsr.active()) {
    x.assign(BSIZE, 0.0f);
    bActive = false;
    return x;
  }
  else if (not bActive) {
    return x;
  }

  // Usamos el mismo filtro pasabajos (alpha) que hereda tu sintetizador de la sierra
  float alpha = 0.1f;

  for (unsigned int i = 0; i < BSIZE; ++i) {
    // 1. Avanzar la fase del modulador
    phase_modulator += step_modulator;
    if (phase_modulator > 2.0f * M_PI) phase_modulator -= 2.0f * M_PI;

    // 2. Calcular la salida del modulador
    float modulator_output = sin(phase_modulator);

    // 3. Avanzar la fase de la portadora
    phase_carrier += step_carrier;
    if (phase_carrier > 2.0f * M_PI) phase_carrier -= 2.0f * M_PI;

    // Modulación de frecuencia (FM) en la fase final
    float modulated_phase = phase_carrier + I * modulator_output;

    // 4. Muestra calculada aplicando el filtro suavizador pasabajos
    x[i] = alpha * (A * sin(modulated_phase)) + (1.0f - alpha) * last_sample;
    last_sample = x[i];
  }

  adsr(x); 

  return x;
}