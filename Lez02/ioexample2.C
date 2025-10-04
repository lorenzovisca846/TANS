#include <Riostream.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
using namespace std;




void ioexample2(const string& fimpName, const string& histName)
{
/*
Invece che usare string usiamo const string& nella definizione della funzione.
Se usassimo string staremmo creando sullo stack una copia della stringa passata come argomento alla funzione dal programma chiamante (passaggio "by value").
Non è un problema per le stringhe, ma se stessimo passando oggetti grandi (come istogrammi) sarebbe uno spreco di memoria notevole.

Usando string& stiamo passando alla funzione un puntatore alla stringa originale (passaggio "by reference"), quindi ogni azione fatta qui dentro si riflette sulla stringa originale.
Aggiungere const davanti ad un oggetto passato by reference garantisce che l'oggetto non venga modificato all'interno della funzione (dà errore altrimenti).
*/



    ifstream in(fimpName);
    if (!in)
    {
        cout << "Il file " << fimpName << " non esiste." << endl;
        return;
    }
//Exception handling per il file di input (controlla se esiste)



    TH1D* hist;
    hist = new TH1D("hist", "Istogramma", 100, -7., 7.);
/*
Crea un istogramma con nome hist, titolo Istogramma, 100 bin tra -7 e +7
Analogo a TH1D* hist = new TH1D("hist", "Istogramma", 100, -7., 7.);

Utilizziamo l'allocazione dinamica: utilizziamo TH1D* e l'operatore new
Utile per oggetti grandi, la heap e' piu' grande della stack.
Ciò che viene definito nello stack viene distrutto quando si esce dallo scope (chiusa la graffa), mentre ciò che viene definito nella heap rimane in memoria fino a quando non viene cancellato manualmente con il comando "" delete; "".
NOTA: Se creiamo troppi oggetti nella heap senza cancellarli, si ha una perdita di memoria (memory leak).

Quando si esce dallo scope il puntatore viene quindi distrutto, l'oggetto a cui fa riferimento no a meno che non sia cancellato manualmente con "" delete hist; ""
Quando finisce l'esecuzione del programma noi continuiamo a vedere l'istogramma e possiamo lavorarci sopra, perché ROOT ha una lista che tiene traccia degli oggetti creati nella heap.
ROOT si tiene ancora un puntatore a quell'oggetto e lo distrugge alla fine dell'esecuzione del programma.

Visto che ROOT si occupa della pulizia della memoria noi non dobbiamo utilizzare "" delete; "", perché altrimenti quando ci prova lui cerca di cancellare un oggetto che non esiste più e si ha un crash del programma.
*/



    int count = 0;
    double x;
    while(in >> x)
    {
        count++;
        hist->Fill(x);
    }
/*
Finché riesce a leggere un numero dal file di input lo memorizza in x e lo aggiunge all'istogramma, e aumenta il counter dei numeri letti.

La freccia "->" vuol dire che si sta utilizando un metodo (Fill) appartenente alla classe dell'oggetto a cui il puntatore (hist) fa riferimento.
Se avessimo creato l'istogramma senza puntatore, avremmo usato il punto "." per accedere ai metodi dell'oggetto.
*/



    cout << "Dati letti: " << count << endl;
    in.close();
//Chiude il file di input (in questo caso si usa . e non -> perché in non è un puntatore ma un oggetto) e dice il numero di dati letti.



    hist->Draw();
//Disegna l'istogramma, crea automaticamente una canvas se non esiste già una canvas aperta.



    TFile file(histName.c_str(), "recreate");
    hist->Write();
    file.Close();
/*
Crea un file chiamato histName (primo argomento) e lo apre in modalità "recreate" (se esiste già lo sovrascrive, altrimenti lo crea).
La funzione c.str() serve a convertire la stringa in un array di char, che è il tipo di dato richiesto dalla funzione TFile.
La funzione Write() scrive l'istogramma nella working directory corrente, ovvero l'ultimo file ROOT aperto (in questo caso il file appena creato).
Alla fine chiude il file. In questo caso Close ha la maiuscola: non è la funzione close() degli stream di C++, ma una funzione dedicata della classe TFile di ROOT.
*/

}