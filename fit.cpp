#include "TCanvas.h"
#include "TF1.h"
#include "TFunction.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TStyle.h"

#include <iostream>
#include <string>

Double_t exp_dec(Double_t *x, Double_t *par) {
  Float_t xx = x[0];
  Double_t a = par[0]; // I_0
  Double_t b = par[1]; // hVT

  return a * (TMath::Exp(xx / b) - 1);
}

void fit(int id = 0) {
  // id = 1 Si, id = 2 Ge
  Double_t max;         // valore massimo di fit
  std::string filename; // nome del file
  std::string graphtitle;
  Double_t a = 0; // I_0
  Double_t b;     // hVT
  int ded;
  if (id == 1) { // Si
    max = 650;
    filename = "Si.txt";
    graphtitle = "Caratteristica I-V del SILICIO";
    b = 50;   
    ded = 30;
  } else if (id == 2) { // Ge
    max = 150;
    filename = "Ge.txt";
    graphtitle = "Caratteristica I-V del GERMANIO";
    b = 25;
    ded = 31;
  } else {
    std::cout << "id incorrect. Valid values are: 1(Si), 2(Ge))" << '\n';
    return;
  }

  TF1 *f_fit = new TF1("f_fit", exp_dec, 0, 750, 2);

  f_fit->SetParameter(0, a);
  f_fit->SetParameter(1, b);

  f_fit->SetParName(0, "I_0");
  f_fit->SetParName(1, "{#heta}V_T");

  // f_fit->SetParLimits(0, -100, 100);
  // f_fit->SetParLimits(1, 0.9943, 1);

  gStyle->SetOptFit(0);

  TCanvas *c1 = new TCanvas("c2", graphtitle.c_str(), 1280, 720);
  c1->cd();
  c1->SetLogy();
  c1->SetGridx();
  c1->SetGridy();

  // per come leggere i file "root lezione III" del primo anno
  TGraphErrors *graph =
      new TGraphErrors(filename.c_str(), "%lg %lg %lg %lg", "");

  graph->SetTitle(graphtitle.insert(ded, (";V (mV);I (mA)")).c_str());
  // graph->SetLineColor(kBlue);
  graph->SetMarkerStyle(20);
  graph->SetMarkerColor(kBlue);
  graph->SetLineWidth(2);
  graph->SetFillColor(6);
  // su "root lezione II" del primo anno si trovano gli stili

  graph->Fit("f_fit", "R", "", 0, max);

  graph->Draw("APE");
  //graph->Draw("3, same"); // per mostrare le fasce di errori
  f_fit->Draw("same");

  TF1 *fitRes = graph->GetFunction("f_fit");

  std::cout << "           Chi^2: " << fitRes->GetChisquare() << '\n'
            << "             NDF: " << fitRes->GetNDF() << '\n'
            << "       Chi^2/NDF: " << fitRes->GetChisquare() / fitRes->GetNDF()
            << '\n'
            << "     Probabilità: " << fitRes->GetProb() << '\n'
            << "             I_0: " << fitRes->GetParameter(0) << " ± "
            << fitRes->GetParError(0) << '\n'
            << "            hV_T: " << fitRes->GetParameter(1) << " ± "
            << fitRes->GetParError(1) << '\n';

  return;
}