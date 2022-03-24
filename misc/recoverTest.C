std::string filePath = "banff.DetEigen.32.root";
int nbRemove = 4;


void recoverTest(){

  auto* f = TFile::Open(filePath.c_str());
  auto* dir = f->GetDirectory("FitterEngine/postFit/matrices");
  dir->cd();
  dir->GetListOfKeys()->Print();

  auto* origV = dir->Get<TVectorD>("totalCovEigenValues_TVectorT_double");
  auto* regV = (TVectorD*) origV->Clone();
  regV->Zero();

  double maxEig = (*origV)[0];
  for(int iE = nbRemove ; iE < origV->GetNrows(); iE++){
    if( (*origV)[iE] > 0){
      (*regV)[iE] = (*origV)[iE] + maxEig;
    }
  }
  regV->Print();
  auto& diagonalMatrix = *GenericToolbox::makeDiagonalMatrix(regV);

  auto* p = dir->Get<TMatrixD>("totalCovEigenVectors_TMatrixT_double");
  auto* pt = dir->Get<TMatrixD>("totalCovEigenVectors_TMatrixT_double");
  pt->T();


  auto* regCov = (TMatrixD*) dir->Get("totalCovMatrix_TMatrixTSym_double");
  regCov->Zero();
  (*regCov) += (*p);
  (*regCov) *= diagonalMatrix;
  (*regCov) *= (*pt);

  regCov->Draw("COLZ");

}