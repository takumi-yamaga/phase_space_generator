
void PhaseSpace() {

   TLorentzVector target(0.0, 0.0, 0.0, 2.8084);
   TLorentzVector beam(0.0, 0.0, 1.0, sqrt(1.0*1.0+0.49*0.49));
   TLorentzVector W = beam + target;
   TVector3 boost = W.BoostVector();

   //(Momentum, Energy units are Gev/C, GeV)
   Double_t masses[3] = { 0.938, 1.115, 0.939 };

   TGenPhaseSpace event;
   event.SetDecay(W, 3, masses);
   std::cout << event.GetWtMax() << std::endl;

   TH1F *h1 = new TH1F("h1","h1", 100,0.0,1.0);
   TH2F *h21 = new TH2F("h21","h21", 50,2.0,3.0, 90,0.0,1.8);
   TH2F *h22 = new TH2F("h22","h22", 50,2.0,3.0, 90,0.0,1.8);

   Double_t max_weight = 0.;
   Double_t diff_max_weight = 1.;
   Int_t n_generate = 0;

   while(diff_max_weight>1e-5||n_generate<1e7){
      Double_t weight = event.Generate();
      if(weight>max_weight){
        diff_max_weight=weight-max_weight;
        max_weight=weight;
      }
      n_generate++;
      TLorentzVector *pProton = event.GetDecay(0);
      TLorentzVector *pLambda    = event.GetDecay(1);
      TLorentzVector *pNeutron    = event.GetDecay(2);

      TLorentzVector pPL = *pProton + *pLambda;
      h21->Fill(pPL.M(), pPL.Vect().Mag(),weight);
   }
   std::cout << max_weight << std::endl;
   std::cout << diff_max_weight << std::endl;
   std::cout << n_generate << std::endl;

   TRandom3* rndm = new TRandom3();
   for(Int_t n=0; n<1e7; n++){
     Double_t threshold;
     Double_t weight = 0.;
     Int_t n_loop=0;
     while(threshold>weight){
       weight=event.Generate();
       threshold = rndm->Uniform(max_weight);
       //if(n_loop>1e3){
       //  threshold=rndm->Uniform(max_weight);
       //  n_loop=0;
       //}
       n_loop++;
     }
     TLorentzVector *pProton = event.GetDecay(0);
     TLorentzVector *pLambda    = event.GetDecay(1);
     TLorentzVector *pNeutron    = event.GetDecay(2);

     TLorentzVector pPL = *pProton + *pLambda;
     h22->Fill(pPL.M(), pPL.Vect().Mag());
   }
   TCanvas* c1 = new TCanvas("c1");
   c1->cd();
   h21->Draw("col");
   TCanvas* c2 = new TCanvas("c2");
   c2->cd();
   h22->Draw("col");
   TCanvas* c3 = new TCanvas("c3");
   c3->cd();
   TH1F* h21_1 = (TH1F*)h21->ProjectionX();
   TH1F* h22_1 = (TH1F*)h22->ProjectionX();
   h22_1->SetLineColor(kRed);
   h22_1->Scale(h21_1->Integral()/h22_1->Integral());
   h21_1->Draw();
   h22_1->Draw("same");
}
