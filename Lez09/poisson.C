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

int Ntrials = 10000;

void simulation(int, double, double, double, TH1D*);

void poisson(unsigned int seed = 12345)
{

    gRandom->SetSeed(seed);

    TCanvas *c1 = new TCanvas("c1", "Poisson Simulation", 1600, 600);
    c1->Divide(2, 1);
    gStyle->SetOptStat(0);
    TH1D* hist[2];

    int N0[2] = {1000, 3000};
    double a[2] = {2.5e-5, 1.0e-4};
    double dt[2] = {1.0, 0.5};
    double T = 100.0;

    for(int i=0;i<2;i++)
    {
        double mu = a[i]*T*(double)N0[i];


        hist[i] = new TH1D(Form("h%d",i+1), Form("Simulation %d",i+1),5*ceil(mu)+1, -0.5, 5.*ceil(mu)+0.5);
        hist[i]->GetXaxis()->SetTitle("#Counts");
        hist[i]->GetYaxis()->SetTitle("#Entries");

        for(int j=0;j<Ntrials;j++)
            simulation(N0[i], a[i], dt[i], T,  hist[i]);
    }
    
    c1->cd(1);
    hist[0]->SetFillColorAlpha(kOrange-4,0.5);
    hist[0]->SetLineColor(kOrange+7);
    hist[0]->Draw("HIST");

    c1->cd(2);
    hist[1]->SetFillColorAlpha(kOrange-4,0.5);
    hist[1]->SetLineColor(kOrange+7);
    hist[1]->Draw("HIST");

}


void simulation(int N0, double mu, double dt, double T, TH1D* histo)
{
    double t = 0.;
    int totdec = 0;
    while(t<T)
    {
        if(t+dt>T) dt = T - t;

        int decayed = 0;
        for(int i=0;i<N0;i++)
            if(gRandom->Rndm() < mu*dt)
                decayed++;

        N0 -= decayed;
        t += dt;
        totdec += decayed;
    }
    histo->Fill(totdec);
}