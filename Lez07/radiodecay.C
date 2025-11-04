/*
loop from 0 to T with step dt 
    reset number of decays in this time step 
    loop over all particles and decide if they decay in this time step: it decays if rnd < mu*dt. end loop
    update ntot, plot ntot vs t
end loop

expected output: N(t)=N0exp(-mu*t)

N0 = 1000,  mu = 0.01, dt = 1,   T = 300
N0 = 50000, mu = 0.03, dt = 1,   T = 300
N0 = 5e5,   mu = 3e-3, dt = .09, T = 1002.5 
All times in seconds
*/
#include <Riostream.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include <TStyle.h>
#include <TF1.h>
#include <TMath.h>
#include <TLegend.h>

using namespace std;

void simulation(int, double, double, double, TH1D*, TCanvas*, int);

void radiodecay(unsigned int seed = 12345)
{

    gRandom->SetSeed(seed);

    TCanvas *c1 = new TCanvas("c1", "Decay Simulation", 1200, 2000);
    c1->Divide(2, 3);
    gStyle->SetOptStat(0);
    TH1D* hist[3];

    simulation(1e3, 0.01,  1.0,  300.0,  hist[0], c1, 1);
    simulation(5e4, 0.03,  1.0,  300.0,  hist[1], c1, 2);
    simulation(5e5, 0.003, 0.09, 1002.5, hist[2], c1, 3);
}


void simulation(int N0, double mu, double dt, double T, TH1D* histo, TCanvas* c1, int row)
{
    int nBins = T/dt + 1;

    histo = new TH1D(Form("h%d", row), Form("Simulation %d",row),nBins, -0.5, T+0.5);
    histo->GetXaxis()->SetTitle("Time (s)");
    histo->GetYaxis()->SetTitle("#Particles");

    TH1D* hexp = new TH1D(Form("hexp%d", row), Form("Expected %d", row),nBins, -0.5, T+0.5);
    TH1D* herr = new TH1D(Form("herr%d", row), Form("Simulation %d relative errors", row),nBins, -0.5, T+0.5);

    double t = 0.;

    int N0_i = N0;
    histo->SetBinContent(0, N0);
    hexp->SetBinContent(0, N0_i);

    while(t<T)
    {
        if(t+dt>T) dt = T - t;

        int decayed = 0;
        for(int i=0;i<N0;i++)
            if(gRandom->Rndm() < mu*dt)
                decayed++;
        N0 -= decayed;
        t += dt;

        int bin = ceil(t/T * nBins);
        histo->SetBinContent(bin, N0);
        hexp->SetBinContent(bin, N0_i*exp(-mu*t));
    }

    c1->cd(row*2 - 1);
    gPad->SetLogy(0);
    histo->SetFillColorAlpha(kOrange-4,0.5);
    histo->SetLineColor(kOrange+7);
    histo->Draw("HIST");
    hexp->SetLineColor(kAzure-2);
    hexp->SetLineWidth(3);
    hexp->Draw("SAME L");
   
    c1->cd(row*2);
    
    for(int i=0;i<nBins;i++)
    {
        double o = histo->GetBinContent(i+1);
        double c = hexp->GetBinContent(i+1);
        if(c!=0)
            herr->SetBinContent(i+1, (o - c)/c);
        else
            herr->SetBinContent(i+1, 0.);
    }

    herr->SetMarkerStyle(8);
    herr->SetMarkerColor(kRed);
    herr->SetMarkerSize(0.7);
    herr->GetYaxis()->SetTitle("o-c");
    herr->Draw("P");

}