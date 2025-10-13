#include <Riostream.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
using namespace std;

void esercizio1_v1(const string& fimpName, const string& histName)
{
    ifstream in(fimpName);
    if (!in)
    {
        cout << "Il file " << fimpName << " non esiste." << endl;
        return;
    }
    double x, min, max;
    int count = 0;

    if(in >> x)
    {
        count++;
        min = x;
        max = x;
    }
    else
    {
        cout << "Il file " << fimpName << " e' vuoto." << endl;
        return;
    }

    while(in >> x)
    {
        count++;
        if(x < min) min = x;
        if(x > max) max = x;
    }

    in.clear();
    in.seekg(0, ios::beg);

    cout << "\nDati letti: " << count << "\nEstremi dell'istogramma: (" << min << ", " << max << ")" << "\n\n";

    TH1D* hist;

    double tol = (max-min)*0.01;
    hist = new TH1D("hist", "Istogramma", 100, min-tol, max+tol);

    while(in >> x)
        hist->Fill(x);

    in.close();
    hist->Draw();


    TFile file(histName.c_str(), "recreate");
    hist->Write();
    file.Close();
}