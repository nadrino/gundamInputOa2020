
std::string ebMatrixPath = "../../BANFF/inputs/xsec_bin_parameter_file_BANFF_Neutrino2020.root";

void generateEbBinning(){

  // Param42_MC_FGD2_NuMuBkg_CC0pi_in_AntiNu_Mode__nu-14_on_targ_16_reac_-1_Enu_0_MeV_4000_MeV___pmu_900_MeV_1250_MeV___cosTheta_0.92000000000000004_0.93999999999999995_spline

  // "SelectedSample NuType Target ReactionCode enu pmu costheta"
  // 4 syst : 39...42

  auto* f = TFile::Open(ebMatrixPath.c_str(), "READ");
  auto* outFile = TFile::Open("Eb_dials.root", "RECREATE");

  auto* xsec_bin_param_name = f->Get<TObjArray>("xsec_bin_param_name");
  auto* xsec_bin_param_id   = f->Get<TVectorT<double>>("xsec_bin_param_id");

  auto* xsec_bin_param_splines_index        = f->Get<TVectorT<double>>("xsec_bin_param_splines_index");
  auto* xsec_bin_param_sample_index         = f->Get<TVectorT<double>>("xsec_bin_param_sample_index");
  auto* xsec_bin_param_sample_reaction_mode = f->Get<TVectorT<double>>("xsec_bin_param_sample_reaction_mode");
  auto* xsec_bin_param_sample_nuPDG         = f->Get<TVectorT<double>>("xsec_bin_param_sample_nuPDG");
  auto* xsec_bin_param_sample_target        = f->Get<TVectorT<double>>("xsec_bin_param_sample_target");
  auto* xsec_bin_param_enu_lb               = f->Get<TVectorT<double>>("xsec_bin_param_enu_lb");
  auto* xsec_bin_param_enu_ub               = f->Get<TVectorT<double>>("xsec_bin_param_enu_ub");
  auto* xsec_bin_param_p_lb                 = f->Get<TVectorT<double>>("xsec_bin_param_p_lb");
  auto* xsec_bin_param_p_ub                 = f->Get<TVectorT<double>>("xsec_bin_param_p_ub");
  auto* xsec_bin_param_cos_theta_lb         = f->Get<TVectorT<double>>("xsec_bin_param_cos_theta_lb");
  auto* xsec_bin_param_cos_theta_ub         = f->Get<TVectorT<double>>("xsec_bin_param_cos_theta_ub");

  auto* xsec_bin_param_splines              = f->Get<TObjArray>("xsec_bin_param_splines");
  auto* xsec_bin_param_graphs               = f->Get<TObjArray>("xsec_bin_param_graphs");

  for( int iPar = 0 ; iPar < xsec_bin_param_id->GetNrows() ; iPar++ ){

    std::stringstream ss;
    ss << "variables: SelectedSample NeutrinoCode TgtMat ReactionCode Enu Enu Pmu Pmu CosThetamu CosThetamu" << std::endl;

    TObjArray* splinesArray = new TObjArray();
    TObjArray* graphsArray   = new TObjArray();

    for( int iBin = 0 ; iBin < xsec_bin_param_splines_index->GetNrows() ; iBin++ ){

      if( (*xsec_bin_param_id)[iPar] != (*xsec_bin_param_splines_index)[iBin] ){ continue; }

      splinesArray->Add(xsec_bin_param_splines->At(iBin));
      graphsArray->Add(xsec_bin_param_graphs->At(iBin));

      ss        << int((*xsec_bin_param_sample_index)[iBin]);
      ss << " " << int((*xsec_bin_param_sample_nuPDG)[iBin]);
      ss << " " << int((*xsec_bin_param_sample_target)[iBin]);
      ss << " " << int((*xsec_bin_param_sample_reaction_mode)[iBin]);
      ss << " " << (*xsec_bin_param_enu_lb)[iBin]/1000.;
      ss << " " << (*xsec_bin_param_enu_ub)[iBin]/1000.;
      ss << " " << (*xsec_bin_param_p_lb)[iBin];
      ss << " " << (*xsec_bin_param_p_ub)[iBin];
      ss << " " << (*xsec_bin_param_cos_theta_lb)[iBin];
      ss << " " << (*xsec_bin_param_cos_theta_ub)[iBin];
      ss << std::endl;

    }

    std::string fileName = Form("binning_%s.txt", xsec_bin_param_name->At(iPar)->GetName());
    std::cout << "Writing binning: " << fileName << std::endl;
    GenericToolbox::dumpStringInFile(fileName, ss.str());

    outFile->WriteObject(splinesArray, Form("%s_splines", xsec_bin_param_name->At(iPar)->GetName()));
    outFile->WriteObject(graphsArray, Form("%s_graphs", xsec_bin_param_name->At(iPar)->GetName()));

  }

  std::cout << "Dials writen in " << outFile->GetName() << std::endl;
  outFile->Close();


  exit(0);
}