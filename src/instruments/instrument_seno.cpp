#include <math.h>
#include "instrument_seno.h"
#include "keyvalue.h"

using namespace upc;
using namespace std;

InstrumentSeno::InstrumentSeno(const string &param)
    : adsr(SamplingRate,param)
{
    bActive = false;

    x.resize(BSIZE);

    KeyValue kv(param);

    int N;

    if (!kv.to_int("N",N))
        N = 1024;

    tbl.resize(N);

    for (int i=0;i<N;i++)
        tbl[i] = sin(2.0*M_PI*i/N);

    phase = 0;
}

void InstrumentSeno::command(long cmd,long note,long vel)
{
    if (cmd == 9)
    {
        bActive = true;

        adsr.start();

        A = vel / 127.0f;

        float freq =
            440.0f *
            pow(2.0f,(note-69.0f)/12.0f);

        step =
            freq *
            tbl.size() /
            SamplingRate;
    }
    else if (cmd == 8)
    {
        adsr.stop();
    }
    else if (cmd == 0)
    {
        adsr.end();
    }
}

const vector<float> &InstrumentSeno::synthesize()
{
    if (!adsr.active())
    {
        x.assign(BSIZE,0);
        bActive = false;
        return x;
    }

    for (unsigned int i=0;i<BSIZE;i++)
    {
        x[i] =
            A * tbl[(int)(phase+0.5)];

        phase += step;

        while (phase >= tbl.size())
            phase -= tbl.size();
    }

    adsr(x);

    return x;
}