// Generate binning file for the fitter

std::string __covMatFilePath__ = "obsnorm_574binning_BANFF_Neutrino2020.root";
std::string __covMatName__ = "obsNorm_cov";
std::string __nomValName__ = "obsNorm_weights";

double minEigenRatio = 1E-12;

const std::vector<std::pair<int, std::string>> sampleNames{
    {3, "FGD1_numuCC_0pi"},
    {4, "FGD1_numuCC_1pi"},
    {5, "FGD1_numuCC_other"},
    {19,"FGD2_numuCC_0pi"},
    {20,"FGD2_numuCC_1pi"},
    {21,"FGD2_numuCC_other"},
    {43,"FGD1_anti-numuCC_0pi"},
    {44,"FGD1_anti-numuCC_1pi"},
    {45,"FGD1_anti-numuCC_other"},
    {49,"FGD2_anti-numuCC_0pi"},
    {50,"FGD2_anti-numuCC_1pi"},
    {51,"FGD2_anti-numuCC_other"},
    {55,"FGD1_NuMuBkg_CC0pi_in_AntiNu_Mode"},
    {56,"FGD1_NuMuBkg_CC1pi_in_AntiNu_Mode"},
    {57,"FGD1_NuMuBkg_CCother_in_AntiNu_Mode"},
    {61,"FGD2_NuMuBkg_CC0pi_in_AntiNu_Mode"},
    {62,"FGD2_NuMuBkg_CC1pi_in_AntiNu_Mode"},
    {63,"FGD2_NuMuBkg_CCother_in_AntiNu_Mode"}
};

void generateBinningFileBanff(){

  auto* f = TFile::Open(__covMatFilePath__.c_str());
  auto* covMatrix = (TMatrixDSym*) f->Get(__covMatName__.c_str());
  auto* nomWeights = (TVectorD*) f->Get(__nomValName__.c_str());

  std::stringstream ssBinFile;
  ssBinFile << "variables: SelectedSample CosThetamu CosThetamu Pmu Pmu" << std::endl;
  std::string invalidBinStr = "-1 -1 -1 -1 -1";

  int nCovBin{0};
  TAxis* axisCovP;
  TAxis* axisCovTh;
  std::string nameBuf;

  for( int iSample = 0 ; iSample < sampleNames.size() ; iSample++ ){

    nameBuf = sampleNames[iSample].second + "_p_axis";
    axisCovP = f->Get<TAxis>(nameBuf.c_str());
    if( axisCovP == nullptr ) throw std::runtime_error("\"" + nameBuf + "\" not found");
    std::cout << nameBuf << " -> " << axisCovP->GetNbins() << " bins / ";

    nameBuf = sampleNames[iSample].second + "_th_axis";
    axisCovTh = f->Get<TAxis>(nameBuf.c_str());
    if( axisCovTh == nullptr ) throw std::runtime_error("\"" + nameBuf + "\" not found");
    std::cout << nameBuf << " -> " << axisCovTh->GetNbins() << " bins." << std::endl;

    for( int iThetaBin = 0 ; iThetaBin < axisCovTh->GetNbins() ; iThetaBin++ ){
      for( int iPmuBin = 0 ; iPmuBin < axisCovP->GetNbins() ; iPmuBin++ ){
        ssBinFile
        << sampleNames[iSample].first << " "
        << axisCovTh->GetBinLowEdge(iThetaBin+1) << " "
        << axisCovTh->GetBinUpEdge(iThetaBin+1) << " "
        << axisCovP->GetBinLowEdge(iPmuBin+1) << " "
        << axisCovP->GetBinUpEdge(iPmuBin+1) << " "
        << std::endl;
        nCovBin++;
      }
    }
  }

  std::cout << ssBinFile.str() << std::endl;
  std::string outFilePath = __covMatFilePath__ + ".txt";
  std::cout << "SAVING AS: " << outFilePath << std::endl;
  GenericToolbox::dumpStringInFile(outFilePath, ssBinFile.str());

  std::cout << "Defined: nCovBin = " << nCovBin << std::endl;
  std::cout << "Matrix is: " << GET_VAR_NAME_VALUE(covMatrix->GetNcols()) << std::endl;
  if( nCovBin != covMatrix->GetNcols() ) throw std::runtime_error("Nb of bins dont match");

  auto* fOut = TFile::Open((__covMatFilePath__ + ".decomp.root").c_str(), "RECREATE");

  std::cout << "Eigen decomposition..." << std::endl;
  auto* eigenDecomp = new TMatrixDSymEigen(*covMatrix);
  std::cout << "Eigen decomposition done." << std::endl;


  std::cout << "Extracting eigen values and vectors..." << std::endl;
  auto* eigenValues = (TVectorD*) eigenDecomp->GetEigenValues().Clone() ;
  auto* eigenVectors = (TMatrixD*) eigenDecomp->GetEigenVectors().Clone() ;
  auto* eigenVectorsInv = (new TMatrixD(TMatrixD::kTransposed, *eigenVectors) );

  std::cout << "Computing inverse matrix using eigen decomposition..." << std::endl;
  auto* covMatrixReg = (TMatrixD*) covMatrix->Clone();
  auto* covMatrixInv = (TMatrixD*) covMatrix->Clone();
  covMatrixReg->Zero();
  covMatrixInv->Zero();

  // SANITY CHECK
  double maxEigenValue = (*eigenValues)[0];
  auto* eigenValuesInv = new TVectorD(eigenValues->GetNrows());
  for( int iEigen = 0 ; iEigen < eigenValues->GetNrows() ; iEigen++ ){
    GenericToolbox::mkdirTFile(fOut, "Projectors")->cd();

//    auto* eigenVectorCol = GenericToolbox::getMatrixColumn(eigenValues,iEigen);
//    auto* eigenVectorLine = GenericToolbox::getMatrixLine(eigenValuesInv,iEigen);
//    auto* outerProd = GenericToolbox::getOuterProduct(eigenVectorCol, eigenVectorLine);

    (*eigenValuesInv)[iEigen] = 1./(*eigenValues)[iEigen];

    if( (*eigenValues)[iEigen] < maxEigenValue * minEigenRatio ){
      std::cout << "Skipping " << GET_VAR_NAME_VALUE(iEigen) << " -> " << (*eigenValues)[iEigen] << " < " << maxEigenValue << "*" << minEigenRatio << std::endl;
      (*eigenValues)[iEigen] = 0;
      (*eigenValuesInv)[iEigen] = 0;
//      for( int iCol = 0 ; iCol < eigenVectors->GetNcols() ; iCol++ ){
//        (*eigenVectorsInv)[iEigen][iCol] = 0;
//        (*eigenVectors)[iCol][iEigen] = 0;
//      }
    }
  }

  GenericToolbox::mkdirTFile(fOut, "Projectors")->cd();

  auto* diagonalMatrix = GenericToolbox::makeDiagonalMatrix(eigenValues);
  auto* diagonalMatrixInv = GenericToolbox::makeDiagonalMatrix(eigenValuesInv);
//  auto* diagonalMatrixInv = new TMatrixD(*diagonalMatrix);
//  std::function<void(TMatrixD*, int, int)> invFct = [](TMatrixD* m, int iRow, int iCol){ if((*m)[iRow][iCol] <= 0) (*m)[iRow][iCol] = 1./(*m)[iRow][iCol]; else (*m)[iRow][iCol] = 0; };
//  GenericToolbox::transformMatrix(diagonalMatrixInv, invFct);

  (*covMatrixReg) =  (*eigenVectors);
  (*covMatrixReg) *= (*diagonalMatrix);
  (*covMatrixReg) *= (*eigenVectorsInv);

  (*covMatrixInv) =  (*eigenVectors);
  (*covMatrixInv) *= (*diagonalMatrixInv);
  (*covMatrixInv) *= (*eigenVectorsInv);

//  (*covMatrixInv) =  (*eigenVectorsInv);
//  (*covMatrixInv) *= (*diagonalMatrixInv);
//  (*covMatrixInv) *= (*eigenVectors);

  std::cout << "Writing inverse matrix..." << std::endl;
  fOut->cd();
  nomWeights->Write("Prior_Weights");
  std::cout << "Extraction done. Now writing..." << std::endl;
  eigenValues->Write("Covariance_Matrix_EigenValues");
  eigenVectors->Write("Covariance_Matrix_EigenVectors");
  eigenVectorsInv->Write("Covariance_Matrix_EigenVectorsInv");
  std::cout << "Eigen data writen." << std::endl;
  covMatrix->Write("Covariance_Matrix");
  covMatrixInv->Write("Covariance_Matrix_Inv");
  covMatrixReg->Write("Covariance_Matrix_Reg");
//  fOut->WriteObject(covMatrixInv, "Covariance_Matrix_Inv");

  std::cout << "Closing file: " << fOut->GetName() << std::endl;
  fOut->Close();
  exit(0);
}
