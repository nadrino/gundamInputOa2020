
std::string matrixPath = "../../BANFF/inputs/xsec_parameters_BANFF_Neutrino2020.root";
//std::string matrixPath = "../../BANFF/inputs/xsec_bin_parameter_file_BANFF_Neutrino2020.root";



void printMatrixTable(){

  auto* f = TFile::Open(matrixPath.c_str());

  // Table printout
  std::vector<std::vector<std::string>> tableLines;
  tableLines.emplace_back(std::vector<std::string>{
    "#"
    , "Name"
    , "Prior"
    , "Nominal"
    , "Min"
    , "Max"
    , "Throw"
    , "Fit"
    , "Constrain"
  });

  auto* xsec_param_prior = f->Get<TVectorT<double>>("xsec_param_prior");
  auto* xsec_param_nom = f->Get<TVectorT<double>>("xsec_param_nom");
  auto* xsec_param_lb = f->Get<TVectorT<double>>("xsec_param_lb");
  auto* xsec_param_ub = f->Get<TVectorT<double>>("xsec_param_ub");
  auto* xsec_param_throw = f->Get<TVectorT<double>>("xsec_param_throw");
  auto* xsec_param_fit = f->Get<TVectorT<double>>("xsec_param_fit");
  auto* xsec_param_constrain = f->Get<TVectorT<double>>("xsec_param_constrain");
  auto* xsec_param_names = f->Get<TObjArray>("xsec_param_names");

  int nPars = xsec_param_prior->GetNrows();

  for( int iPar = 0 ; iPar < nPars ; iPar++ ){
    std::vector<std::string> lineValues(tableLines[0].size());
    int valIndex{0};
    lineValues[valIndex++] = std::to_string(iPar);

    lineValues[valIndex++] = xsec_param_names->At(iPar)->GetName();
    lineValues[valIndex++] = std::to_string((*xsec_param_prior)[iPar]);
    lineValues[valIndex++] = std::to_string((*xsec_param_nom)[iPar]);
    lineValues[valIndex++] = std::to_string((*xsec_param_lb)[iPar]);
    lineValues[valIndex++] = std::to_string((*xsec_param_ub)[iPar]);
    (*xsec_param_throw)[iPar] == 1 ? lineValues[valIndex++] = "Yes": lineValues[valIndex++] = "No";
    lineValues[valIndex++] = std::to_string((*xsec_param_fit)[iPar]);
    lineValues[valIndex++] = std::to_string((*xsec_param_constrain)[iPar]);

    tableLines.emplace_back(lineValues);
  }

  GenericToolbox::TablePrinter t;
  t.fillTable(tableLines);
  t.printTable();

}