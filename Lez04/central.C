#include <Riostream.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TRandom3.h>
using namespace std;

/*
Consideriamo un parametro w con valore tra [0,0.5]
Definiamo una densità di probabilità:

f(x) = 1/(2w) se 0 <= x <= w
f(x) = 1/(2w) se 1-w <= x <= 1
f(x) = 0 altrimenti

Dimostriamo che la media di questa distribuzione è 0.5
E(X) = \int x f(x) dx = \int[0,w] x/(2w) dx + \int[1-w,1] x/(2w) dx

Dimostriamo che la varianza di questa distribuzione è (1-2w+w^2)/12
E(X^2) = \int x^2 f(x) dx = \int[0,w] x^2/(2w) dx + \int[1-w,1] x^2/(2w) dx = 1/(6w)(w^3 + 1 - (1-w)^3)
Var(X) = E(X^2) - E(X)^2 = 1/6 (2w^2 - 3w + 3) - 1/4

1) campioniamo N=2e6 valori di x con w=0.2, istogramma con 100 bin
2) vediamo media e varianza campionarie
3) sommiamo N variabili x_i e vediamo che la distribuzione della somma tende a gaussiana (CLT) con N = boh
*/


/*
    Per il campionamento utilizziamo una sequenza di numeri pseudo-casuali.

    Se utilizziamo più volte una funzione che genera numeri casuali, vogliamo che la sequenza di numeri generati sia diversa ogni volta.
    La sequenza che utilizzano i generatori è unica, ciò che cambia è il punto di partenza, detto seed. Se utilizziamo lo stesso seed, otteniamo la stessa sequenza.
    la libreria ROOT utilizzata è TRandom3

    TRandom3 *myptr = new TRandom3(36482);
    cout << myptr->Rndm();

    NOTA: se usiamo due puntatori con lo stesso seed otteniamo la stessa sequenza. Quello che fa ROOT è definire un TRandom3 globale,
    i puntatori globali di ROOT iniziano tutti con la lettera g
    cout << gRandom->Rndm();
    In questo modo siamo sicuri di non avere due sequenze tra loro correlate
*/

double random1(const double& w)
{
    double r = gRandom->Rndm();
    return (r<0.5) ? (2.*w*r):(1. - 2.*w*(1.-r));
}

double random2(const double& w)     //estremamente inefficiente
{
    double r = gRandom->Rndm();
    while(r>w && r<(1.-w)) r = gRandom->Rndm();
    return r;
}

void createhisto(const double& w, const int& entries, const int& Nsum)
{
    string name = "Sum of " + to_string(Nsum);
    string cname = "canvas" + to_string(Nsum);
    string hname = "hist" + to_string(Nsum);

    TCanvas* canvas = new TCanvas(cname.c_str(), "Teorema Limite Centrale", 800, 600);
    canvas->SetFrameFillColor(17);

    double mean = 0.5 * Nsum;
    double stdev = sqrt(((1./6.)*(2.*w*w - 3.*w + 3) - 0.25) * (double)Nsum);

    TH1D* hist = new TH1D(hname.c_str(), name.c_str(), 100, mean-5.*stdev, mean+5.*stdev);
    hist->SetLineColor(kGray+3);
    hist->SetLineWidth(3);
    hist->SetFillColorAlpha(kOrange+5, 0.75);
    for(int i=0; i<entries; i++)
    {
        double sum = 0.;
        for(int j=0; j<Nsum; j++)    sum += random1(w);
        hist->Fill(sum);
    }

    hist->Draw();
}

void central(const double& w = 0.2, const int& entries = 2000000)
{
    if(w<0 || w>0.5)
    {
        cout << "w deve essere compreso tra 0 e 0.5" << endl;
        return;
    }

    createhisto(w, entries, 300);
    createhisto(w, entries, 200);
    createhisto(w, entries, 100);
    createhisto(w, entries, 50);
    createhisto(w, entries, 10);
    createhisto(w, entries, 5);
    createhisto(w, entries, 2);
    createhisto(w, entries, 1);
}