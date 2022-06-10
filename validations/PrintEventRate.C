#include <cstdio>
#include <iostream>
#include <vector>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "THn.h"
#include <iostream>
#include <iomanip>

int GetNBins(THn *hist)
{

    //Loop through, multiplying the number of bins in each axis together, to
    //get the total number of bins in the n dimensional histogram.
    int nDim = hist->GetNdimensions();
    int nBins = 1;

    for (int i = 0; i < nDim; i++)
    {
        nBins *= hist->GetAxis(i)->GetNbins();
    }

    //With all axes looped over, have the full bin count.  Return the result.
    return nBins;
}

int *GetBins(THn *hist)
{

    //Extract the axes from the histogram, some information we will find useful
    //later, and information for the first bin we will consider.
    int nDim = hist->GetNdimensions();
    TAxis **axes = new TAxis *[nDim];
    int *axesNBins = new int[nDim];
    int *currentBin = new int[nDim];

    int *bins = new int[GetNBins(hist)];
    int binsIter = 0;

    for (int i = 0; i < nDim; i++)
    {
        axes[i] = hist->GetAxis(i);
        currentBin[i] = 1;
        axesNBins[i] = axes[i]->GetNbins();
    }

    //Follow a similar procedure used to load in
    //  the observable normalization parameters.
    //For each axis, bin zero is underflow, and bin Nbins+1 is overflow.
    bool firstBin = true;

    while (currentBin[0] <= axesNBins[0])
    {

        //Loop backward over each dimension, determining whether to increment.
        for (int k = nDim - 1; k >= 0; k--)
        {
            //If it's our first pass through, do nothing,
            //  since we started with the first bin of all of the axes.
            if (firstBin)
            {
                firstBin = false;
                break;
            }

            //If the value we were previously considering for this bin was the
            //last bin, reset it to the first bin, then the loop will continue
            //around to consider the next axis.
            if (currentBin[k] == axesNBins[k])
            {
                currentBin[k] = 1;
            }

            //Otherwise, consider the next bin for this axis, and break.
            else
            {
                currentBin[k]++;
                break;
            }
        }

        //Check whether this bin is the last bin (all entries equal to last
        //axis entry) and if so, break out of the while loop.
        bool lastBin = true;

        for (int i = 0; i < nDim; i++)
        {
            lastBin = lastBin && (currentBin[i] == axesNBins[i]);
        }

        //Now, we've assembled a multidimensional bin identifier. Use this to
        //get the linear bin index in the list.  Save this to the array we're
        //returning.
        bins[binsIter] = hist->GetBin(currentBin);
        binsIter++;

        if (lastBin)
        {
            break;
        }
    }

    //Delete the various arrays that aren't being returned from memory.
    //NB: We want to delete the axes array, but not the axes themselves, so
    //just call the array delete and not loop through it deleting each pointer.
    delete[] axes;
    delete[] axesNBins;
    delete[] currentBin;

    return bins;
}

void PrintEventRate()
{

    
    freopen("output.txt","w",stdout); // uncomment to save the log in a .txt file

    
    string filename = "./MultiPiRHC_v3r47_Data.root"; 
    bool doPhotonProton = false;
    
    TFile *file = new TFile(filename.c_str(), "READ");

    std::vector<TFile*> files_gundam;
    
    
    vector<string> type_display = {
        "prefit with no weights",
        "prefit with POT weights",
        "prefit with Nom.\\ flux weights",
        "prefit with Nom.\\ ObsNorm weights (574 params)",
        "prefit with Nom.\\ Det.\\ weights",
        "prefit with Flux \\& XSec weights",
        "prefit with Nom.\\ XSec \\& XSecBin weights",
//         "prefit with Flux, XSec \& XSecBin weights",
        
    };
    vector<string> type_banff = {
        "prefit_withNoWeights",
        "prefit_withPOTWeights",
        "prefit_withNomFluxWeights",
        "prefit_withNomCovWeights",
        "prefit_withNomDetWeights",
        "prefit_withNomXSecWeights",
        "prefit_withNomXSecWeights",
//         "prefit_withNomFluxAndXSecWeights",
//         "prefit_withNomFluxAndXSecWeights",
    };
    
    std::vector<string> types_gundam = {
        "NoWeights",
        "POTWeights",
        "POTFluxWeights",
        "POTObsNormWeights",
        "POTDetWeights",
        "POTXsecWeights",
        "POTXsecXsecbinWeights",
//         "withPassedPsychePileUpPOTFluxXsecWeight",
//         "withPassedPsychePileUpPOTFluxXsecXsecBinWeight",
    };

    
    // I take the event rates by running gundam dry-runs and  changing  
    // the nominalWeightFormula and/or activating sets of parameters
    // 
    for (int iType = 0; iType < types_gundam.size(); iType++){
        files_gundam.emplace_back(new TFile(Form(
            "/sps/t2k/jchakran/gundam/outputs/OA2020/validations/val_%s.root", 
            types_gundam[iType].c_str()
        )));        
    }

    // Sample names in BANFF
    vector<string> name_banff(18, "");
    name_banff[0]  = "FGD1_numuCC_0pi";
    name_banff[1]  = "FGD1_numuCC_1pi";
    name_banff[2]  = "FGD1_numuCC_other";
    name_banff[3]  = "FGD2_numuCC_0pi";
    name_banff[4]  = "FGD2_numuCC_1pi";
    name_banff[5]  = "FGD2_numuCC_other";
    name_banff[6]  = "FGD1_anti-numuCC_0pi";
    name_banff[7]  = "FGD1_anti-numuCC_1pi";
    name_banff[8]  = "FGD1_anti-numuCC_other";
    name_banff[9]  = "FGD2_anti-numuCC_0pi";
    name_banff[10] = "FGD2_anti-numuCC_1pi";
    name_banff[11] = "FGD2_anti-numuCC_other";
    name_banff[12] = "FGD1_NuMuBkg_CC0pi_in_AntiNu_Mode";
    name_banff[13] = "FGD1_NuMuBkg_CC1pi_in_AntiNu_Mode";
    name_banff[14] = "FGD1_NuMuBkg_CCother_in_AntiNu_Mode";
    name_banff[15] = "FGD2_NuMuBkg_CC0pi_in_AntiNu_Mode";
    name_banff[16] = "FGD2_NuMuBkg_CC1pi_in_AntiNu_Mode";
    name_banff[17] = "FGD2_NuMuBkg_CCother_in_AntiNu_Mode";
    
    // Sample names in gundam
    std::vector<string> name_gundam = {
        "FHC FGD1 #nu_{#mu} CC 0#pi",
        "FHC FGD1 #nu_{#mu} CC 1#pi",
        "FHC FGD1 #nu_{#mu} CC Other",
        "FHC FGD2 #nu_{#mu} CC 0#pi",
        "FHC FGD2 #nu_{#mu} CC 1#pi",
        "FHC FGD2 #nu_{#mu} CC Other",
        "RHC FGD1 #bar{#nu}_{#mu} CC 0#pi",
        "RHC FGD1 #bar{#nu}_{#mu} CC 1#pi",
        "RHC FGD1 #bar{#nu}_{#mu} CC Others",
        "RHC FGD2 #bar{#nu}_{#mu} CC 0#pi",
        "RHC FGD2 #bar{#nu}_{#mu} CC 1#pi",
        "RHC FGD2 #bar{#nu}_{#mu} CC Others",
        "RHC FGD1 #nu_{#mu} (background) CC 0#pi",
        "RHC FGD1 #nu_{#mu} (background) CC 1#pi",
        "RHC FGD1 #nu_{#mu} (background) CC Other",
        "RHC FGD2 #nu_{#mu} (background) CC 0#pi",
        "RHC FGD2 #nu_{#mu} (background) CC 1#pi",
        "RHC FGD2 #nu_{#mu} (background) CC Other",
    };
    
    // Sample names to be displayed
    std::vector<string> name_display = {
        "FHC FGD1 $\\nu_\\mu$ CC$0\\pi$",
        "FHC FGD1 $\\nu_\\mu$ CC$1\\pi$",
        "FHC FGD1 $\\nu_\\mu$ CCOth",
        "FHC FGD2 $\\nu_\\mu$ CC$0\\pi$",
        "FHC FGD2 $\\nu_\\mu$ CC$1\\pi$",
        "FHC FGD2 $\\nu_\\mu$ CCOth",
        "RHC FGD1 $\\bar\\nu_\\mu$ CC$0\\pi$",
        "RHC FGD1 $\\bar\\nu_\\mu$ CC$1\\pi$",
        "RHC FGD1 $\\bar\\nu_\\mu$ CCOth",
        "RHC FGD2 $\\bar\\nu_\\mu$ CC$0\\pi$",
        "RHC FGD2 $\\bar\\nu_\\mu$ CC$1\\pi$",
        "RHC FGD2 $\\bar\\nu_\\mu$ CCOth",
        "RHC FGD1 $\\nu_\\mu$ (bkg) CC$0\\pi$",
        "RHC FGD1 $\\nu_\\mu$ (bkg) CC$1\\pi$",
        "RHC FGD1 $\\nu_\\mu$ (bkg) CCOth",
        "RHC FGD2 $\\nu_\\mu$ (bkg) CC$0\\pi$",
        "RHC FGD2 $\\nu_\\mu$ (bkg) CC$1\\pi$",
        "RHC FGD2 $\\nu_\\mu$ (bkg) CCOth",
    };

    std::cout << "\\documentclass[multi=my,crop]{standalone}\n\\usepackage[utf8]{inputenc}\n\\begin{document}" << std::endl; 

    
    for (int iName = 0; iName < name_banff.size(); iName++)
    {
        
        
        std::cout << std::endl;
        std::cout << "\\begin{my}" << std::endl; 
        std::cout << "\\begin{tabular}{|l|c|c|}" << std::endl; 
        std::cout << "\\hline" << std::endl;
        std::cout << "\\hline" << std::endl;

        std::cout << name_display[iName] << " & BANFF & GUNDAM \\\\"  << std::endl; 
        std::cout << "\\hline" << std::endl;

        double totalMCEvents = 0.;
        for (int iType = 0; iType < type_banff.size(); ++iType)
        {

            // ***** BANFF *****

            std::string histoName = name_banff[iName] + "_" + type_banff[iType];

            THnD *histo = (THnD *)file->Get(histoName.c_str());
            double MCcounter = 0.;
            double MCcounterUnbinned = 0.;

            if (histo)
            {
                int *bins = GetBins(histo);
                for (int iBins = 0; iBins < GetNBins(histo); iBins++)
                {
                    MCcounter += histo->GetBinContent(bins[iBins]);
                }
                for (int iBins = 0; iBins < histo->GetNbins(); iBins++)
                {
                    MCcounterUnbinned += histo->GetBinContent(iBins);
                }
                
                // adding pmu underflow bin for debug
                TH1D* pmu_projection = histo->Projection(0);
                
                std::cout << type_display[iType] << " & "
                          << MCcounter << " & ";

                totalMCEvents += histo->GetEntries();
                
                delete pmu_projection;
                
                

            }
        
        
            // ***** GUNDAM *****
            // Event rates are obtained from the weights of individual events saved by sample
            // These don't include the events that fell out of the binning
            //
            TTreeReader r_prefit(Form("FitterEngine/preFit/events/%s/MC_TTree", name_gundam[iName].c_str()), files_gundam[iType]);
            TTreeReaderValue<double> PreFitWeight_event = {r_prefit, "Event.eventWeight"}; //treeWeight

            double rate = 0;
            while (r_prefit.Next()){
                rate += *PreFitWeight_event;
            }
        
            std::cout << rate << " \\\\" << std::endl;
            std::cout << "\\hline" << std::endl;
            
        
        
        
        }
        std::cout << "\\hline" << std::endl;
        std::cout << "\\end{tabular}" << std::endl; 
        std::cout << "\\end{my}" << std::endl; 

    }

    file->Close();

    std::cout << "\\end{document}" << std::endl; 
}