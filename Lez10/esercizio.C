#include <Riostream.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include <TStyle.h>
#include <TF1.h>
#include <TStopwatch.h>
#include <TLegend.h>

#include "MyRandom.h"

using namespace std;

//Remember to load .L MyRandom.cxx+ 


void esercizio(double alpha=0.5, int Nsim=1e5);
//void esercizio(double alpha=0.001, int Nsim=1e5);

void esercizio(double alpha, int Nsim)
{
    TCanvas *c1 = new TCanvas("c1", "Random Number Generation", 1500, 1200);
    c1->Divide(2,2);
    gStyle->SetOptStat(0);

    MyRandom myrand(alpha,12345);

    double xmin = 0.;
    double xmax = 2.*M_PI;
    int nbins = 200;

    TH1D* hrejection = new TH1D("hrejection", Form("Rejection Method, #alpha = %f", alpha), nbins, xmin, xmax);
    TH1D* hinversion = new TH1D("hinversion", Form("Inversion Method, #alpha = %f", alpha), nbins, xmin, xmax);
    TH1D* hfunction = new TH1D("hfunction", Form("ROOT Standard Method, #alpha = %f", alpha), nbins, xmin, xmax);

    TF1* func = static_cast<TF1*>(myrand.GetFunc().Clone("func"));
    func->SetNpx(1000);
    func->SetLineColor(kBlack);
    func->SetLineWidth(2);

    TStopwatch timer;

    timer.Start();
    for(int i=0;i<Nsim;i++)
        hrejection->Fill(myrand.Rejection());
    timer.Stop();
    cout << "Rejection method time: " << timer.RealTime() << " s" << endl;

    timer.Start();
    for(int i=0;i<Nsim;i++)
        hinversion->Fill(myrand.Inversion());
    timer.Stop();
    cout << "Inversion method time: " << timer.RealTime() << " s" << endl;

    timer.Start();
    for(int i=0;i<Nsim;i++)
        hfunction->Fill(myrand.FromFunc());
    timer.Stop();
    cout << "ROOT standard method time: " << timer.RealTime() << " s" << endl;

    double norm = func->Integral(xmin, xmax) / ((xmax-xmin)/(double)nbins * Nsim);
    hrejection->Scale(norm);
    hinversion->Scale(norm);
    hfunction->Scale(norm);

    c1->cd(1);
    hrejection->SetLineColor(kOrange+7);
    hrejection->GetXaxis()->SetTitle("#theta");
    hrejection->GetYaxis()->SetTitle("f(#theta)");
    hrejection->Draw("HIST");
    func->Draw("SAME");

    c1->cd(2);
    hinversion->SetLineColor(kOrange+7);
    hinversion->GetXaxis()->SetTitle("#theta");
    hinversion->GetYaxis()->SetTitle("f(#theta)");
    hinversion->Draw("HIST");
    func->Draw("SAME");

    c1->cd(3);
    hfunction->SetLineColor(kOrange+7);
    hfunction->GetXaxis()->SetTitle("#theta");
    hfunction->GetYaxis()->SetTitle("f(#theta)");
    hfunction->Draw("HIST");
    func->Draw("SAME");

    c1->cd(4);
    TH1D* hrejection1 = (TH1D*)hrejection->Clone("hrejection1");
    TH1D* hinversion1 = (TH1D*)hinversion->Clone("hinversion1");
    TH1D* hfunction1 = (TH1D*)hfunction->Clone("hfunction1");

    hrejection1->SetLineColor(kRed);
    hinversion1->SetLineColor(kBlue);
    hfunction1->SetLineColor(kGreen);

    hinversion1->SetTitle(Form("Comparison, #alpha = %f", alpha));
    hinversion1->GetXaxis()->SetTitle("#theta");
    hinversion1->GetYaxis()->SetTitle("f(#theta)");
    hinversion1->Draw("HIST");
    hrejection1->Draw("HIST SAME");
    hfunction1->Draw("HIST SAME");
    func->Draw("SAME");

    auto legend = new TLegend(0.7,0.7,0.9,0.9);
    legend->AddEntry(hrejection1, "Rejection", "l");
    legend->AddEntry(hinversion1, "Inversion", "l");
    legend->AddEntry(hfunction1, "ROOT std", "l");
    legend->AddEntry(func, "Function", "l");
    legend->Draw();
}