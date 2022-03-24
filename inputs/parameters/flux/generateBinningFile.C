

const std::string fluxRootFilePath = "../../BANFF/inputs/flux_13av7p1_run1-10_ndto9_banff_Neutrino2020.root";

void generateBinningFile(){

  auto* f = TFile::Open(fluxRootFilePath.c_str());
  std::stringstream ss;
  ss << "variables: isRHC NeutrinoCode Enu Enu" << std::endl;

  TKey* key;
  TClass* classPtr;
  TAxis* axis;
  for( int iKey = 0 ; iKey < f->GetListOfKeys()->GetSize() ; iKey++ ){
    key = (TKey*) f->GetListOfKeys()->At(iKey);
    classPtr = gROOT->GetClass(key->GetClassName());
    if( not classPtr->InheritsFrom("TAxis") ) continue;
    axis = f->Get<TAxis>(f->GetListOfKeys()->At(iKey)->GetName());
    std::cout << f->GetListOfKeys()->At(iKey)->GetName() << std::endl;
    auto splitStr = GenericToolbox::splitString(f->GetListOfKeys()->At(iKey)->GetName(), "_");
    if( splitStr.size() != 4 ) continue;

    int isRHC;
    int NeutrinoCode;
    int enutrue_low;
    int enutrue_high;

    if( splitStr[0] == "sk" ){
      // sk parameters won't affect nd280 spectra
      isRHC = -2;
      NeutrinoCode = 0;
    }
    else{
      if( splitStr[1] == "numode" ) isRHC = 0;
      if( splitStr[1] == "anumode" ) isRHC = 1;

      if( splitStr[2] == "numu" ) NeutrinoCode = 14;
      if( splitStr[2] == "numub" ) NeutrinoCode = -14;
      if( splitStr[2] == "nue" ) NeutrinoCode = 12;
      if( splitStr[2] == "nueb" ) NeutrinoCode = -12;
    }

    for( int iBin = 1 ; iBin <= axis->GetNbins() ; iBin++ ){
      ss << isRHC << " " << NeutrinoCode << " ";
      ss << axis->GetBinLowEdge(iBin) << " " << axis->GetBinUpEdge(iBin) << std::endl;
    }
  }

  std::cout << ss.str() << std::endl;
  GenericToolbox::dumpStringInFile("fluxBinning.txt", ss.str());

}
