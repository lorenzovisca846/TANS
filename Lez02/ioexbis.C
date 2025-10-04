#include <Riostream.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
using namespace std;

void ioexbis(const string& fimpName, const string& histName)
{
    ifstream in(fimpName);
    if (!in)
    {
        cout << "Il file " << fimpName << " non esiste." << endl;
        return;
    }

    TFile file(histName.c_str(), "recreate");

    TH1D* hist;
    hist = new TH1D("hist", "Istogramma", 100, -7., 7.);

    int count = 0;
    double x;
    while(in >> x)
    {
        count++;
        hist->Fill(x);
    }

    cout << "Dati letti: " << count << endl;
    in.close();
    hist->Draw();
    file.Write();

    file.Close();
}