#include "TCanvas.h"
#include "TF1.h"
#include "TFunction.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TStyle.h"

#include <iostream>
#include <string>

Double_t lin_regr(Double_t *x, Double_t *par) {
  Float_t xx = x[0];
  Double_t a = par[0]; // intercetta
  Double_t b = par[1]; // pendenza

  return a + b * xx;
}

void run() {

  TF1 *f_fit = new TF1("f_fit", lin_regr, 0, 900, 2);

  Double_t a = 0;
  Double_t b = 1;
  f_fit->SetParameter(0, a);
  f_fit->SetParameter(1, b);

  f_fit->SetParName(0, "intercetta");
  f_fit->SetParName(1, "pendenza");

  // f_fit->SetParLimits(0, -100, 100);
  // f_fit->SetParLimits(1, 0.9943, 1);

  std::string filename = "cal.txt";
  gStyle->SetOptFit(0);

  TCanvas *c1 = new TCanvas("c1", "Calibration results", 720, 720);
  c1->cd();
  c1->SetGridx();
  c1->SetGridy();

  // per come leggere i file "root lezione III" del primo anno
  TGraphErrors *graph =
      new TGraphErrors(filename.c_str(), "%lg %lg %lg %lg", "");
  graph->SetTitle("Calibrazione dell'oscilloscopio;V_{MM} (mV);V_{osc} (mV)");
  // graph->SetLineColor(kBlue);
  graph->SetMarkerStyle(20);
  graph->SetMarkerColor(kBlue);
  graph->SetLineWidth(2);
  // su "root lezione II" del primo anno si trovano gli stili

  graph->Fit("f_fit", "R");

  // graph->Draw("APEL");
  graph->Draw("APE");

  TF1 *fitRes = graph->GetFunction("f_fit");

  std::cout << "           Chi^2: " << fitRes->GetChisquare() << '\n'
            << "             NDF: " << fitRes->GetNDF() << '\n'
            << "       Chi^2/NDF: " << fitRes->GetChisquare() / fitRes->GetNDF()
            << '\n'
            << "     Probabilità: " << fitRes->GetProb() << '\n'
            << "      Intercetta: " << fitRes->GetParameter(0) << " ± "
            << fitRes->GetParError(0) << '\n'
            << "        Pendenza: " << fitRes->GetParameter(1) << " ± "
            << fitRes->GetParError(1) << '\n';

  return;
}