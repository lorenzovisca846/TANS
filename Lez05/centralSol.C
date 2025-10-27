#include <Riostream.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include <TStyle.h>
#include <TF1.h>
using namespace std;

const int nHisto = 7;
const int entries = 2000000;

double random1(const double& w)
{
    double r = gRandom->Rndm();
    return (r<0.5) ? (2.*w*r) : (1. - 2.*w*(1.-r));
}

double randomeff(const double& w)
{
    double r = 2.*w*gRandom->Rndm();
    return (r<w) ? r : (1.-2.*w+r);
}

double randomlong(const double& w)
{
    double r = gRandom->Rndm();
    while(r>w && r<(1.-w))
        r = gRandom->Rndm();
    return r;
}

void centralSol(const double& w = 0.2, TString choice = "", unsigned int seed = 98765)
{   
    double (*randFunc)(const double&);
    if(choice.Contains("1") || choice.Contains("random1")) 
    {
        randFunc = &random1;
        cout << "Using random1 function" << endl;
    }
    else if(choice.Contains("long") || choice.Contains("randomlong"))
        randFunc = &randomlong;
    else
        randFunc = &randomeff;

    gRandom->SetSeed(seed);

    if(w<0 || w>0.5)
    {
        cout << "w deve essere compreso tra 0 e 0.5" << endl;
        return;
    }

    cout << setprecision(5) << setiosflags(ios::scientific) << "CENTRAL LIMIT THEOREM SIMULATION" << endl;
    
    char nome[40];
    char titolo[40];

    TH1D* hist[nHisto];

    const int N[] = {1, 2, 5, 10, 50, 100, 300, 900, 2000};

    for(int i=0; i<nHisto; i++)
    {
        snprintf(nome, 15, "hist%d", N[i]);
        snprintf(titolo, 30, "Sum of %d values", N[i]);

        double mean = 0.5 * (double)N[i];
        double stdev = sqrt(((1./6.)*(2.*w*w - 3.*w + 3) - 0.25) * (double)N[i]);

        double xmin = mean - 5.*stdev;
        if(xmin<0.) xmin = 0.;
        double xmax = mean + 5.*stdev;
        if(xmax>(double)N[i]) xmax = (double)N[i];

        hist[i] = new TH1D(nome, titolo, 100, xmin, xmax);
    }

    for(int i=0; i<entries; i++)
    {
        double sum = 0.;
        for(int j=0; j<N[nHisto-1]; j++)
        {
            sum += randFunc(w);
            for(int k=0; k<nHisto; k++)
                if(j == N[k]-1) hist[k]->Fill(sum);
        }
    }

    TCanvas *cv[nHisto];
    gStyle->SetOptFit();

    for(int i=0;i<nHisto;i++)
    {
        cv[i] = new TCanvas();
        hist[i]->SetFillColor(kOrange-6);
        if(N[i]>4) hist[i]->Fit("gaus");
        hist[i]->Draw();
    }

}