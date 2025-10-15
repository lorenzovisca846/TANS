#include <Riostream.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>

// Libreria necessaria
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

    while(getline(in, line))        //Legge tutta la riga
    {
        istringstream iss(line);    //Crea uno stream dalla riga
        while(iss >> x)             //Mette in x i valori validi trovati nella riga prima di un eventuale errore
                                    //NOTA: se la riga contiene 1 2 3 4 pippo 5 6, mette in x 1,2,3,4 ma poi esce dal while saltando 5, 6
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