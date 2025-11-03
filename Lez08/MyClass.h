//Se la classe non è stata ancora definita, la definisco. In questo modo il compilatore definisce la classe una sola volta, la seconda volta non entra nell'if quindi salta tutto evitando di includere più volte la classe
#ifndef MYCLASS_H
#define MYCLASS_H

#include "TObject.h"

//Creo una classe che eredita da TObject, in questo modo posso usare le member functions di TObject come il Write su file.
//Esempio: definizione di dati biologici di una persona

class MyClass : public TObject {

    public:     //Ciò che viene dichiarato come public è accessibile dall'esterno della classe. Gli altri tipi sono private (si può accedere solo dall'interno della classe) e protected (si può accedere solo dall'interno della classe e dalle classi figlie)
                //È buona norma dichiarare private tutto ciò che non deve esplicitamente essere accessibile dall'esterno della classe

    MyClass();                                              //Default constructor: serve a dedicare la memoria per un oggetto della classe MyClass senza inizializzarne i data members
    Myclass(double s, double p, int e, bool sex);           //Costruttore con argomenti: serve a dedicare la memoria per un oggetto della classe MyClass e inizializzarne i data members
    MyClass(const MyClass &source);                         //Copy constructor: serve a copiare un oggetto della classe MyClass in un altro oggetto della stessa classe. Viene passato by reference perché il compilatore "non sa" ancora come creare la copia per il passaggio by value perché questa è proprio la funzione che definisce la copia. Non si usa se non si alloca memoria (vedremo poi)
    virtual ~MyClass();                                     //Destructor: serve a liberare la memoria occupata dall'oggetto della classe MyClass quando va out of scope. È virtual perché in questo modo se una classe figlia di MyClass viene distrutta, viene chiamato prima il distruttore della figlia e poi quello della madre. Deve essere inserito manualmente se si alloca memoria

    MyClass& operator=(const MyClass &source);              //Permette di effettuare l'operazione di assegnazione a=b su oggetti della classe

    void ChangeAge(int age) {fAge=age;}                     //Member function che cambia l'età. Esempio di implementazione in-line direttamente nell'header. In questo modo il compilatore "vede" subito la funzione e ottimizza la compilazione
    double GetHeight() const;                               //Member function che restituisce l'altezza. Tutte le member functions che non modificano i data members vengono dichiarate const
    double GetWeight() const;
    int GetAge() const {return fAge;}
    bool IsMale() const {return fMale;}
    void PrintStatus() const;
    int GetArray(int i) const {return (i>=0 && i<fSize)? fInfo[i] : -1;}
    void SetArray(int *vt, int sz);
    double& Altezza() {return fHeight;}

    private:
    double fHeight;         //Data member: altezza in metri
    double fWeight;         //Data member: peso in kg
    int fAge;               //Data member: età in anni
    bool fMale;             //Data member: true se maschio, false se femmina
    int fSize;              //Data member: dimensione dell'array di informazioni aggiuntive
    int *fInfo;             //Data member: array di informazioni aggiuntive. Questo data member alloca spazio sull'heap, motivo per cui servono il copy constructor e il distruttore manuale.

}; //Quando si chiude la graffa di definizione della classe serve ;

#endif // MYCLASS_H