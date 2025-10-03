/*
CODICE 1: gestione di file di input e output in C++ (senza ROOT)
*/


#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void ioexample(string fimpName, string foupName)
{
    ifstream in(fimpName);
    if (!in)
    {
        cout << "Il file " << fimpName << " non esiste." << endl;
        return;
    }

    ofstream fout(foupName);
    if (!fout)
    {
        cout << "Non posso aprire il file di output " << foupName << endl;
        return;
    }

    double number1, number2;
    int count = 0;

    while (in >> number1 >> number2)
    {
        count++;
        
        cout << count << ") ";
        cout << "Number 1 = " << number1 << ";\t Number 2 = " << number2 << endl;
        
        fout << count << ") ";
        fout << "Number 1 = " << number1 << ";\t Number 2 = " << number2 << endl;
    }

    in.close();
    fout.close();
}