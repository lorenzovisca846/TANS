#include "MyRandom.h"

ClassImp(MyRandom);

MyRandom::MyRandom():
    TRandom3(),
    fAlpha(1.0),
    fSeed(0),
    Func("Func", "1/(sin(x)*sin(x) + [0]*cos(x)*cos(x))", 0, 2.*M_PI)
    {
        gRandom->SetSeed(0);
        Func.SetParameter(0, 1.0);
    }

MyRandom::MyRandom(double alpha,unsigned int seed):
    TRandom3(), 
    fAlpha(alpha),
    fSeed(seed),
    Func("Func", "1/(sin(x)*sin(x) + [0]*cos(x)*cos(x))", 0, 2.*M_PI)
    {
        gRandom->SetSeed(seed);
        Func.SetParameter(0, alpha);
    }

double MyRandom::Rejection() const
    {
        double xt = gRandom->Rndm() * 2 * M_PI;
        double yt = gRandom->Rndm() * Fmax(xt);

        while(yt > Func.Eval(xt))
        {
            xt = gRandom->Rndm() * 2 * M_PI;
            yt = gRandom->Rndm() * Fmax(xt);
        }
        return xt;
    }

double MyRandom::Inversion() const
    {
        double u = gRandom->Rndm();
        double x = atan(sqrt(fAlpha)*tan(M_PI*(u-0.5)));

        double w = gRandom->Rndm();
        if(w<0.5)
            return x + M_PI;
        if(x<0.)
            return x + 2*M_PI;
        return x;
    }

double MyRandom::FromFunc()
    {
        return Func.GetRandom();
    }

double MyRandom::Fmax(double theta) const
    {
        // Can be optimized, I need to find an upper bound whose integral is easy to invert
        return (fAlpha<1.) ? 1.0/fAlpha : 1.0;
    }