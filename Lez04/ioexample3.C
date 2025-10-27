#include <Riostream.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <sstream>
using namespace std;

void ioexample3(const string& fimpName, const string& histName, const unsigned int limit = 100000)
{
    ifstream in(fimpName);
    if (!in)
    {
        cout << "Il file " << fimpName << " non esiste." << endl;
        return;
    }

    double x, min, max;
    vector<double> data; 

    string line;

    bool first = true;

    while(getline(in, line))
    {
        istringstream iss(line);
        while(iss >> x)
        {
            data.push_back(x);
            if (first)
            {
                min = x;
                max = x;
                first = false;
            }
            else
            {
                if(x < min) min = x;
                if(x > max) max = x;
            }
            
        }
    }
    if(first)
    {
        cout << "Non sono stati trovati dati validi." << endl;
        return;
    }

    in.close();

    if(data.size() == limit)
        cout << "WARNING: e' stato raggiunto il limite massimo di " << limit << " dati letti." << endl;

    cout << "\nDati letti: " << data.size() << "\nValori estremi: (" << min << ", " << max << ")" << "\n\n";

    TH1D* hist;
    hist = new TH1D("hist", "Istogramma", 100, min-1, max+1);

    for(auto y:data) hist->Fill(y);

    hist->Draw();

    TFile file(histName.c_str(), "recreate");
    hist->Write();
    file.Close();
}