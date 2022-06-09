#include "Logger.h"

//std::string gundamOutFilePath{"../oa2020Official.json.root"};
std::string gundamOutFilePath{"../fit.allPars.json.root"};

std::string banffOutFilePath{"./postfitfile_Asimov.root"};

std::string outFilePath{"./compareBanff.root"};

//std::string gundamBaseFolder{"FitterEngine/postFit/Migrad/errors"};
std::string gundamBaseFolder{"FitterEngine/postFit/Hesse/errors"};

std::vector<std::string> banffParSetList{
  "Flux Systematics",
  "ND280 Detector Systematics",
  "Cross-Section Systematics",
  "Cross-Section Systematics (binned splines)"
};

std::map<std::string, double> banffParOffsets{
   {"MAQE", 1}
  ,{"2p2h_shape_C" ,1}
  ,{"2p2h_shape_O" ,1}
  ,{"CA5" ,1}
  ,{"MARES" ,1}
  ,{"ISO_BKG_LowPPi" ,1}
  ,{"ISO_BKG" ,1}
  ,{"CC_BY_DIS" ,1}
  ,{"CC_BY_MPi" ,1}
  ,{"CC_AGKY_Mult" ,1}
};

TFile* gundamFile;

TFile* banffFile;
TObjArray* banffNames;

void init();
int fetchHistParIndex(TH1D* hist_, const std::string& name_ );
std::pair<int, int> fetchGundamParIndex(int banffIndex_);

void compareBanff(){

  init();

  std::vector<int> parSetIndexOffsets;
  std::vector<std::string> banffNamesList;
  std::vector<double> prefitBanffValues;
  std::vector<double> prefitGundamValues;

  {
    std::cout << "Comparing prefit par values..." << std::endl;
    auto* banffPrefit = banffFile->Get<TVectorD>("prefit_params");

    TH1D* gundamPrefit;
    std::string pathBuffer;
    for( int iBanffPar = 0 ; iBanffPar < banffPrefit->GetNrows() ; iBanffPar++ ){
      std::pair<int, int> pairIndex = fetchGundamParIndex(iBanffPar);

      std::string path = Form("%s/%s/values/preFitErrors_TH1D", gundamBaseFolder.c_str(), banffParSetList[pairIndex.first].c_str());
      if(pathBuffer != path){
        std::cout << path << std::endl;
        parSetIndexOffsets.emplace_back(iBanffPar);
        pathBuffer = path;
        gundamPrefit = gundamFile->Get<TH1D>(pathBuffer.c_str());
        LogThrowIf(gundamPrefit==nullptr, "Invalid path: " << pathBuffer)
      }
      std::string parName = banffNames->At(iBanffPar)->GetName();
      double banffOffset = banffParOffsets[parName];

      banffNamesList.emplace_back(parName);
      prefitBanffValues.emplace_back((*banffPrefit)[iBanffPar]);
      prefitGundamValues.emplace_back(gundamPrefit->GetBinContent(pairIndex.second) + banffOffset);

      double diff = prefitGundamValues.back() - prefitBanffValues.back();

      if( diff != 0 ){
        LogError << banffParSetList[pairIndex.first] << "/" << parName << "/prefit-value: "
        << diff << " -> "
        << " gundam(" << prefitGundamValues.back() << ") - "
        << " banff(" << prefitBanffValues.back() << ")"
        << std::endl;
      }
      else{
        LogInfo << banffParSetList[pairIndex.first] << "/" << parName << "/prefit-value: " << "OK" << std::endl;
      }

    }
  }

  std::vector<double> prefitBanffErrors;
  std::vector<double> prefitGundamErrors;
  {
    std::cout << "Comparing prefit par errors..." << std::endl;
    auto* banffPrefit = banffFile->Get<TMatrixDSym>("prefit_cov");

    TH1D* gundamPrefit;
    std::string pathBuffer;
    for( int iBanffPar = 0 ; iBanffPar < banffPrefit->GetNrows() ; iBanffPar++ ){
      std::pair<int, int> pairIndex = fetchGundamParIndex(iBanffPar);

      std::string path = Form("%s/%s/values/preFitErrors_TH1D", gundamBaseFolder.c_str(), banffParSetList[pairIndex.first].c_str());
      if(pathBuffer != path){
        std::cout << path << std::endl;
        pathBuffer = path;
        gundamPrefit = gundamFile->Get<TH1D>(pathBuffer.c_str());
        LogThrowIf(gundamPrefit==nullptr, "Invalid path: " << pathBuffer)
      }
      std::string parName = banffNames->At(iBanffPar)->GetName();

      prefitBanffErrors.emplace_back(TMath::Sqrt( (*banffPrefit)[iBanffPar][iBanffPar] ));
      prefitGundamErrors.emplace_back(gundamPrefit->GetBinError(pairIndex.second));

      double diff = prefitGundamErrors.back() - prefitBanffErrors.back();

      if( diff != 0 ){
        LogError << banffParSetList[pairIndex.first] << "/" << parName << "/prefit-error: "
        << diff << " -> "
        << " gundam(" << prefitGundamErrors.back() << ") - "
        << " banff(" << prefitBanffErrors.back() << ")"
        << std::endl;
      }
      else{
        LogInfo << banffParSetList[pairIndex.first] << "/" << parName << "/prefit-error: " << "OK" << std::endl;
      }
    }
  }

  TFile* outFile = TFile::Open(outFilePath.c_str(), "RECREATE");
  {
    auto* outDir = GenericToolbox::mkdirTFile(outFile, "prefit");
    std::cout << "Building comparison histograms..." << std::endl;
    auto* banffPrefit = banffFile->Get<TVectorD>("prefit_params");

    int iOffset{-1};
    TH1D* banffHistBuff{nullptr};
    TH1D* gundamHistBuff{nullptr};

    auto writeFct = [&](){
      banffHistBuff->Write();
      gundamHistBuff->Write();

      auto* compCanvas = new TCanvas("compCanvas", "compCanvas", 1280, 720);
      banffHistBuff->SetFillColor(kRed-9);
      banffHistBuff->SetMarkerStyle(kFullDotLarge);
      banffHistBuff->SetMarkerColor(kRed-3);
      banffHistBuff->SetMarkerSize(0);
      banffHistBuff->SetLabelSize(0.02);
      banffHistBuff->SetTitle("BANFF prefit");
      banffHistBuff->GetXaxis()->SetLabelSize(0.03);
      banffHistBuff->GetXaxis()->LabelsOption("v");
      banffHistBuff->Draw("E2");

      TH1D banffHistLine = TH1D("banffHistLine", "banffHistLine",
                                banffHistBuff->GetNbinsX(),
                                banffHistBuff->GetXaxis()->GetXmin(),
                                banffHistBuff->GetXaxis()->GetXmax()
                                );
      GenericToolbox::transformBinContent(&banffHistLine, [&](TH1D* h_, int b_){
        h_->SetBinContent(b_, banffHistBuff->GetBinContent(b_));
      });

      banffHistLine.SetLineColor(kRed-3);
      banffHistLine.Draw("SAME");

      gundamHistBuff->SetLineColor(9);
      gundamHistBuff->SetLineWidth(2);
      gundamHistBuff->SetMarkerColor(9);
      gundamHistBuff->SetMarkerStyle(kFullDotLarge);
      gundamHistBuff->SetTitle("Gundam prefit");
      gundamHistBuff->Draw("E1 X0 SAME");

      gPad->SetGridx();
      gPad->SetGridy();

      TLegend l(0.7, 0.8, 0.9, 0.9);
      l.AddEntry(banffHistBuff, "BANFF prefit values");
      l.AddEntry(gundamHistBuff, "Gundam prefit values");
      l.Draw();

      banffHistBuff->SetTitle("Prefit parameters comparison");
      compCanvas->Write();
    };

    int histOffset{-1};
    for( int iBanffPar = 0 ; iBanffPar < banffPrefit->GetNrows() ; iBanffPar++ ){
      iOffset = GenericToolbox::findElementIndex(iBanffPar, parSetIndexOffsets);
      if( iOffset != -1 ){

        if( banffHistBuff != nullptr ){
          writeFct();
        }


        std::string parSetName = banffParSetList[iOffset];
        GenericToolbox::mkdirTFile(outDir, parSetName)->cd();

        int nPars = ( iOffset+1 != parSetIndexOffsets.size() ) ? parSetIndexOffsets[iOffset+1]: banffPrefit->GetNrows();
        std::cout << "nPars = " << nPars << std::endl;
        nPars -= parSetIndexOffsets[iOffset];
        std::cout << "nPars = " << nPars << std::endl;
        banffHistBuff = new TH1D(
          "banffHist_TH1D", Form("%s prefit parameters", parSetName.c_str())
          ,nPars, 0, nPars
        );
        banffHistBuff->SetLineColor(kRed);
        gundamHistBuff = new TH1D(
          "gundamHistBuff", Form("%s prefit parameters", parSetName.c_str())
          ,nPars, 0, nPars
        );
        histOffset = 0;
      }

      histOffset++;
      banffHistBuff->SetBinContent(histOffset, prefitBanffValues[iBanffPar]);
      banffHistBuff->SetBinError(histOffset, prefitBanffErrors[iBanffPar]);
      gundamHistBuff->SetBinContent(histOffset, prefitGundamValues[iBanffPar]);
      gundamHistBuff->SetBinError(histOffset, prefitGundamErrors[iBanffPar]);

      banffHistBuff->GetXaxis()->SetBinLabel(histOffset, banffNamesList[iBanffPar].c_str());

    }
    writeFct();
  }

  std::vector<double> postfitBanffValues;
  std::vector<double> postfitGundamValues;
  {
    std::cout << "Comparing postfit par values..." << std::endl;
    auto* banffPostfit = banffFile->Get<TVectorD>("postfit_params");

    TH1D* gundamPrefit;
    std::string pathBuffer;
    for( int iBanffPar = 0 ; iBanffPar < banffPostfit->GetNrows() ; iBanffPar++ ){
      std::pair<int, int> pairIndex = fetchGundamParIndex(iBanffPar);

      std::string path = Form("%s/%s/values/preFitErrors_TH1D", gundamBaseFolder.c_str(), banffParSetList[pairIndex.first].c_str());
      if(pathBuffer != path){
        std::cout << path << std::endl;
        pathBuffer = path;
        gundamPrefit = gundamFile->Get<TH1D>(pathBuffer.c_str());
      }
      std::string parName = banffNames->At(iBanffPar)->GetName();
      double banffOffset = banffParOffsets[parName];

      postfitBanffValues.emplace_back((*banffPostfit)[iBanffPar]);
      postfitGundamValues.emplace_back(gundamPrefit->GetBinContent(pairIndex.second) + banffOffset);

      double diff = postfitGundamValues.back() - postfitBanffValues.back();

      if( diff != 0 ){
        LogWarning << banffParSetList[pairIndex.first] << "/" << parName << "/postfit-value: "
        << diff << " -> "
        << " gundam(" << postfitGundamValues.back() << ") - "
        << " banff(" << postfitBanffValues.back() << ")"
        << std::endl;
      }
      else{
        LogInfo << banffParSetList[pairIndex.first] << "/" << parName << "/postfit-value: " << "OK" << std::endl;
      }

    }
  }

  std::vector<double> postfitBanffErrors;
  std::vector<double> postfitGundamErrors;
  {
    std::cout << "Comparing postfit par errors..." << std::endl;
    auto* banffPostfit = banffFile->Get<TMatrixDSym>("postfit_cov");

    TH1D* gundamPrefit;
    std::string pathBuffer;
    for( int iBanffPar = 0 ; iBanffPar < banffPostfit->GetNrows() ; iBanffPar++ ){
      std::pair<int, int> pairIndex = fetchGundamParIndex(iBanffPar);

      std::string path = Form("%s/%s/values/postFitErrors_TH1D", gundamBaseFolder.c_str(), banffParSetList[pairIndex.first].c_str());
      if(pathBuffer != path){
        std::cout << path << std::endl;
        pathBuffer = path;
        gundamPrefit = gundamFile->Get<TH1D>(pathBuffer.c_str());
      }
      std::string parName = banffNames->At(iBanffPar)->GetName();

      postfitBanffErrors.emplace_back(TMath::Sqrt( (*banffPostfit)[iBanffPar][iBanffPar] ));
      postfitGundamErrors.emplace_back(gundamPrefit->GetBinError(pairIndex.second));

      double diff = postfitGundamErrors.back() - postfitBanffErrors.back();

      if( diff != 0 ){
        LogWarning << banffParSetList[pairIndex.first] << "/" << parName << "/postfit-error: "
        << diff << " -> "
        << " gundam(" << postfitGundamErrors.back() << ") - "
        << " banff(" << postfitBanffErrors.back() << ")"
        << std::endl;
      }
      else{
        LogInfo << banffParSetList[pairIndex.first] << "/" << parName << "/postfit-error: " << "OK" << std::endl;
      }
    }
  }

  {
    auto* outDir = GenericToolbox::mkdirTFile(outFile, "postfit");
    std::cout << "Building comparison histograms..." << std::endl;
    auto* banffPostfit = banffFile->Get<TVectorD>("postfit_params");

    int iOffset{-1};
    TH1D* banffHistBuff{nullptr};
    TH1D* gundamHistBuff{nullptr};

    auto writeFct = [&](){
      banffHistBuff->Write();
      gundamHistBuff->Write();

      auto* compCanvas = new TCanvas("compCanvas", "compCanvas", 1280, 720);
      banffHistBuff->SetFillColor(kRed-9);
      banffHistBuff->SetMarkerStyle(kFullDotLarge);
      banffHistBuff->SetMarkerColor(kRed-3);
      banffHistBuff->SetMarkerSize(0);
      banffHistBuff->SetLabelSize(0.02);
      banffHistBuff->SetTitle("BANFF postfit");
      banffHistBuff->GetXaxis()->SetLabelSize(0.03);
      banffHistBuff->GetXaxis()->LabelsOption("v");
      banffHistBuff->Draw("E2");

      TH1D banffHistLine = TH1D("banffHistLine", "banffHistLine",
                                banffHistBuff->GetNbinsX(),
                                banffHistBuff->GetXaxis()->GetXmin(),
                                banffHistBuff->GetXaxis()->GetXmax()
                                );
      GenericToolbox::transformBinContent(&banffHistLine, [&](TH1D* h_, int b_){
        h_->SetBinContent(b_, banffHistBuff->GetBinContent(b_));
      });

      banffHistLine.SetLineColor(kRed-3);
      banffHistLine.Draw("SAME");

      gundamHistBuff->SetLineColor(9);
      gundamHistBuff->SetLineWidth(2);
      gundamHistBuff->SetMarkerColor(9);
      gundamHistBuff->SetMarkerStyle(kFullDotLarge);
      gundamHistBuff->SetTitle("Gundam postfit");
      gundamHistBuff->Draw("E1 X0 SAME");

      gPad->SetGridx();
      gPad->SetGridy();
//      gPad->SetBottomMargin(0.1*(1 + longestTitleSize/15.));

      TLegend l(0.7, 0.8, 0.9, 0.9);
      l.AddEntry(banffHistBuff, "BANFF postfit values");
      l.AddEntry(gundamHistBuff, "Gundam postfit values");
      l.Draw();

      banffHistBuff->SetTitle("Postfit parameters comparison");
      compCanvas->Write();
    };

    int histOffset{-1};
    for( int iBanffPar = 0 ; iBanffPar < banffPostfit->GetNrows() ; iBanffPar++ ){
      iOffset = GenericToolbox::findElementIndex(iBanffPar, parSetIndexOffsets);
      if( iOffset != -1 ){

        if( banffHistBuff != nullptr ){
          writeFct();
        }


        std::string parSetName = banffParSetList[iOffset];
        GenericToolbox::mkdirTFile(outDir, parSetName)->cd();

        int nPars = ( iOffset+1 != parSetIndexOffsets.size() ) ? parSetIndexOffsets[iOffset+1]: banffPostfit->GetNrows();
        std::cout << "nPars = " << nPars << std::endl;
        nPars -= parSetIndexOffsets[iOffset];
        std::cout << "nPars = " << nPars << std::endl;
        banffHistBuff = new TH1D(
          "banffHist_TH1D", Form("%s postfit parameters", parSetName.c_str())
          ,nPars, 0, nPars
        );
        banffHistBuff->SetLineColor(kRed);
        gundamHistBuff = new TH1D(
          "gundamHistBuff", Form("%s postfit parameters", parSetName.c_str())
          ,nPars, 0, nPars
        );
        histOffset = 0;
      }

      histOffset++;
      banffHistBuff->SetBinContent(histOffset, postfitBanffValues[iBanffPar]);
      banffHistBuff->SetBinError(histOffset, postfitBanffErrors[iBanffPar]);
      gundamHistBuff->SetBinContent(histOffset, postfitGundamValues[iBanffPar]);
      gundamHistBuff->SetBinError(histOffset, postfitGundamErrors[iBanffPar]);

      banffHistBuff->GetXaxis()->SetBinLabel(histOffset, banffNamesList[iBanffPar].c_str());

    }
    writeFct();
  }

  outFile->Close();

  exit(0);
}


void init(){
  banffFile = TFile::Open(banffOutFilePath.c_str());
  gundamFile = TFile::Open(gundamOutFilePath.c_str());

  banffNames = banffFile->Get<TObjArray>("param_list");
}
std::pair<int, int> fetchGundamParIndex(int banffIndex_){
  std::pair<int, int> outIndex{-1, -1};
  int iBanffPar{-1};
  for( int iParSet = 0 ; iParSet < banffParSetList.size() ; iParSet++ ){
    std::string path = Form("%s/%s/values/preFitErrors_TH1D", gundamBaseFolder.c_str(), banffParSetList[iParSet].c_str());
    outIndex.first = iParSet;
    auto* gundamPrefit = gundamFile->Get<TH1D>(path.c_str());
    for( int iPar = 0 ; iPar < gundamPrefit->GetNbinsX() ; iPar++ ){
      iBanffPar++;

      int gundamParIndex = iPar+1;
      std::string parName = banffNames->At(iBanffPar)->GetName();
      if( banffParSetList[iParSet] == "Cross-Section Systematics" ){
        gundamParIndex = fetchHistParIndex(gundamPrefit, parName);
      }

      if( iBanffPar == banffIndex_ ){
        outIndex.second = gundamParIndex;
        return outIndex;
      }

      if(gundamParIndex == gundamPrefit->GetNbinsX()){
        break;
      }
    }
  }
  LogThrow("Invalid banff index: " << banffIndex_);
  return outIndex;
}
int fetchHistParIndex(TH1D* hist_, const std::string& name_ ){
  auto* xAxis = hist_->GetXaxis();
  for( int iBin = 1 ; iBin < xAxis->GetNbins()+1 ; iBin++ ){
    if( GenericToolbox::doesStringEndsWithSubstring(xAxis->GetBinLabel(iBin), name_) ){ return iBin; }
  }
  throw std::runtime_error("Could not find: "+name_);
}
