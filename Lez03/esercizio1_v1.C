#include <Riostream.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
using namespace std;

/*
Nota: facendo .L esercizio.C++ viene utilizzato il compilatore integrato di ROOT, inoltre crea una shared library:
- un file .pcm  (precompiled module)
- un file .d    (dependency file)
- un file .so   (shared object)
Si vede la differenza quando si usano codici pesanti, perché non c'è interpretazione ma compilazione: la compilazione avviene una volta sola, poi l'esecuzione avviene su un oggetto
già pronto ed è più veloce.

Utilizzando .L esercizio.C+ la macro viene compilata, ma viene utilizzata la shared library già esistente se è aggiornata (vengono controllati i timestamp).
Se la shared library non è aggiornata viene ricompilata.
Utilizzando .L esercizio.C++ la macro viene sempre ricompilata. Serve a forzare la ricompilazione ad esempio se si fa un upgrade del compilatore.


CREARE SCRIPT SHELL DI PULIZIA
rm -v *.d
rm -v *.pcm
rm -v *.so
rm -v *~


Abbiamo creato il file di output pippo.root, come lo apro?

Metodo 1) ci permette di vedere l'istogramma. Scrivere nel terminale di ROOT:
    new TBrowser();


Metodo 2) (molto più utilizzato) permette di accedere al file in un'altra macro:
    TFile *miofile = new TFile("pippo.root");
    
La classe TFile presenta delle member functions utili:
    miofile->ls();                                              per vedere il contenuto del file.
    TH1D *hh = (TH1D*) miofile->Get("hist");                    Get serve a estrarre l'istogramma dal file: il comando (TH1D*) serve per fare il cast, perché Get ritorna un TObject generico (classe base di ogni oggetto ROOT).
A questo punto possiamo lavorare con l'istogramma hh come vogliamo, ad esempio hh->Draw(); per vederlo.
hh però rimane linkato a miofile, se facciamo miofile->Close(); e delete miofile; perdiamo hh.
Per evitare questo possiamo fare hh->SetDirectory(NULL); che lo scollega dal file PRIMA di chiudere miofile.

*/


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
        cout << "Il file " << fimpName << " è vuoto." << endl;
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
    hist = new TH1D("hist", "Istogramma", 100, min, max);

    while(in >> x)
        hist->Fill(x);

    in.close();
    hist->Draw();


    TFile file(histName.c_str(), "recreate");
    hist->Write();
    file.Close();
}