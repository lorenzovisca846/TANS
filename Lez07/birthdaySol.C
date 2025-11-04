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

constexpr int kPeople = 100;
constexpr int kYear = 365;

double theoprob(int);
int GetDay();
void FillHist(TH1D*, TH1D*, int, int);

void birthdaySol(const int events = 100000, unsigned int seed = 12345)
{
    gRandom->SetSeed(seed);
    gStyle->SetOptStat(0);
    cout << std::scientific << std::setprecision(1);

    //Static function della classe TH1 che calcola di default gli errori associati alle celle
    TH1::SetDefaultSumw2(true);

    TH1D* num = new TH1D("num","numeratore",   kPeople-1, 1.5, 0.5+kPeople);
    TH1D* den = new TH1D("den","denominatore", kPeople-1, 1.5, 0.5+kPeople);
    TH1D* par = new TH1D("par","Paradosso dei compleanni", kPeople-1, 1.5, 0.5+kPeople);

    par->GetXaxis()->SetTitle("Numero di persone");
    par->GetYaxis()->SetTitle("Probabilit#grave{a} di almeno una coppia");

    TH1D* teo = new TH1D("teo","Paradosso dei compleanni: valore atteso", kPeople-1, 1.5, 0.5+kPeople);

    teo->GetXaxis()->SetTitle("Numero di persone");
    teo->GetYaxis()->SetTitle("Probabilit#grave{a} di almeno una coppia");

    for(int i = 1; i <= events; i++)
    {
        if(i%100==0) cout << "Iterazione " << i << endl;
        std::array<bool,kYear+1> seen{};
        seen[GetDay()] = true;
        bool found = false;
        int j = 1;
        while(!found && j<kPeople)
        {
            j++;
            int day = GetDay();
            if(seen[day]) found = true;
                else seen[day] = true;
            den->Fill(i);
        }
        if(found)
            FillHist(num,den,j,kPeople);
    }
    par->Divide(num,den,1,1,"B");
    par->SetMarkerStyle(20);
    par->SetMarkerColor(kRed);
    par->Draw("PE");

    for(double i=2;i<=kPeople;i++)
        teo->Fill(i,theoprob(i));
    teo->SetLineColor(kBlue);
    teo->Draw("samehistoc");
    
    TCanvas *cv = new TCanvas();
    cv->SetWindowPosition(750,50);
    teo->Draw("histoc");


}

void FillHist(TH1D* num, TH1D* den, int i, int imax)
{
    num->Fill(i);
    for(int k=i+1;k<=imax;k++)
    {
        num->Fill(k);
        den->Fill(k);
    }
}



double theoprob(int n)
{
    return 1.0 - TMath::Exp(TMath::LnGamma(kYear+1) - TMath::LnGamma(kYear+1 - n) - log((double)kYear) * (double)n);
}

int GetDay()
{
    return gRandom->Integer(kYear) + 1;
}