/*
COME SCRIVERE UNA CLASSE E IMPLEMENTARLA IN ROOT

Si utilizzano due file, convenzionalmente:
1) header file MyClass.h che dichiara i data members e le member functions
2) file di implementazione MyClass.cxx (oppure MyClass.cpp) in cui le member functions vengono implementate

L'idea è che dai programmi si dialoghi solamente con l'header che fa da interfaccia, il cxx non viene mai guardato.
*/


//Se la classe non è stata ancora definita, la definisco. In questo modo il compilatore definisce la classe una sola volta, la seconda volta non entra nell'if quindi salta tutto evitando di includere più volte la classe
#ifndef MYCLASS_H
#define MYCLASS_H

#include "TObject.h"

using namespace std;

//Creo una classe che eredita da TObject, in questo modo posso usare le member functions di TObject come il Write su file.
//Esempio: definizione di dati biologici di una persona

class MyClass : public TObject {

    public:     //Ciò che viene dichiarato come public è accessibile dall'esterno della classe. Gli altri tipi sono private (si può accedere solo dall'interno della classe) e protected (si può accedere solo dall'interno della classe e dalle classi figlie)
                //È buona norma dichiarare private tutto ciò che non deve esplicitamente essere accessibile dall'esterno della classe

    MyClass();                                              //Default constructor: serve a dedicare la memoria per un oggetto della classe MyClass senza inizializzarne i data members
    MyClass(double s, double p, int e, bool sex);           //Costruttore con argomenti: serve a dedicare la memoria per un oggetto della classe MyClass e inizializzarne i data members
    MyClass(const MyClass &source);                         //Copy constructor: serve a copiare un oggetto della classe MyClass in un altro oggetto della stessa classe. Viene passato by reference perché il compilatore "non sa" ancora come creare la copia per il passaggio by value perché questa è proprio la funzione che definisce la copia. Non si usa se non si alloca memoria (vedremo poi)
    virtual ~MyClass();                                     //Destructor: serve a liberare la memoria occupata dall'oggetto della classe MyClass quando va out of scope. È virtual perché in questo modo se una classe figlia di MyClass viene distrutta, viene chiamato prima il distruttore della figlia e poi quello della madre. Deve essere inserito manualmente se si alloca memoria

    MyClass& operator=(const MyClass &source);              //Permette di effettuare l'operazione di assegnazione a=b su oggetti della classe

    void ChangeAge(int age) {fAge=age;}                     //Member function che cambia l'età. Esempio di implementazione in-line direttamente nell'header. In questo modo il compilatore "vede" subito la funzione e ottimizza la compilazione
    double GetHeight() const;                               //Member function che restituisce l'altezza. Tutte le member functions che non modificano i data members vengono dichiarate const
    double GetWeight() const;                               //Member function che restituisce il peso
    int GetAge() const {return fAge;}                       //Member function che restituisce l'età. Esempio di implementazione in-line
    bool IsMale() const {return fMale;}                     //Member function che restituisce true se maschio, false se femmina. Esempio di implementazione in-line
    void PrintStatus() const;
    int GetArray(int i) const {return (i>=0 && i<fSize)? fInfo[i] : -1;}
    void SetArray(int *vt, int sz);
    double& Altezza() {return fHeight;}                     //In questo modo restituisco un riferimento al data member fHeight, quindi posso modificarlo direttamente dall'esterno della classe. Pericoloso!

    private:
    double fHeight;         // altezza in metri
    double fWeight;         // peso in kg
    int fAge;               // età in anni
    bool fMale;             // true se maschio, false se femmina
    int fSize;              // dimensione dell'array di informazioni aggiuntive
    int *fInfo;             //[fSize] array di informazioni aggiuntive. Questo data member alloca spazio sull'heap, motivo per cui servono il copy constructor e il distruttore manuale.

    ClassDef(MyClass,1);    // Istruzione di ROOT che permette di utilizzare un I/O "storico": il numero 1 indica la versione della classe. Se facciamo una nuova versione della classe (ad esempio aggiungendo data members) scriviamo 2 al posto di 1
                            // In questo modo se leggiamo un file scritto con la versione 1 della classe, ROOT sa che deve fare il "downgrade" alla versione 1
                            // (AUTOMATIC VERSIONING SYSTEM)

}; //Quando si chiude la graffa di definizione della classe serve ;

#endif // MYCLASS_H


/*
Operazione del copy constructor in presenza di *fInfo:
fInfo è un puntatore ad un array di dimensione fSize allocato sull'heap. Il default copy-constructor crea una copia di tutti gli oggetti con i valori associati:
ciò rappresenta un problema per *fInfo, perché il valore associato è l'indirizzo di memoria dell'array originale sull'heap. (SHALLOW COPY)
Quello che vogliamo è che il copy-constructor crei un nuovo array sull'heap e copi i valori dell'array originale in questo nuovo array (DEEP COPY), con il nuovo fSize puntatore a questo nuovo array.
*/



/*
GESTIONE DEI COMMENTI DI ROOT: vedi la Users Guide
//! Commento di una riga che non viene salvato nel file .root


*/

/*

Schifezze varie

lorenzo@MacBook-Air-di-Lorenzo-6 Lez08 % root MyClass.cxx++
root [0] 
Processing MyClass.cxx++...
Info in <TMacOSXSystem::ACLiC>: creating shared library /Users/lorenzo/Documents/TANS/Lez08/./MyClass_cxx.so
DEFAULT CONSTRUCTOR - this= 0x7d05855e0
(MyClass) Name: MyClass Title: Istruzione di ROOT che permette di utilizzare un I/O "storico": il numero 1 indica la versione della classe. Se facciamo una nuova versione della classe (ad esempio aggiungendo data members) scriviamo 2 al posto di 1
root [1] MyClass *ptrA = new MyClass(183,80,30,true);
DEFAULT CONSTRUCTOR - this= 0x7d7834940
root [2] int vt[10]={11,10,9,8,7,6,5,4,3,2};
root [3] ptrA->PrintStatus();

Fun<ione PrintStatus di MyClass

Eta'= 30, sesso = F, statura= 183, peso= 80
root [4] ptrA->SetArray(vt,10);
root [5] ptrA->PrintStatus();

Fun<ione PrintStatus di MyClass

Eta'= 30, sesso = F, statura= 183, peso= 80

Ulteriori informazioni: 11 10 9 8 7 6 5 4 3 2 
root [6] MyClass *ptrB = new MyClass(*ptrA);
COPY CONSTRUCTOR - from 0x7d7834940 to this: 0x7d7834ac0
root [7] MyClass C;
DEFAULT CONSTRUCTOR - this= 0x124ca40a8
root [8] C = *ptrB;
ASSIGNMENT OPERATOR - from 0x7d7834ac0 to this: 0x124ca40a8
DESTRUCTOR - this= 0x124ca40a8
COPY CONSTRUCTOR - from 0x7d7834ac0 to this: 0x124ca40a8
ASSIGNMENT OPERATOR - after placement new, this= 0x124ca40a8
root [9] delete ptrA;
DESTRUCTOR - this= 0x7d7834940
root [10] TFile *miofile = new TFile("ciccio.root","recreat");
Error in <TFile::TFile>: file /Users/lorenzo/Documents/TANS/Lez08/ciccio.root does not exist
root [11] TFile *miofile = new TFile("ciccio.root","recreate");
root [12] ptrB->Write("BB");
root [13] miofile->Close();
root [14] delete miofile;
root [15] .q








root [0] TFile *f = new TFile("ciccio.root");
Warning in <TClass::Init>: no dictionary for class MyClass is available
root [1] f->ls()
TFile**		ciccio.root	
 TFile*		ciccio.root	
  KEY: MyClass	BB;1	Istruzione di ROOT che permette di utilizzare un I/O "storico": il numero 1 indica la versione della classe. Se facciamo una nuova versione della classe (ad esempio aggiungendo data members) scriviamo 2 al posto di 1
root [2] .q
lorenzo@MacBook-Air-di-Lorenzo-6 Lez08 % root
root [0] .L MyClass.cxx+
Info in <TMacOSXSystem::ACLiC>: creating shared library /Users/lorenzo/Documents/TANS/Lez08/./MyClass_cxx.so
root [1] TFile *f = new TFile("ciccio.root");
root [2] f->ls()
TFile**		ciccio.root	
 TFile*		ciccio.root	
  KEY: MyClass	BB;1	Istruzione di ROOT che permette di utilizzare un I/O "storico": il numero 1 indica la versione della classe. Se facciamo una nuova versione della classe (ad esempio aggiungendo data members) scriviamo 2 al posto di 1
root [3] cout << ptr->Altezza() << endl;
input_line_15:2:33: error: use of undeclared identifier 'ptr'
 (((*(ostream*)0x20793bc60)) << ptr->Altezza() << endl)
                                ^
root [4] MyClass *ptr = static_cast<MyClass*>(f->Get("BB"));
DEFAULT CONSTRUCTOR - this= 0x9761eb1c0
root [5] cout << ptr->Altezza() << endl;
183
root [6] ptr->Altezza() = 184;
root [7] cout << ptr->Altezza() << endl;
184
root [8] 
*/