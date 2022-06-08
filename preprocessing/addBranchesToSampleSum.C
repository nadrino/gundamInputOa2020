#include "GenericToolbox.Root.h"
#include "Logger.h"

// std::string outputFolder = "./out";
std::string outputFolder = "/sps/t2k/common/inputs/OA2020/NDMC_psycheInfoAdded/WithExtraBranchesForGundam";

// from MaCh3
double ApplyCoulombShift(int TruePDG, int NuPDG, int target, int reaction);

void addBranchesToSampleSum(){

  auto filesList = GenericToolbox::dumpFileAsVectorString("./inputFiles.txt");

  LogInfo << "Files to process: " << GenericToolbox::parseVectorAsString(filesList) << std::endl;

  for( auto& filePath : filesList ){
    filePath = GenericToolbox::trimString(filePath, " ");
    if( filePath.empty() ){ continue; }
    if( GenericToolbox::doesStringStartsWithSubstring(filePath, "#") ){ continue; }
    if( not GenericToolbox::doesTFileIsValid(filePath) ){ continue; }

    LogInfo << "Opening: " << filePath << std::endl;
    auto* inFile = TFile::Open(filePath.c_str(), "READ");
    auto* tree = inFile->Get<TTree>("sample_sum");
    LogThrowIf(tree==nullptr, "Can't find sample_sum tree.");

    GenericToolbox::mkdirPath(outputFolder);
    std::string outputPath = outputFolder + "/" + GenericToolbox::getFileNameFromFilePath(filePath);
    LogInfo << "Creating: " << outputPath << std::endl;

    auto* outFile = TFile::Open( outputPath.c_str(), "RECREATE" );

    LogInfo << "Clonning input tree..." << std::endl;
    tree->SetBranchStatus("*", true);
    auto* outTree = tree->CloneTree();
    LogInfo << "Input tree cloned." << std::endl;

    double Pmu;
    int TrueLepPDG;
    int NeutrinoCode;
    int TgtMat;
    int ReactionCode;

    outTree->SetBranchStatus("*", false);
    outTree->SetBranchStatus("Pmu", true);
    outTree->SetBranchStatus("TrueLepPDG", true);
    outTree->SetBranchStatus("NeutrinoCode", true);
    outTree->SetBranchStatus("TgtMat", true);
    outTree->SetBranchStatus("ReactionCode", true);

    outTree->SetBranchAddress("Pmu", &Pmu);
    outTree->SetBranchAddress("TrueLepPDG", &TrueLepPDG);
    outTree->SetBranchAddress("NeutrinoCode", &NeutrinoCode);
    outTree->SetBranchAddress("TgtMat", &TgtMat);
    outTree->SetBranchAddress("ReactionCode", &ReactionCode);

    bool isConsecutiveIdenticalEvent;
    double PmuCoulombCorrection;
    std::vector<TBranch*> bList;

    bList.emplace_back(outTree->Branch("PmuCoulombCorrection", &PmuCoulombCorrection));
    bList.emplace_back(outTree->Branch("isConsecutiveIdenticalEvent", &isConsecutiveIdenticalEvent));


    double lastPlep{0};
    Long64_t nEntries = outTree->GetEntries();
    for(Long64_t iEntry = 0 ; iEntry < nEntries ; iEntry++){
      GenericToolbox::displayProgressBar(iEntry, nEntries, "Adding branches...");
      outTree->GetEntry(iEntry);

      // from MaCh3: https://github.com/t2k-software/MaCh3/blob/db103738d759ee08707dbe33cf2ebb588a43a1c7/samplePDF/samplePDFND.cpp#L4661
      isConsecutiveIdenticalEvent = false;
      if(fabs(lastPlep-Pmu)<1E-6) { isConsecutiveIdenticalEvent = true; }
      lastPlep = Pmu;

      PmuCoulombCorrection = Pmu;
      PmuCoulombCorrection += ApplyCoulombShift(TrueLepPDG, NeutrinoCode, TgtMat, TMath::Abs(ReactionCode));

      for(const auto& branch : bList){ branch->Fill(); }
    }

    outTree->SetBranchStatus("*", true);
    outTree->Write();
    outFile->Close();

    delete outFile;
  }

}

double ApplyCoulombShift(int TruePDG, int NuPDG, int target, int reaction) {
  // ******************************************
  // Adapted from MaCh3

  int kNEUT_NC1pi01n = 31;
  int kTarget_C = 12;
  int kTarget_O = 16;
  // Only want CC events
  if (reaction >= kNEUT_NC1pi01n) return 0.0;

  // The amount we shift by
  double shift = 0.0;

  // Check the lepton candidate matches the neutrino PDG
  // If anti-neutrino interaction, check that NuPDG
  if (NuPDG < 0) {
    if (NuPDG != TruePDG-1) return shift;
  } else {
    if (NuPDG != TruePDG+1) return shift;
  }
  // Check PID of true particle making this lepton candidate
  // This is now done above as before. The following line is only left for historical comparison
  //if (abs(TruePDG) != 11 && abs(TruePDG) != 13 && abs(TruePDG) != 15) return 0.0;

  // Shift the lepton momentum
//   if (kKinType == kLeptonMomentum) {
  if (true) {
    // Neutrino
    if (TruePDG > 0) {
      // If target was C
      if (target == kTarget_C) {
        shift = -3.6;
        // If target was O
      } else if (target == kTarget_O) {
        shift = -4.3;
      }
      // Anti-neutrino
    } else if (TruePDG < 0) {
      // If target was C
      if (target == kTarget_C) {
        shift = 2.6;
      } else if (target == kTarget_O) {
        shift = 3.3;
      }
    }
    // Return the shiften lepton momentum
    return shift;
    // Shift the Q2 if requested
  }

  return shift;
}
