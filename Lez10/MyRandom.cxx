#include "MyRandom.h"

ClassImp(MyRandom);

MyRandom::MyRandom():
    TRandom3(),
    fAlpha(0.5),
    Func("Func", "1/(sin(x)*sin(x) + [0]*cos(x)*cos(x))", 0, 2.*M_PI)
    {
        Func.SetParameter(0, 0.5);
        MyRandom::SetSeed(0);
    }

MyRandom::MyRandom(double alpha,unsigned int seed):
    TRandom3(), 
    fAlpha(alpha),
    Func("Func", "1/(sin(x)*sin(x) + [0]*cos(x)*cos(x))", 0, 2.*M_PI)
    {
        Func.SetParameter(0, alpha);
        SetSeed(seed);
    }

double MyRandom::Rejection()
    {
        double xt, yt;
        do
        {
            xt = 2. * M_PI * Rndm();
            yt = Fmax(xt) * Rndm();
        }while(yt > Func.Eval(xt));

        return xt;
    }

double MyRandom::Inversion()
    {
        double u = Rndm();
        double x = atan(sqrt(fAlpha)*tan(M_PI*(u-0.5)));

        double w = Rndm();
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
        return (fAlpha<1.) ? 1.0/fAlpha : 1.0;      // Can be optimized, I need to find an upper bound whose integral is easy to invert
    }