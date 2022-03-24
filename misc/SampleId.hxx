#ifndef SampleId_hxx
#define SampleId_hxx

#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <set>

/// Event sample - whether it passes the NuMu-CCQE selection, or CC1Pi selection for example
namespace SampleId{

  
  enum SampleEnum {
    kUnassigned,
    // FGD1
    kFGD1NuMuCC,
    kFGD1NuMuCCQE,
    kFGD1NuMuCC0Pi,
    kFGD1NuMuCC0PiNoPhoton,
    kFGD1NuMuCC0Pi0Protons,
    kFGD1NuMuCC0PiNProtons,
    kFGD1NuMuCC0Pi0ProtonsNoPhoton,
    kFGD1NuMuCC0PiNProtonsNoPhoton,
    kFGD1NuMuCC1Pi,
    kFGD1NuMuCC1PiNoPhoton,
    kFGD1NuMuCCOther,
    kFGD1NuMuCCOtherNoPhoton,
    kFGD1NuMuCCPhoton,
    // FGD1 CC4pi
    kFGD1NuMuCC0PiFwd,
    kFGD1NuMuCC1PiFwd,
    kFGD1NuMuCCOtherFwd,
    kFGD1NuMuCC0PiBwd,
    kFGD1NuMuCC1PiBwd,
    kFGD1NuMuCCOtherBwd,
    kFGD1NuMuCC0PiHAFwd,
    kFGD1NuMuCC1PiHAFwd,
    kFGD1NuMuCCOtherHAFwd,
    kFGD1NuMuCC0PiHABwd,
    kFGD1NuMuCC1PiHABwd,
    kFGD1NuMuCCOtherHABwd,
    // FGD2
    kFGD2NuMuCC,
    kFGD2NuMuCC0Pi,
    kFGD2NuMuCC0PiNoPhoton,
    kFGD2NuMuCC0Pi0Protons,
    kFGD2NuMuCC0PiNProtons,
    kFGD2NuMuCC0Pi0ProtonsNoPhoton,
    kFGD2NuMuCC0PiNProtonsNoPhoton,
    kFGD2NuMuCC1Pi,
    kFGD2NuMuCC1PiNoPhoton,
    kFGD2NuMuCCOther,
    kFGD2NuMuCCOtherNoPhoton,
    kFGD2NuMuCCPhoton, 
    // FGD2 CC4pi
    kFGD2NuMuCC0PiFwd,
    kFGD2NuMuCC1PiFwd,
    kFGD2NuMuCCOtherFwd,
    kFGD2NuMuCC0PiBwd,
    kFGD2NuMuCC1PiBwd,
    kFGD2NuMuCCOtherBwd,
    kFGD2NuMuCC0PiHAFwd,
    kFGD2NuMuCC1PiHAFwd,
    kFGD2NuMuCCOtherHAFwd,
    kFGD2NuMuCC0PiHABwd,
    kFGD2NuMuCC1PiHABwd,
    kFGD2NuMuCCOtherHABwd,
    // FGD1
    kFGD1NuECC,
    kFGD1NuECC0Pi,
    kFGD1NuECCOther,
    // FGD2
    kFGD2NuECC,
    kFGD2NuECC0Pi,
    kFGD2NuECCOther,
    // FGD1
    kFGD1AntiNuMuCC,
    kFGD1AntiNuMuCC1Track,
    kFGD1AntiNuMuCCNTracks,
    kFGD1AntiNuMuCC0Pi,
    kFGD1AntiNuMuCC1Pi,
    kFGD1AntiNuMuCCOther,
    // FGD2
    kFGD2AntiNuMuCC,
    kFGD2AntiNuMuCC1Track,
    kFGD2AntiNuMuCCNTracks,
    kFGD2AntiNuMuCC0Pi,
    kFGD2AntiNuMuCC1Pi,
    kFGD2AntiNuMuCCOther,
    // FGD1
    kFGD1NuMuBkgInAntiNuModeCC,
    kFGD1NuMuBkgInAntiNuModeCC1Track,
    kFGD1NuMuBkgInAntiNuModeCCNTracks,
    kFGD1NuMuBkgInAntiNuModeCC0Pi,
    kFGD1NuMuBkgInAntiNuModeCC1Pi,
    kFGD1NuMuBkgInAntiNuModeCCOther,
    // FGD2
    kFGD2NuMuBkgInAntiNuModeCC,
    kFGD2NuMuBkgInAntiNuModeCC1Track,
    kFGD2NuMuBkgInAntiNuModeCCNTracks,
    kFGD2NuMuBkgInAntiNuModeCC0Pi,
    kFGD2NuMuBkgInAntiNuModeCC1Pi,
    kFGD2NuMuBkgInAntiNuModeCCOther,
    // FGD1
    kFGD1NuMuCCBwd,
    // FGD2
    kFGD2NuMuCCBwd,
    // FGD1
    kFGD1AntiNuECC,
    kFGD1AntiNuECC0Pi,
    kFGD1AntiNuECCOther,
    // FGD2
    kFGD2AntiNuECC,
    kFGD2AntiNuECC0Pi,
    kFGD2AntiNuECCOther,
    // FGD1
    kFGD1AntiNuEBkgInNuModeCC,
    kFGD1AntiNuEBkgInNuModeCC0Pi,
    kFGD1AntiNuEBkgInNuModeCCOther,
    // FGD2
    kFGD2AntiNuEBkgInNuModeCC,
    kFGD2AntiNuEBkgInNuModeCC0Pi,
    kFGD2AntiNuEBkgInNuModeCCOther,
    // FGD1
    kFGD1NuEBkgInAntiNuModeCC,
    kFGD1NuEBkgInAntiNuModeCC0Pi,
    kFGD1NuEBkgInAntiNuModeCCOther,
    // FGD2
    kFGD2NuEBkgInAntiNuModeCC,
    kFGD2NuEBkgInAntiNuModeCC0Pi,
    kFGD2NuEBkgInAntiNuModeCCOther,
    // FGD1
    kFGD1Gamma,
    // FGD2
    kFGD2Gamma,
    // FGD1
    kFGD1GammaInAntiNuMode,
    // FGD2
    kFGD2GammaInAntiNuMode,
    // P0D
    kP0DNuMuCC,
    kNSamples
  };

  std::string ConvertSample(SampleEnum sample);
  std::set<std::string> ConvertSampleToSelection(SampleEnum sample);
  
}

#endif
