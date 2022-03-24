// Generate binning file for the fitter

std::string __covMatFilePath__ = "ND280_Syst_Cov_20200114_TH2D574Binning_all.root";
std::string __covMatName__ = "Total_Covariance_Matrix";
std::string __selectionHist__ = "Selection";

void generateBinningFile(){

  auto* f = TFile::Open(__covMatFilePath__.c_str());
  auto* m = f->Get<TH2D>(__covMatName__.c_str());
  auto* selectionNames = f->Get<TH1D>(__selectionHist__.c_str())->GetXaxis();
  std::map<std::string, int> selectionDict;

  for( int iBin = 0 ; iBin < selectionNames->GetNbins() ; iBin++ ){
    selectionDict[selectionNames->GetBinLabel(iBin+1)] = iBin;
  }

  cout << GenericToolbox::parseMapAsString(selectionDict) << endl;

  std::stringstream ssBinFile;
  ssBinFile << "variables: SelectedSample isRHC CosThetamu CosThetamu Pmu Pmu" << std::endl;

  std::string invalidBinStr = "-1 -1 -1 -1 -1 -1";

  int fgd_reco;
  int cut_branch;
  int beammode;
  int analysis;

  int iBinKin = -1;
  std::string kinBinName;
  TAxis* axisCovP;
  TAxis* axisCovTh;
  std::stringstream topologyStr;
  for( int iBinMat = 1 ; iBinMat < m->GetXaxis()->GetNbins()+1 ; iBinMat++ ){

    if( not std::string(m->GetXaxis()->GetBinLabel(iBinMat)).empty() ){
      iBinKin = -1;
      kinBinName = m->GetXaxis()->GetBinLabel(iBinMat);
      if( kinBinName == "Unassigned" ){
        ssBinFile << invalidBinStr << std::endl;
        break;
      }
      std::cout << "NEW BINNING: " << kinBinName << std::endl;

      fgd_reco = 0;
      cut_branch = 0;
      beammode = 1;
      analysis = 1;
      auto splitStr = GenericToolbox::splitString(kinBinName, " ");
      for( const auto& str : splitStr ){
        if( str == "FGD2" ) fgd_reco = 1;
        if( str == "NuMuBkg" ){ analysis = -1; beammode = -1; }
        if( str == "1pi" or str == "CC1pi" ) cut_branch = 1;
        if( str == "other" or str == "CCother" ) cut_branch = 2;
        if( str == "anti-numuCC" ) beammode = -1;
      }
      topologyStr.str("");
      topologyStr << selectionDict[kinBinName] << " " << ( beammode < 0 );

      GenericToolbox::replaceSubstringInsideInputString(kinBinName, " ", "_");
      axisCovP = f->Get<TAxis>(("AxisForCov/" + kinBinName + "_p_axis").c_str());
      axisCovTh = f->Get<TAxis>(("AxisForCov/" + kinBinName + "_th_axis").c_str());
    }

    iBinKin++;
    if( kinBinName.empty() ){ std::cout << "NO BIN NAME" << std::endl; exit(0); }

    int binP = iBinKin % axisCovP->GetNbins() + 1;
    int binTh = iBinKin / axisCovP->GetNbins() + 1;

    ssBinFile << topologyStr.str() << " ";
    ssBinFile << axisCovTh->GetBinLowEdge(binTh) << " ";
    ssBinFile << axisCovTh->GetBinUpEdge(binTh) << " ";
    ssBinFile << axisCovP->GetBinLowEdge(binP) << " ";
    ssBinFile << axisCovP->GetBinUpEdge(binP) << std::endl;

  }

  std::cout << ssBinFile.str() << std::endl;
  std::string outFilePath = __covMatFilePath__ + ".txt";
  std::cout << "SAVED AS: " << outFilePath << std::endl;
  GenericToolbox::dumpStringInFile(outFilePath, ssBinFile.str());

  TMatrixDSym* cov = f->Get<TMatrixTSym<double>>("Covariance_Matrix");

  auto* fOut = TFile::Open((__covMatFilePath__ + ".decomp.root").c_str(), "RECREATE");

  std::cout << "Eigen decomposition..." << cov << std::endl;
  auto* eigenDecomp = new TMatrixDSymEigen(*cov);
  std::cout << "Eigen decomposition done." << std::endl;

  auto* eigenValues = (TVectorD*) eigenDecomp->GetEigenValues().Clone() ;
  auto* eigenVectors = (TMatrixD*) eigenDecomp->GetEigenVectors().Clone() ;
  auto* eigenVectorsInv = (new TMatrixD(TMatrixD::kTransposed, *eigenVectors) );

  fOut->WriteObject(eigenValues, "Covariance_Matrix_EigenValues");
  fOut->WriteObject(eigenVectors, "Covariance_Matrix_EigenVectors");
  fOut->WriteObject(eigenVectorsInv, "Covariance_Matrix_EigenVectorsInv");

  std::cout << "Getting inverse of the cov matrix..." << std::endl;
  auto* inverseCovarianceMatrix = (TMatrixDSym*)(cov->Clone());
  inverseCovarianceMatrix->Invert();

  fOut->WriteObject(cov, "Covariance_Matrix");
  fOut->WriteObject(inverseCovarianceMatrix, "Covariance_Matrix_Inv");

  fOut->Close();
  exit(0);
}
