#ifndef MYRANDOM_H
#define MYRANDOM_H

#include "TRandom3.h"
#include "TF1.h"

class MyRandom : public TRandom3 {
    public:
        MyRandom();
        MyRandom(double alpha,unsigned int seed);

        void SetAlpha(double alpha) {fAlpha=alpha; Func.SetParameter(0, fAlpha);}

        double Rejection();
        double Inversion();
        double FromFunc();                              // Cannot be const because of GetRandom(), could be defined as const but cloning the function would be slower
        const TF1& GetFunc() const {return Func;}       // Const getter, only for cloning

    private:
        double fAlpha;
        double Fmax(double theta) const;

        TF1 Func;     //   1 /(sin(x)^2 + alpha*cos(x)^2)

    ClassDef(MyRandom,1);
};


#endif // MYRANDOM_H