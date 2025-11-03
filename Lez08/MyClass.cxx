#include <Riostream.h>
#include "MyClass.h"
#include "TString.h"


ClassImp(MyClass);  //Implementazione della classe MyClass per ROOT


MyClass::MyClass():                                                     //Default constructor, richiamiamo anche il default constructor della classe madre TObject
    TObject(),                                          
    fHeight(0.),
    fWeight(0.),
    fAge(0),
    fMale(false),
    fSize(0),
    fInfo(NULL)
    {
        cout << "DEFAULT CONSTRUCTOR - this= " << this << endl;         // this è un puntatore all'oggetto che sta venendo creato. Con questo cout possiamo vedere ogni volta che viene chiamato il default constructor
    }

MyClass::MyClass(double s, double p, int e, bool sex): TObject(),       //Costruttore con argomenti
    fHeight(s),
    fWeight(p),
    fAge(e),
    fMale(sex),
    fSize(0),
    fInfo(NULL)
    {
        cout << "DEFAULT CONSTRUCTOR - this= " << this << endl;     // this è un puntatore all'oggetto che sta venendo creato. Con questo cout possiamo vedere ogni volta che viene chiamato il default constructor
    }

MyClass::MyClass(const MyClass &source):                            //Copy constructor, prende in ingresso il riferimento all'oggetto da copiare  
    TObject(source),                                                //Richiamiamo il copy constructor della classe madre TObject per copiare la parte di TObject dell'oggetto source
    fHeight(source.fHeight),                                        //Copio il data member fHeight dall'oggetto source all'oggetto che sto creando. Posso accedere ad un data member privato perché sono nella stessa classe
    fWeight(source.fWeight),
    fAge(source.fAge),
    fMale(source.fMale),
    fSize(source.fSize)
    {
        cout << "COPY CONSTRUCTOR - from " << &source << " to this: " << this << endl; //Stampa indirizzo della sorgente e indirizzo della copia
        if(fSize>0)
        {
            fInfo = new int[fSize];                                 //Alloco un nuovo array sull'heap per il data member fInfo dell'oggetto copiato
            for(int i=0; i<fSize; i++)
                fInfo[i] = source.fInfo[i];                         //Copio i singoli valori dall'array dell'oggetto source al nuovo array dell'oggetto copiato
                                                                    //Nota: qui dentro potrei modificare i valori dell'oggetto originale, perché viene passato const il riferimento fInfo, non l'array. Chiaramente, non va fatto
                                                                    //Quindi potrei (NON FARLO!) fare source.fInfo[2] = 7; ma non ad esempio source.fHeight = 1.8; (perchè invece fHeight è effettivamente passato const)
        }
        else
            fInfo = NULL;                                           //Se il source non alloca memoria, non la alloco nemmeno nella copia
    } 
    
MyClass::~MyClass()                                                 //Destructor
    {
        cout << "DESTRUCTOR - this= " << this << endl;              // this è un puntatore all'oggetto che sta venendo distrutto. Con questo cout possiamo vedere ogni volta che viene chiamato il destructor
        if(fSize>0) delete []fInfo;                                 // se fInfo ha memoria allocata, la libero
    }




//OPERATORE DI ASSEGNAZIONE. Vediamo che è molto simile al copy constructor
/*
MyClass& MyClass::operator=(const MyClass &source)                  //Operatore di assegnazione
    {
        cout << "ASSIGNMENT OPERATOR - from " << &source << " to this: " << this << endl; //Stampa indirizzo della sorgente e indirizzo della copia
        if(this == &source) return *this;                           //Se sto facendo a = a 

        if(fSize>0) delete []fInfo;                                 //Se l'oggetto da sovrascrivere ha memoria allocata, la libero
        TObject::operator=(source);                                 //Richiamo l'operatore di assegnazione della classe madre TObject per copiare la parte di TObject dell'oggetto source
        fHeight = source.fHeight;                                   //Copio i data members dall'oggetto source all'oggetto corrente
        fWeight = source.fWeight;
        fAge    = source.fAge;
        fMale   = source.fMale;
        fSize   = source.fSize;
        if(fSize>0)
        {
            fInfo = new int[fSize];                                 //Alloco un nuovo array sull'heap
            for(int i=0; i<fSize; i++)
                fInfo[i] = source.fInfo[i];                         //Copio i singoli valori dall'array dell'oggetto source al nuovo array dell'oggetto corrente
        }
        else
            fInfo = NULL;                                           //Se il source non alloca memoria, non la alloco nemmeno nell'oggetto corrente
    }
*/

MyClass& MyClass::operator=(const MyClass &source)                  //Operatore di assegnazione
    {
        if(this == &source) return *this;
        cout << "ASSIGNMENT OPERATOR - from " << &source << " to this: " << this << endl;
        this->~MyClass();
        new (this) MyClass(source); 
        cout << "ASSIGNMENT OPERATOR - after placement new, this= " << this << endl;
        return *this;
        
        // Sfrutto la somiglianza con il copy constructor: quello che viene fatto con questo metodo è distruggere l'oggetto corrente e poi ricrearlo allo stesso indirizzo di memoria con il copy constructor dalla source
    }

double MyClass::GetHeight() const
    {
        return fHeight;
    }

double MyClass::GetWeight() const
    {
        return fWeight;
    }

void MyClass::PrintStatus() const
    {
        //printout

        cout << "\nFun<ione PrintStatus di MyClass\n" << endl;
        TString sex = (IsMale()? "F" : "M");

        cout << "Eta'= " << fAge << ", sesso = " << sex.Data() << ", statura= " << fHeight << ", peso= " << fWeight << endl;

        if(fSize>0)
        {
            cout << "\nUlteriori informazioni: ";
            for(int i=0; i<fSize; i++)
                cout << fInfo[i] << " ";
            cout << endl;
        }
    }

void MyClass::SetArray(int *vt, int sz)
    {
        if(fSize>0) delete []fInfo;                                 //Se l'array era già stato allocato, libero la memoria
            
        fInfo = new int[sz];
        fSize = sz;
        for(int i=0; i<sz; i++)
            fInfo[i] = vt[i];
    }


/*
Provare i vari metodi:

MyClass *A = new MyClass(183,80,23,true);

myClass *B = new MyClass(*A);                   //copy constructor

MyClass C;                                      //assegnazione
    C = *A;

*/