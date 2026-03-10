#include <TFile.h>
#include <TH1.h>
#include <TKey.h>
#include <TCanvas.h>
#include <TString.h>
#include <TF1.h>
#include <iostream>
#include <map>
#include <utility>
#include <cmath>
#include <iomanip>

using namespace std;

void fitHistograms(fName ="histograms.root")
{
    TFile *f = TFile::Open(fName, "READ");
    if (!f || f->IsZombie()) {
        cerr << "Errore apertura file\n";
        return;
    }


    //Questo metodo legge gli istogrammi dal file f
    TH1D *h = (TH1D*)f->Get("nome_istogramma");

    //disegna prima di fittare, altrimenti crea casino
    h->Draw();


    //Procedura di fit
    //1) troviamo l'intervallo corretto del fit
    
    int maxBin = h->GetMaximumBin(); //trova il bin a cui c'è il massimo GLOBALE dell'istogramma 
    //se l'istogramma ha più massimi e a te ne interessa uno locale 
    //tipo hai un picco a zero e a te interessa il picco più basso a 100 MeV,
    //allora restringi l'intervallo e poi trovi il massimo, quindi:

    //h->GetXaxis()->SetRangeUser(20, 300); //restringe l'intervallo dell'asse x tra 20 e 300
    //int maxBin = h->GetMaximumBin(); //trova il massimo in quell'intervallo

    double maxX  = h->GetBinCenter(maxBin); //trovi il valore X di quel massimo (GetMaximumBin trova solo il bin, a te serve il valore)
    double rms   = h->GetRMS(); //prendiamo la RMS che è utile per calcolare il range di fit
    //se per qualche motivo la RMS non è definita usiamo tutto l'intervallo xmax - xmin
    if(rms<=0) rms = (h->GetXaxis()->GetXmax() - h->GetXaxis()->GetXmin())/10.0;

    //ora definiamo un intervallo di fit come
    // max +/- frazione * rms
    //in cui la frazione va decisa per ogni esercizio

    double xFitMin = maxX - 0.2*rms;
    double xFitMax = maxX + 0.5*rms;

    //Di nuovo un paio di controlli:
    //se xFitMin o xFitMax cadono fuori dall'intervallo dell'istogramma, 
    //allora prendiamo la massima estensione dell'istogramma come limite
    xFitMin = max(xFitMin, e.second.first->GetXaxis()->GetXmin());
    xFitMax = min(xFitMax, e.second.first->GetXaxis()->GetXmax());
    //e se per qualche motivo xFitMax viene minore di xFitMin
    //di nuovo prendiamo gli estremi dell'istogramma
    if(xFitMax <= xFitMin) {
        xFitMin = e.second.first->GetXaxis()->GetXmin();
        xFitMax = e.second.first->GetXaxis()->GetXmax();
    }
    //Ora, questi controlli sono praticamente sempre superflui in esercizi semplici,
    //ma quando si fittano centinaia di istogrammi tutti insieme possono tornare utili


    //Definizione della funzione di fit:
    //sintassi:
    //TF1 *f = new TF1("nome", "tipo", minimo, massimo);
    //il tipo di funzione può essere gaus (gaussiana), landau (landau), pol0 (polinomio grado 0), pol1 (grado 1) etc
    TF1 *func = new TF1("nome_funzione", "gaus", xFitMin, xFitMax);
    //imposta dei parametri di base
    //per la gaussiana:
    func->SetParameter(0, 1.); //ampiezza del picco 
    func->SetParameter(1, maxX); //valore medio, usiamo maxX che abbiamo trovato prima
    func->SetParameter(2, rms/2.0); //sigma, usiamo mezza RMS che abbiamo trovato prima
    func->SetLineColor(kMagenta);
    //questi sono valori di riferimento da cui parte il fit

    //e ora fittiamo:
    h->Fit(func, "R");
    //sintassi: istogramma->Fit(variabileFunzione, "opzioni");
    //le opzioni sono varie, noi usiamo solo "R" che specifica che il fit va fatto in Range
    //cioè, nell'intervallo in cui abbiamo definito la funzione (xFitMin, xFitMax) nel nostro caso

    //la funzione viene disegnata automaticamente, se non lo fosse aggiungi
    //func->Draw("same");
}




