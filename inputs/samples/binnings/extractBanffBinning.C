// From BinningDefinition.cxx of the BANFF

// FHCNumuCC0Pi
int    FHCNumuCC0Pi_Mom_NBin        = 29;
double FHCNumuCC0Pi_Mom_Bin[30]     = {0.   , 200. , 300. , 400. , 450. , 500. , 550. , 600. ,
                                       650. , 700. , 750. , 800. , 850. , 900. , 950. , 1000.,
                                       1050., 1100., 1200., 1300., 1400., 1500., 1600., 1700.,
                                       1800., 2000., 2500., 3000., 5000., 30000.};
int    FHCNumuCC0Pi_Cos_NBin = 29;
double FHCNumuCC0Pi_Cos_Bin[30];

// FHCNumuCC1Pi
int    FHCNumuCC1Pi_Mom_NBin        = 18;
double FHCNumuCC1Pi_Mom_Bin[19]     = {0.   , 300. , 350. , 400. , 500. , 600. , 650. , 700. ,
                                       750. , 800. , 900. , 1000., 1100., 1200., 1500., 2000.,
                                       3000., 5000., 30000.};
int    FHCNumuCC1Pi_Cos_NBin = 16;
double FHCNumuCC1Pi_Cos_Bin[20];

//FHCNumuCCOth
int    FHCNumuCCOth_Mom_NBin        = 18;
double FHCNumuCCOth_Mom_Bin[19]     = {0.   , 300. , 400. , 500. , 600. , 650. , 700. , 750. ,
                                       800. , 900. , 1000., 1100., 1250., 1500., 1750., 2000.,
                                       3000., 5000., 30000.};
int    FHCNumuCCOth_Cos_NBin = 19;
double FHCNumuCCOth_Cos_Bin[20];

//RHCANumuCC0Pi
int    RHCANumuCC0Pi_Mom_NBin       = 17;
double RHCANumuCC0Pi_Mom_Bin[18]    = {0.   , 300. , 400. , 500. , 550. , 600. , 650. , 700. ,
                                       750. , 800. , 900. , 1000., 1100., 1200., 1500., 2000.,
                                       4000., 30000.};
int    RHCANumuCC0Pi_Cos_NBin       = 18;
double RHCANumuCC0Pi_Cos_Bin[19]    = {-1.0  , 0.6  , 0.7  , 0.8  , 0.85 , 0.9  , 0.92 , 0.93 ,
                                       0.94 , 0.95 , 0.96 , 0.965, 0.97 , 0.975, 0.98 , 0.985,
                                       0.99 , 0.995, 1.0};

//RHCANumuCC1Pi
int    RHCANumuCC1Pi_Mom_NBin       = 6;
double RHCANumuCC1Pi_Mom_Bin[7]     = {0.   , 500. , 700. , 900. , 1300., 2500., 30000.};
int    RHCANumuCC1Pi_Cos_NBin       = 8;
double RHCANumuCC1Pi_Cos_Bin[9]     = {-1.0 , 0.7 , 0.8 , 0.9 , 0.94, 0.96, 0.98, 0.99, 1.0};

//RHCANumuCCOth
int    RHCANumuCCOth_Mom_NBin       = 8;
double RHCANumuCCOth_Mom_Bin[9]     = {0. , 600. , 800. , 1000., 1250., 1500., 2000., 4000., 30000.};
int    RHCANumuCCOth_Cos_NBin       = 10;
double RHCANumuCCOth_Cos_Bin[11]    = {-1.0 , 0.7 , 0.8, 0.85, 0.9, 0.93, 0.95, 0.97, 0.98, 0.99, 1.0};

//RHCNumuCC0Pi
int    RHCNumuCC0Pi_Mom_NBin       = 10;
double RHCNumuCC0Pi_Mom_Bin[11]    = {0. , 300. , 500. , 700. , 800. , 900. , 1250., 1500., 2000., 4000., 30000.};
int    RHCNumuCC0Pi_Cos_NBin       = 12;
double RHCNumuCC0Pi_Cos_Bin[13]    = {-1.0 , 0.7 , 0.8 , 0.85, 0.88, 0.9 , 0.92, 0.94,
                                      0.96, 0.97, 0.98, 0.99, 1.0};

//RHCNumuCC1Pi
int    RHCNumuCC1Pi_Mom_NBin       = 4;
double RHCNumuCC1Pi_Mom_Bin[5]     = {0. , 600. , 800. , 1500., 30000.};
int    RHCNumuCC1Pi_Cos_NBin       = 10;
double RHCNumuCC1Pi_Cos_Bin[11]    = {-1.0 , 0.7 , 0.8 , 0.86, 0.9 , 0.94, 0.96, 0.97, 0.98, 0.99, 1.0};

//RHCNumuCCOth
int    RHCNumuCCOth_Mom_NBin       = 6;
double RHCNumuCCOth_Mom_Bin[7]     = {0. , 600. , 1000., 1250., 2000., 4000., 30000.};
int    RHCNumuCCOth_Cos_NBin       = 9;
double RHCNumuCCOth_Cos_Bin[10]    = {-1.0, 0.7, 0.8, 0.86, 0.9 , 0.93, 0.95, 0.97, 0.99, 1.0};

void fillBinArrays();
std::string makeBinning(double* pBounds, int pNbins, double* cosBounds, int cosNbins);

void extractBanffBinning(){

  fillBinArrays();

  GenericToolbox::dumpStringInFile(
    "FHCNumuCC0Pi.txt",
    makeBinning(FHCNumuCC0Pi_Mom_Bin ,FHCNumuCC0Pi_Mom_NBin, FHCNumuCC0Pi_Cos_Bin, FHCNumuCC0Pi_Cos_NBin)
  );
  GenericToolbox::dumpStringInFile(
      "FHCNumuCC1Pi.txt",
      makeBinning(FHCNumuCC1Pi_Mom_Bin ,FHCNumuCC1Pi_Mom_NBin, FHCNumuCC1Pi_Cos_Bin, FHCNumuCC1Pi_Cos_NBin)
  );
  GenericToolbox::dumpStringInFile(
      "FHCNumuCCOth.txt",
      makeBinning(FHCNumuCCOth_Mom_Bin ,FHCNumuCCOth_Mom_NBin, FHCNumuCCOth_Cos_Bin, FHCNumuCCOth_Cos_NBin)
  );

  GenericToolbox::dumpStringInFile(
      "RHCANumuCC0Pi.txt",
      makeBinning(RHCANumuCC0Pi_Mom_Bin ,RHCANumuCC0Pi_Mom_NBin, RHCANumuCC0Pi_Cos_Bin, RHCANumuCC0Pi_Cos_NBin)
  );
  GenericToolbox::dumpStringInFile(
      "RHCANumuCC1Pi.txt",
      makeBinning(RHCANumuCC1Pi_Mom_Bin ,RHCANumuCC1Pi_Mom_NBin, RHCANumuCC1Pi_Cos_Bin, RHCANumuCC1Pi_Cos_NBin)
  );
  GenericToolbox::dumpStringInFile(
      "RHCANumuCCOth.txt",
      makeBinning(RHCANumuCCOth_Mom_Bin ,RHCANumuCCOth_Mom_NBin, RHCANumuCCOth_Cos_Bin, RHCANumuCCOth_Cos_NBin)
  );

  GenericToolbox::dumpStringInFile(
      "RHCNumuCC0Pi.txt",
      makeBinning(RHCNumuCC0Pi_Mom_Bin ,RHCNumuCC0Pi_Mom_NBin, RHCNumuCC0Pi_Cos_Bin, RHCNumuCC0Pi_Cos_NBin)
  );
  GenericToolbox::dumpStringInFile(
      "RHCNumuCC1Pi.txt",
      makeBinning(RHCNumuCC1Pi_Mom_Bin ,RHCNumuCC1Pi_Mom_NBin, RHCNumuCC1Pi_Cos_Bin, RHCNumuCC1Pi_Cos_NBin)
  );
  GenericToolbox::dumpStringInFile(
      "RHCNumuCCOth.txt",
      makeBinning(RHCNumuCCOth_Mom_Bin ,RHCNumuCCOth_Mom_NBin, RHCNumuCCOth_Cos_Bin, RHCNumuCCOth_Cos_NBin)
  );

  exit(0);
}

std::string makeBinning(double* pBounds, int pNbins, double* cosBounds, int cosNbins){
  std::stringstream ss;
  ss << "variables: CosThetamu CosThetamu Pmu Pmu" << std::endl;

  for( int iCos = 0 ; iCos < cosNbins ; iCos++ ){
    for( int iP = 0 ; iP < pNbins ; iP++ ){
      ss << cosBounds[iCos] << " " << cosBounds[iCos+1] << " ";
      ss << pBounds[iP] << " " << pBounds[iP+1] << std::endl;
    }
  }

  return ss.str();
}


void fillBinArrays(){

  FHCNumuCC0Pi_Cos_NBin =  29;
  FHCNumuCC0Pi_Cos_Bin[ 0] = -1.0  ;
  FHCNumuCC0Pi_Cos_Bin[ 1] =  0.5  ;
  FHCNumuCC0Pi_Cos_Bin[ 2] =  0.6  ;
  FHCNumuCC0Pi_Cos_Bin[ 3] =  0.7  ;
  FHCNumuCC0Pi_Cos_Bin[ 4] =  0.76 ;
  FHCNumuCC0Pi_Cos_Bin[ 5] =  0.78 ;
  FHCNumuCC0Pi_Cos_Bin[ 6] =  0.8  ;
  FHCNumuCC0Pi_Cos_Bin[ 7] =  0.83 ;
  FHCNumuCC0Pi_Cos_Bin[ 8] =  0.85 ;
  FHCNumuCC0Pi_Cos_Bin[ 9] =  0.88 ;
  FHCNumuCC0Pi_Cos_Bin[10] =  0.89 ;
  FHCNumuCC0Pi_Cos_Bin[11] =  0.9  ;
  FHCNumuCC0Pi_Cos_Bin[12] =  0.91 ;
  FHCNumuCC0Pi_Cos_Bin[13] =  0.92 ;
  FHCNumuCC0Pi_Cos_Bin[14] =  0.925;
  FHCNumuCC0Pi_Cos_Bin[15] =  0.93 ;
  FHCNumuCC0Pi_Cos_Bin[16] =  0.935;
  FHCNumuCC0Pi_Cos_Bin[17] =  0.94 ;
  FHCNumuCC0Pi_Cos_Bin[18] =  0.945;
  FHCNumuCC0Pi_Cos_Bin[19] =  0.95 ;
  FHCNumuCC0Pi_Cos_Bin[20] =  0.955;
  FHCNumuCC0Pi_Cos_Bin[21] =  0.96 ;
  FHCNumuCC0Pi_Cos_Bin[22] =  0.965;
  FHCNumuCC0Pi_Cos_Bin[23] =  0.97 ;
  FHCNumuCC0Pi_Cos_Bin[24] =  0.975;
  FHCNumuCC0Pi_Cos_Bin[25] =  0.98 ;
  FHCNumuCC0Pi_Cos_Bin[26] =  0.985;
  FHCNumuCC0Pi_Cos_Bin[27] =  0.99 ;
  FHCNumuCC0Pi_Cos_Bin[28] =  0.995;
  FHCNumuCC0Pi_Cos_Bin[29] =  1.0  ;

  FHCNumuCC1Pi_Cos_NBin = 16;
  FHCNumuCC1Pi_Cos_Bin[ 0] = -1.0  ;
  FHCNumuCC1Pi_Cos_Bin[ 1] =  0.6  ;
  FHCNumuCC1Pi_Cos_Bin[ 2] =  0.7  ;
  FHCNumuCC1Pi_Cos_Bin[ 3] =  0.8  ;
  FHCNumuCC1Pi_Cos_Bin[ 4] =  0.85 ;
  FHCNumuCC1Pi_Cos_Bin[ 5] =  0.88 ;
  FHCNumuCC1Pi_Cos_Bin[ 6] =  0.9  ;
  FHCNumuCC1Pi_Cos_Bin[ 7] =  0.92 ;
  FHCNumuCC1Pi_Cos_Bin[ 8] =  0.93 ;
  FHCNumuCC1Pi_Cos_Bin[ 9] =  0.94 ;
  FHCNumuCC1Pi_Cos_Bin[10] =  0.95 ;
  FHCNumuCC1Pi_Cos_Bin[11] =  0.96 ;
  FHCNumuCC1Pi_Cos_Bin[12] =  0.97 ;
  FHCNumuCC1Pi_Cos_Bin[13] =  0.98 ;
  FHCNumuCC1Pi_Cos_Bin[14] =  0.99 ;
  FHCNumuCC1Pi_Cos_Bin[15] =  0.995;
  FHCNumuCC1Pi_Cos_Bin[16] =  1.0  ;

  FHCNumuCCOth_Cos_NBin = 19;
  FHCNumuCCOth_Cos_Bin[ 0] = -1.0  ;
  FHCNumuCCOth_Cos_Bin[ 1] =  0.6  ;
  FHCNumuCCOth_Cos_Bin[ 2] =  0.7  ;
  FHCNumuCCOth_Cos_Bin[ 3] =  0.76 ;
  FHCNumuCCOth_Cos_Bin[ 4] =  0.8  ;
  FHCNumuCCOth_Cos_Bin[ 5] =  0.85 ;
  FHCNumuCCOth_Cos_Bin[ 6] =  0.88 ;
  FHCNumuCCOth_Cos_Bin[ 7] =  0.89 ;
  FHCNumuCCOth_Cos_Bin[ 8] =  0.9  ;
  FHCNumuCCOth_Cos_Bin[ 9] =  0.91 ;
  FHCNumuCCOth_Cos_Bin[10] =  0.92 ;
  FHCNumuCCOth_Cos_Bin[11] =  0.93 ;
  FHCNumuCCOth_Cos_Bin[12] =  0.94 ;
  FHCNumuCCOth_Cos_Bin[13] =  0.95 ;
  FHCNumuCCOth_Cos_Bin[14] =  0.96 ;
  FHCNumuCCOth_Cos_Bin[15] =  0.97 ;
  FHCNumuCCOth_Cos_Bin[16] =  0.98 ;
  FHCNumuCCOth_Cos_Bin[17] =  0.99 ;
  FHCNumuCCOth_Cos_Bin[18] =  0.995;
  FHCNumuCCOth_Cos_Bin[19] =  1.0  ;

}