#include <Riostream.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include <TF1.h>
using namespace std;

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



void createhisto(const double& w, const int& entries, const int& Nsum)
{
    string name = "Sum of " + to_string(Nsum);
    string cname = "canvas" + to_string(Nsum);
    string hname = "hist" + to_string(Nsum);

    TCanvas* canvas = new TCanvas(cname.c_str(), "Teorema Limite Centrale", 800, 600);
    canvas->SetFrameFillColor(17);

    double mean = 0.5 * Nsum;
    double stdev = sqrt(((1./6.)*(2.*w*w - 3.*w + 3) - 0.25) * (double)Nsum);
    if(Nsum==1) stdev = 0.1;
    
    TH1D* hist = new TH1D(hname.c_str(), name.c_str(), 100, mean-5.*stdev, mean+5.*stdev);
    hist->SetLineColor(kGray+3);
    hist->SetLineWidth(3);
    hist->SetFillColorAlpha(kOrange+5, 0.75);
    for(int i=0; i<entries; i++)
    {
        double sum = 0.;
        for(int j=0; j<Nsum; j++)    sum += randomeff(w);
        hist->Fill(sum);
    }

    TF1* fit = new TF1("gaus", "gaus", mean-5.*stdev, mean+5.*stdev);
    fit->SetLineColor(kBlue+2);
    hist->Fit(fit,"RQ");

    cout << "N = " << Nsum
         << "\t mu error = " << fabs(fit->GetParameter(1)/mean -1.)
         << "\t sigma error = " << fabs(fit->GetParameter(2)/stdev -1.)
         << "\t prob = " << fit->GetProb() << endl;

    hist->Draw();
}


void central(const double& w = 0.2, const int& nHisto = 7, const int& entries = 2000000, unsigned int seed = 98765)
{   
    gRandom->SetSeed(seed);

    if(w<0 || w>0.5)
    {
        cout << "w deve essere compreso tra 0 e 0.5" << endl;
        return;
    }

    cout << setprecision(5) << setiosflags(ios::scientific)
         << "CENTRAL LIMIT THEOREM SIMULATION" << endl;
    
    int N[] = {1, 2, 5, 10, 50, 100, 300, 900, 2000};

    for(int i=0; i<nHisto; i++) createhisto(w, entries, N[i]);
}