#include "GenericToolbox.Root.h"
#include "GenericToolbox.Root.LeafHolder.h"
#include "Logger.h"

bool overrideOutput{false}; // ignore if files have already been processed.

//std::string toyFolder{"/Users/ablanche/Documents/Work/Output/results/gundam/common/OA2020/NDMC_psycheInfoAdded_DetThrows"};
std::string toyFolder{"/sps/t2k/common/inputs/OA2020/NDMC_psycheInfoAdded_DetThrows"};
//std::string asimovFolder{"/Users/ablanche/Documents/Work/Output/results/gundam/common/OA2020/NDMC_psycheInfoAdded/ExtraBranchesForGundam"};
std::string asimovFolder{"/sps/t2k/common/inputs/OA2020/NDMC_psycheInfoAdded/ExtraBranchesForGundam"};

std::map<std::string, std::string> filePairs{
    {"run4a_throw_splines.root", "run4aMCsplines.root"}
    ,{"run5_throw_splines.root", "run5MCsplines.root"}
    ,{"run2a_throw_splines.root", "run2aMCsplines.root"}
    ,{"run2w_throw_splines.root", "run2wMCsplines.root"}
    ,{"run3_throw_splines.root", "run3MCsplines.root"}
    ,{"run4w_throw_splines.root", "run4wMCsplines.root"}
    ,{"run6_throw_splines.root", "run6MCsplines.root"}
    ,{"run7_throw_splines.root", "run7MCsplines.root"}
    ,{"run8w_throw_splines.root", "run8wMCsplines.root"}
    ,{"run9_throw_splines.root", "run9MCsplines.root"}
};

std::vector<std::string> branchesToAdd{
    {"isRHC"},
    {"passedPsyche"},
    {"foundTrueVert"},
    {"PileUpWeight"},
    {"POTWeight"}
};

//std::string outputFolder = "./out";
std::string outputFolder = "/sps/t2k/common/inputs/OA2020/NDMC_psycheInfoAdded_DetThrows/WithExtraBranchesForGundam";

void processToySampleSum(){

  for( auto& filePair : filePairs ){
    LogInfo << "Processing " << filePair.first << " | " << filePair.second << std::endl;

    std::string filePath;
    filePath = toyFolder + "/" + filePair.first;
    LogThrowIf(not GenericToolbox::doesTFileIsValid(filePath), "Could not open " << filePath);
    auto* toyFile = TFile::Open(filePath.c_str(), "READ");
    auto* toyTree = toyFile->Get<TTree>("sample_sum");

    std::string outputPath = outputFolder + "/" + GenericToolbox::getFileNameFromFilePath(filePath);
    if( not overrideOutput and GenericToolbox::doesTFileIsValid(outputPath) ){
      LogWarning << outputPath << " -> already processed." << std::endl;
      continue;
    }

    filePath = asimovFolder + "/" + filePair.second;
    LogThrowIf(not GenericToolbox::doesTFileIsValid(filePath), "Could not open " << filePath);
    auto* asimovFile = TFile::Open(filePath.c_str(), "READ");
    auto* asimovTree = asimovFile->Get<TTree>("sample_sum");

    asimovTree->SetBranchStatus("*", false);
    for( auto& branchName : branchesToAdd ){ asimovTree->SetBranchStatus(branchName.c_str(), true); }

    LogDebug << GET_VAR_NAME_VALUE(toyTree->GetEntries()) << std::endl;
    LogDebug << GET_VAR_NAME_VALUE(asimovTree->GetEntries()) << std::endl;

    GenericToolbox::mkdirPath(outputFolder);
    LogInfo << "Creating: " << outputPath << std::endl;
    auto* outFile = TFile::Open( outputPath.c_str(), "RECREATE" );
    LogInfo << "Clonning input tree..." << std::endl;
    toyTree->SetBranchStatus("*", true);
    auto* outTree = toyTree->CloneTree();
    outTree->SetBranchStatus("*", false);
    LogInfo << "Input tree cloned." << std::endl;

    std::vector<GenericToolbox::LeafHolder> leafHolderList(branchesToAdd.size());
    std::vector<TBranch*> newBranchList(branchesToAdd.size());
    for( size_t iLeaf = 0 ; iLeaf < branchesToAdd.size() ; iLeaf++ ){
      leafHolderList[iLeaf].hook(asimovTree, branchesToAdd[iLeaf]);
      char c = GenericToolbox::findOriginalVariableType(GenericToolbox::leafToAnyType(leafHolderList[iLeaf].getLeafTypeName()));
      newBranchList[iLeaf] = outTree->Branch(branchesToAdd[iLeaf].c_str(), (void*) &leafHolderList[iLeaf].getByteBuffer()[0], (branchesToAdd[iLeaf] + "/" + c).c_str());
    }

    Long64_t nEntries{asimovTree->GetEntries()};
    for(Long64_t iEntry = 0 ; iEntry < nEntries ; iEntry++){
      GenericToolbox::displayProgressBar(iEntry, nEntries, "Reading trees...");
      asimovTree->GetEntry(iEntry);
      outTree->GetEntry(iEntry);

      for(const auto& branch : newBranchList){ branch->Fill(); }
    }

    outTree->SetBranchStatus("*", true);
    outTree->Write();
    outFile->Close();

    delete outFile;
  }

//  LogInfo << "Files to process: " << GenericToolbox::parseVectorAsString(filesList) << std::endl;
//
//  for( auto& filePath : filesList ){
//    filePath = GenericToolbox::trimString(filePath, " ");
//    if( filePath.empty() ){ continue; }
//    if( GenericToolbox::doesStringStartsWithSubstring(filePath, "#") ){ continue; }
//    if( not GenericToolbox::doesTFileIsValid(filePath) ){ continue; }
//
//    LogInfo << "Opening: " << filePath << std::endl;
//    auto* inFile = TFile::Open(filePath.c_str(), "READ");
//    auto* tree = inFile->Get<TTree>("sample_sum");
//    LogThrowIf(tree==nullptr, "Can't find sample_sum tree.");
//
//    std::string outputPath = outputFolder + "/" + GenericToolbox::getFileNameFromFilePath(filePath);
//    if( not overrideOutput and GenericToolbox::doesTFileIsValid(outputPath) ){
//      LogWarning << outputPath << " -> already processed." << std::endl;
//      continue;
//    }
//
//    GenericToolbox::mkdirPath(outputFolder);
//    LogInfo << "Creating: " << outputPath << std::endl;
//    auto* outFile = TFile::Open( outputPath.c_str(), "RECREATE" );
//    LogInfo << "Clonning input tree..." << std::endl;
//    tree->SetBranchStatus("*", true);
//    auto* outTree = tree->CloneTree();
//    LogInfo << "Input tree cloned." << std::endl;
//
//    double Pmu;
//    int TrueLepPDG;
//    int NeutrinoCode;
//    int TgtMat;
//    int ReactionCode;
//
//    outTree->SetBranchStatus("*", false);
//    outTree->SetBranchStatus("Pmu", true);
//    outTree->SetBranchStatus("TrueLepPDG", true);
//    outTree->SetBranchStatus("NeutrinoCode", true);
//    outTree->SetBranchStatus("TgtMat", true);
//    outTree->SetBranchStatus("ReactionCode", true);
//
//    outTree->SetBranchAddress("Pmu", &Pmu);
//    outTree->SetBranchAddress("TrueLepPDG", &TrueLepPDG);
//    outTree->SetBranchAddress("NeutrinoCode", &NeutrinoCode);
//    outTree->SetBranchAddress("TgtMat", &TgtMat);
//    outTree->SetBranchAddress("ReactionCode", &ReactionCode);
//
//    bool isConsecutiveIdenticalEvent;
//    double PmuCoulombCorrection;
//    std::vector<TBranch*> bList;
//
//    bList.emplace_back(outTree->Branch("PmuCoulombCorrection", &PmuCoulombCorrection));
//    bList.emplace_back(outTree->Branch("isConsecutiveIdenticalEvent", &isConsecutiveIdenticalEvent));
//
//
//    double lastPlep{0};
//    Long64_t nEntries = outTree->GetEntries();
//    for(Long64_t iEntry = 0 ; iEntry < nEntries ; iEntry++){
//      GenericToolbox::displayProgressBar(iEntry, nEntries, "Adding branches...");
//      outTree->GetEntry(iEntry);
//
//      // from MaCh3: https://github.com/t2k-software/MaCh3/blob/db103738d759ee08707dbe33cf2ebb588a43a1c7/samplePDF/samplePDFND.cpp#L4661
//      isConsecutiveIdenticalEvent = false;
//      if(fabs(lastPlep-Pmu)<1E-6) { isConsecutiveIdenticalEvent = true; }
//      lastPlep = Pmu;
//
//      PmuCoulombCorrection = Pmu;
//      PmuCoulombCorrection += ApplyCoulombShift(TrueLepPDG, NeutrinoCode, TgtMat, TMath::Abs(ReactionCode));
//
//      for(const auto& branch : bList){ branch->Fill(); }
//    }
//
//    outTree->SetBranchStatus("*", true);
//    outTree->Write();
//    outFile->Close();
//
//    delete outFile;
//  }

}
