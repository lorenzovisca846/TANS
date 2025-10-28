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

const int kPeople = 100;
const int kYear = 365;

double theoprob(int n)
{
    return 1.0 - TMath::Exp(TMath::LnGamma(kYear+1) - TMath::LnGamma(kYear+1 - n) - log((double)kYear) * (double)n);
}

void birthday(const int events = 100000, unsigned int seed = 12345)
{
    gRandom->SetSeed(seed);
    gStyle->SetOptStat(0);
    cout << std::scientific << std::setprecision(1);

    bool shared_birthday[kYear];
    bool found;
    double probabilities[kPeople];

    for(int i=0;i<kPeople;i++)  probabilities[i] = 0.;
    
    for(int i=0;i<events;i++)
    {
        for(int j=0;j<kYear;j++) shared_birthday[j] = false;

        found = false;

        for(int j=0;j<kPeople && !found;j++)
        {
            int day = gRandom->Integer(kYear);

            if(shared_birthday[day])
            {
                found = true;
                for(int k=j;k<kPeople;k++)  probabilities[k] += 1.0; //Probability with j+1 people
            }
            shared_birthday[day] = true;
        }
        if (i > 0 && i % (int)1e6 == 0)
            cout << "Trial " << (double)i << endl;
    }

    TCanvas *c1 = new TCanvas("c1", "Birthday Problem Simulation", 1700, 600);
    c1->Divide(2,1);
    c1->cd(1);

    TH1D *h1 = new TH1D("h1", "Birthday Problem", kPeople, 1, kPeople);

    for (int n = 1; n <= kPeople; n++)
        h1->SetBinContent(n, theoprob(n));

    TH1D *h2 = new TH1D("h2", "Birthday Problem", kPeople, 1, kPeople);
    for(int i=1;i<=kPeople;i++) h2->SetBinContent(i,probabilities[i-1]/(double)events);

    h1->GetXaxis()->SetTitle("Number of people");
    h1->GetYaxis()->SetTitle("Probability of shared birthday");

    h1->SetLineColor(kBlue);
    h1->SetLineWidth(2);
    h1->SetFillColorAlpha(kBlue,0.15);
    h1->SetTitle("Birthday Paradox Simulation");
    h1->Draw("L"); 

    h2->SetMarkerColor(kRed);
    h2->SetMarkerStyle(8);
    h2->SetMarkerSize(0.5);
    h2->Draw("P SAME");

    auto legend = new TLegend(0.65, 0.10, 0.90, 0.25);
    legend->AddEntry(h1, "Theoretical", "l");
    legend->AddEntry(h2, "Simulated", "p");
    legend->Draw();

    c1->cd(2);
    TH1D *hr = new TH1D("hr", "Birthday Problem Residuals", kPeople, 1, kPeople);

    for(int i=1;i<=kPeople;i++)
    {
        double res;
        if(i==1) res = probabilities[0];
        else res = fabs((probabilities[i-1]/(double)events - theoprob(i))/theoprob(i));

        hr->SetBinContent(i,res);
    }

    hr->GetXaxis()->SetTitle("Number of people");
    hr->GetYaxis()->SetTitle("Relative Errors");
    hr->GetYaxis()->SetMaxDigits(3);
    hr->SetMarkerColor(kRed);
    hr->SetMarkerStyle(8);
    hr->SetMarkerSize(0.5);
    hr->Draw("P");

}