#include <iostream>
#include <math.h>
#include "instrument_saw.h"
#include "keyvalue.h"
#include <stdlib.h>

using namespace upc;
using namespace std;

InstrumentSaw::InstrumentSaw(const std::string &param) 
  : adsr(SamplingRate, param) {
    last_sample = 0.0;
  bActive = false;
  x.resize(BSIZE);

  KeyValue kv(param);
  int N;

  if (!kv.to_int("N", N))
    N = 40; // Valor por defecto del tamaño de la tabla
  
  tbl.resize(N);
  this->phase = 0;
  this->step = 0;
 
  // Va subiendo linealmente desde -1.0 hasta 1.0 a lo largo de la tabla
  for (int i = 0; i < N; ++i) {
    tbl[i] = -1.0 + 2.0 * ((float)i / (float)N);
  }
}

void InstrumentSaw::command(long cmd, long note, long vel) {
  if (cmd == 9) {   // Nota activada
    bActive = true;
    adsr.start();
    A = vel / 127.0;
    
    // Calculamos el step con los decimales corregidos (.0)
    float frecuencia = 440.0 * pow(2.0, (note - 69) / 12.0);
    frecuencia = frecuencia / 4.0; //-2  octava.
    this->step = frecuencia * tbl.size() / SamplingRate; 
  }
  else if (cmd == 8) {  // Nota soltada
    adsr.stop();
  }
  else if (cmd == 0) {  // Nota apagada de golpe
    adsr.end();
  }
}

const vector<float> & InstrumentSaw::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    last_sample = 0.0; 
    return x;
  }
  else if (not bActive)
    return x;
  float alpha = 0.3; //VALOR DEL FILTRO PASABAJO 
  for (unsigned int i = 0; i < x.size(); ++i) {
    // Leemos de la tabla usando el redondeo corregido
    x[i] = alpha * (A * tbl[(int)(phase + 0.5)]) + (1 - alpha) * last_sample;
    phase += step; 
    

    last_sample = x[i]; 
    
    // Evitamos que la fase se desborde de la tabla
    while (phase >= tbl.size() - 0.5) {
      phase -= tbl.size();
    }
  }
  
  adsr(x); // Aplicamos la envolvente ADSR
  return x;
}