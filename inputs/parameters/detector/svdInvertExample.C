// Generate binning file for the fitter

template<typename T> TMatrixT<T>* makeDiagonalMatrix(TVectorT<T>* v_){
  if( v_ == nullptr ) return nullptr;
  auto* out = new TMatrixT<T>(v_->GetNrows(), v_->GetNrows());
  for( int iDiag = 0 ; iDiag < out->GetNrows() ; iDiag++ ){
    (*out)[iDiag][iDiag] = (*v_)[iDiag];
  }
  return out;
}


void svdInvertExample(){

  double minEigenRatio = 1E-5;

  TMatrixDSym* covMatrix = new TMatrixDSym(3);
  (*covMatrix)[0][0] = 4;
  (*covMatrix)[1][1] = 4;
  (*covMatrix)[1][0] = 4;
  (*covMatrix)[0][1] = 4;
  (*covMatrix)[2][2] = 9;
  covMatrix->Print();

  std::cout << "Eigen decomposition..." << std::endl;
  auto* eigenDecomp = new TMatrixDSymEigen(*covMatrix);
  std::cout << "Eigen decomposition done." << std::endl;


  std::cout << "Extracting eigen values and vectors..." << std::endl;
  auto* eigenValues = (TVectorD*) eigenDecomp->GetEigenValues().Clone() ;

  std::cout << "Here there should be zero eigen values:" << std::endl;
  eigenValues->Print();

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

    (*eigenValuesInv)[iEigen] = 1./(*eigenValues)[iEigen];

    if( (*eigenValues)[iEigen] < maxEigenValue * minEigenRatio ){
      std::cout << "Skipping #" << (iEigen) << " -> " << (*eigenValues)[iEigen] << " < " << maxEigenValue << "*" << minEigenRatio << std::endl;
      (*eigenValues)[iEigen] = 0;
      (*eigenValuesInv)[iEigen] = 0;
    }
  }

  auto* diagonalMatrix = makeDiagonalMatrix(eigenValues);
  auto* diagonalMatrixInv = makeDiagonalMatrix(eigenValuesInv);

  (*covMatrixReg) =  (*eigenVectors);
  (*covMatrixReg) *= (*diagonalMatrix);
  (*covMatrixReg) *= (*eigenVectorsInv);

  std::cout << "covMatrixReg: " << std::endl;
  covMatrixReg->Print();

  (*covMatrixInv) =  (*eigenVectors);
  (*covMatrixInv) *= (*diagonalMatrixInv);
  (*covMatrixInv) *= (*eigenVectorsInv);

  std::cout << "covMatrixInv: " << std::endl;
  covMatrixInv->Print();

  std::cout << "Testing partial inversion: " << std::endl;
  auto* covMatrixRegInvReg = (TMatrixD*) covMatrixReg->Clone();
  (*covMatrixRegInvReg) *= (*covMatrixInv);
  (*covMatrixRegInvReg) *= (*covMatrixReg);
  covMatrixRegInvReg->Print();

  exit(0);
}
