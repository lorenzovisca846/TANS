#include <Riostream.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
using namespace std;

void esercizio1_v2(const string& fimpName, const string& histName, const unsigned int limit = 100000)
{
    ifstream in(fimpName);
    if (!in)
    {
        cout << "Il file " << fimpName << " non esiste." << endl;
        return;
    }

    double x, min, max;
    int count = 0;
    vector<double> data; 

    if(in >> x)
    {
        data.push_back(x);
        min = x;
        max = x;
        count++;
    }
    else
    {
        cout << "Il file " << fimpName << " e' vuoto." << endl;
        return;
    }
    while(in >> x && count < limit)
    {
        count++;
        data.push_back(x);
        if(x < min) min = x;
        if(x > max) max = x;
    }

    in.close();

    cout << "\nDati letti: " << data.size() << "\nEstremi dell'istogramma: (" << min << ", " << max << ")" << "\n\n";

    TH1D* hist;
    hist = new TH1D("hist", "Istogramma", 100, min, max);

    for(int i=0; i<data.size(); i++)
        hist->Fill(data[i]);

    hist->Draw();

    TFile file(histName.c_str(), "recreate");
    hist->Write();
    file.Close();
}