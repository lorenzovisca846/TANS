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



/*
Ora vediamo un altro modo per mettere l'istogramma nel file. Questo metodo è utile nel caso in cui volessimo salvare più oggetti nel file senza scrivere Write(); per ogni oggetto.
Semplicemente dichiarando il file prima di creare l'istogramma, l'istogramma viene automaticamente associato al file appena creato.
*/    

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
/*
Utilizzando file.Write() senza argomenti, vengono scritti tutti gli oggetti associati al file.
*/
    file.Close();


/*
NOTA DI SERVIZIO
Se avessimo dichiarato hist nella stack con:
    TH1D hist("hist", "Istogramma", 100, -7., 7.);

Invece che hist.Draw(); dovremmo utilizzare hist.DrawCopy(); perché altrimenti l'oggetto hist muore appena l'esecuzione arriva a "}" e non possiamo più usarlo.
DrawCopy() crea una copia nell'heap dell'istogramma e la disegna, quindi possiamo usare l'istogramma anche dopo la fine della funzione.
*/



//ESERCIZIO: creare l'istogramma senza dichiarare a priori l'intervallo (-7,7) ma a posteriori.
}