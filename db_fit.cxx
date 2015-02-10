/*            _   __   _   _   _    _        
	|  | |   |  | | \ |   | \  /          
	|  | |   |  | | | |   |_/  \_
        |__| |_  |__| | | |_  |\     \
        |  | |   |  | | | |   | \    |
        |  | |__ |  | |_/ |__ |  \ __/
*/

//ROOT


#include<TChain.h>
#include<TH1F.h>
#include<RAT/DS/Root.hh>
#include<RAT/DS/CAL.hh>
#include<RAT/DS/PMT.hh>
#include<RAT/DS/Pulse.hh>
#include<RAT/DS/Block.hh>
#include<TCanvas.h>
#include<TApplication.h>
#include<TH2F.h>
#include<TFile.h>
#include<TMath.h>>
#include<RAT/DS/TS.hh>
#include<TLine.h>
#include<TBox.h>
#include<TAttLine.h>
#include<TAttFill.h>
#include<TColor.h>
#include<TText.h>
#include<TAttText.h>
#include<TLatex.h>
#include<TROOT.h>
#include<RAT/DS/CAL.hh>
#include<RAT/DS/QT.hh>
#include<TDirectory.h>
#include<TNtuple.h>
#include<TTree.h>
#include<RAT/CAENDigitizer.hh>
#include<TF1.h>


//C++
#include<iostream>
#include<stdio.h>
#include<algorithm>
#include<string>
#include<vector>
#include<sstream>
#include<math.h>

using namespace std;

#include "db_fit.h"


/*             __
        |\ /| |  | | |\  |
        | | | |  | | | | |
        |   | |__| | | | |
        |   | |  | | | | |
        |   | |  | | |  \|
*/

int main(int argc, char ** argv){
TApplication theApp("App",&argc,argv);

control_fct();

theApp.Run();

return 0;

}



/*       __   __  ___  _
        |    |     |  /
        |__  |     |  \_
        |    |     |    \
        |    |     |    |
        |    |__   |  __/
*/

//Controlling fct
void control_fct(){

comp_spectrum();
pmt_loop();

}

//Loop through all PMT plots
void pmt_loop(){
        TH1D *k[4];
        TFile *n[4];
        TPad *p[4];
        TF1 *f[4];

        TCanvas *ai = new TCanvas("at2","at2",1200,800);
        ai->Divide(2,2);

        for(int i =1; i<5; i++){

//Path strings
                string pmt_n = "pmt" + convertInt(i) + ".root";

                TString pmt_nm(pmt_n);

                string path = "/home/singhrao/Documents/macros/Dark_Box_analysis/db_analysis/db_data/delta_t/" + pmt_n;

                TString r_path(path);


                string pmt_id = "PMT" + convertInt(i);
                TString r_pmt_id(pmt_id);

                string fit_name = "PMT_fit" + convertInt(i);
                TString sfit_name(fit_name);

//File acquisition
                n[i] = new TFile(r_path);
                k[i] = (TH1D*)n[i]->Get(r_pmt_id);
//Fit parameters
                f[i] = new TF1(sfit_name,lin_spectra_fct,0,1E9,10);

                f[i]->SetParameters(4e-06,0.02, 1.8e+03, 1.32688e+03, 0.036e-00,7e+03,1.5e+03);;

                f[i]->SetParNames("DNrate","AP1prob","AP1mean","AP1sigma","AP2prob","AP2mean","AP2sigma");

                p[i] = (TPad*)(ai->cd(i));
                k[i]->Draw();
                //k[i]->Fit(sfit_name);
                p[i]->SetLogx();
                k[i]->GetXaxis()->SetRangeUser(2E2,2E6);
		k[i]->GetXaxis()->SetTitle("delta_t (ns)");
		k[i]->GetYaxis()->SetTitle("Probability/ns");
        }


}


//Fit of complete spectrum
void comp_spectrum(){
        TH1D *cs;
        TFile *csf;
        TF1 *csfit;
        TPad* pad;
//Canvas and path definitions
        TCanvas *a2 = new TCanvas("at1","at1");
        a2->Divide(1,1);
        string path = "/home/singhrao/Documents/macros/Dark_Box_analysis/db_analysis/db_data/delta_t/delta_pmt_t.root";
        TString r_path(path);

//Plot acquistion
        csf = new TFile(r_path);
        cs = (TH1D*)csf->Get("delta_pmt_t");

//Fit parameters
	Double_t range_min = 0.8E3;
        Double_t range_max = 1E9;
        csfit = new TF1("complete_fit",lin_spectra_fct,range_min,range_max,10);
//three afterpulse fit
        csfit->SetParameters(4e-06,0.02, 1.8e+03, 1.32688e+03, 0.036e-00,7e+03,1.5e+03);
	csfit->SetParNames("DNrate","AP1prob","AP1mean","AP1sigma","AP2prob","AP2mean","AP2sigma");
/*	
	csfit->SetParameter(7,0.015);
	csfit->SetParName(7,"f1prob");
	csfit->SetParameter(8,2e+03);
	csfit->SetParName(8,"f1mean");
	csfit->SetParameter(9,1.5e+03);
	csfit->SetParName(9,"f1sigma");
  */      

//Complete plot
        pad = (TPad*)(a2->cd(1));
        pad->SetLogx();
        cs->GetXaxis()->SetRangeUser(2E2,2E6);
        //cs->Fit("complete_fit","R");
	cs->Draw();
	//csfit->Draw("same");

	cs->GetXaxis()->SetTitle("delta_t (ns)");
	cs->GetYaxis()->SetTitle("Probability/ns");

}

//Fit function
double lin_spectra_fct(double *x, double *p){

        double DN,AP1,AP2,nDN,nAP1,nAP2;
	//features

	double f1,f2,f3,f4,f5; 
	double nf1,nf2,nf3,nf4,nf5;  
//fcts
	DN = p[0]*TMath::Exp(-p[0]*x[0]);

        AP1 = p[1]*TMath::Exp(-1* pow( (x[0] - p[2]),2 ) / (2*pow( (p[3]) ,2)))*(1 /(TMath::Sqrt(2*TMath::Pi())*p[3] )) ;

        AP2 = p[4]*TMath::Exp(-1* pow( (x[0] - p[5]),2 ) / (2*pow( (p[6]) ,2)))/(TMath::Sqrt(2*TMath::Pi())*p[6] );

	f1 = p[7]*TMath::Exp(-1* pow( (x[0] - p[8]),2 ) / (2*pow( (p[9]) ,2)))/(TMath::Sqrt(2*TMath::Pi())*p[9] );


//Anti fcts
        nDN = 1+TMath::Exp(-x[0]*p[0]);
        nAP1 = 1-0.5*p[1]*(1+TMath::Erf((x[0] - p[2]) /(TMath::Sqrt(2)*p[3])));
        nAP2 = 1-0.5*p[4]*(1+TMath::Erf((x[0] - p[5]) /(TMath::Sqrt(2)*p[6])));

	nf1 = 1-0.5*p[7]*(1+TMath::Erf((x[0] - p[8]) /(TMath::Sqrt(2)*p[9])));
//fit construction


//DN and two afterpulses ONLY
//	return DN*nAP2 + AP2*nDN;	
	return DN*nAP1*nAP2 + AP2*nDN*nAP1 +  AP1*nDN*nAP2;

//Return ap and feature between ap1 and ap2s
//	return DN*nAP1*nAP2*nf1 + AP2*nAP1*nDN*nf1 + f1*nDN*nAP1*nAP2 + AP1*nf1*nAP2*nDN;



}//spectra fit

//convertInt function
string convertInt(int number){
        stringstream ss;
        ss<< number;
        string sss = ss.str();
        return sss;

}//string conversion






