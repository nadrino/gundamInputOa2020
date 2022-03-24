const int kNneutModes = 26;

enum neutModeEnum {  
  tot,
  ccqe,  
  ccppip,
  ccppi0,
  ccnpip,
  cccoh, 
  ccgam, 
  ccmpi, 
  cceta, 
  cck,   
  ccdis, 
  ncnpi0,
  ncppi0,
  ncppim,
  ncnpip,
  nccoh, 
  ncngam,
  ncpgam,
  ncmpi, 
  ncneta,
  ncpeta,
  nck0,  
  nckp,  
  ncdis, 
  ncqep, 
  ncqen
};

int neutModeID[kNneutModes] = {

  0,
  1, 
  11,
  12,
  13,
  16,
  17,
  21,
  22,
  23,
  26,
  31,
  32,
  33,
  34,
  36,
  38,
  39,
  41,
  42,
  43,
  44,
  45,
  46,
  51,
  52

};

char neutModeName[kNneutModes][7] = {

  "tot",
  "ccqe",  
  "ccppip",
  "ccppi0",
  "ccnpip",
  "cccoh", 
  "ccgam", 
  "ccmpi", 
  "cceta", 
  "cck",   
  "ccdis", 
  "ncnpi0",
  "ncppi0",
  "ncppim",
  "ncnpip",
  "nccoh", 
  "ncngam",
  "ncpgam",
  "ncmpi", 
  "ncneta",
  "ncpeta",
  "nck0",  
  "nckp",  
  "ncdis", 
  "ncqep", 
  "ncqen"

};

char neutModeTitle[kNneutModes][75] = {

  "Total",
  "CCQE: #nu_{l} n #rightarrow l^{-} p",					   
  "CC 1#pi: #nu_{l} p #rightarrow l^{-} p #pi^{+}",			   
  "CC 1#pi: #nu_{l} n #rightarrow l^{-} p #pi^{0}",			   
  "CC 1#pi: #nu_{l} n #rightarrow l^{-} n #pi^{+}",			   
  "CC coherent-#pi: #nu_{l} ^{16}O #rightarrow l^{-} ^{16}O #pi^{+}",	   
  "1#gamma from #Delta: #nu_{l} n #rightarrow l^{-} p #gamma",		   
  "CC (1.3 < W < 2 GeV): #nu_{l} N #rightarrow l^{-} N' multi-#pi",	   
  "CC 1#eta: #nu_{l} n #rightarrow l^{-} p #eta",				   
  "CC 1K: #nu_{l} n #rightarrow l^{-} #Lambda K^{+}",			   
  "CC DIS (2 GeV < W): #nu_{l} N #rightarrow l^{-} N' mesons",		   
  "NC 1#pi: #nu_{l} n #rightarrow #nu_{l} n #pi^{0}",			   
  "NC 1#pi: #nu_{l} p #rightarrow #nu_{l} p #pi^{0}",			   
  "NC 1#pi: #nu_{l} n #rightarrow #nu_{l} p #pi^{-}",			   
  "NC 1#pi: #nu_{l} p #rightarrow #nu_{l} n #pi^{+}",			   
  "NC coherent-#pi: #nu_{l} ^{16}O #rightarrow #nu_{l} ^{16}O #pi^{0}",	   
  "1#gamma from #Delta: #nu_{l} n #rightarrow #nu_{l} n #gamma",		   
  "1#gamma from #Delta: #nu_{l} p #rightarrow #nu_{l} p #gamma",		   
  "NC (1.3 < W < 2 GeV): #nu_{l} N #rightarrow #nu_{l} N multi-#pi",	   
  "NC 1#eta: #nu_{l} n #rightarrow #nu_{l} n #eta",			   
  "NC 1#eta: #nu_{l} p #rightarrow #nu_{l} p #eta",			   
  "NC 1K: #nu_{l} n #rightarrow #nu_{l} #Lambda K^{0}",			   
  "NC 1K: #nu_{l} n #rightarrow #nu_{l} #Lambda K^{+}",			   
  "NC DIS (2 GeV < W): #nu_{l} N #rightarrow #nu_{l} N' mesons",		   
  "NC elastic: #nu_{l} p #rightarrow #nu_{l} p",				   
  "NC elastic: #nu_{l} n #rightarrow #nu_{l} n"

};



const Int_t kNneutNus = 4;
char  neutNuName[kNneutNus][6]  = {"numu","nue","numub","nueb"};
char neutNuTitle[kNneutNus][13] = {"#nu#mu","#nue","#bar{#nu#mu}","#bar{#nue}"};
int     neutNuID[kNneutNus]     = {14,12,-14,-12};



/*
int neutModeID[kNneutModes];
char neutModeName[kNneutModes][7];
char neutModeTitle[kNneutModes][75];
 neutModeID[0] = 1;    neutModeName[0] = "ccqe";    neutModeTitle[0] = "CCQE: #nu_{l} n #rightarrow l^{-} p";
 neutModeID[1] = 11;   neutModeName[1] = "ccppip";  neutModeTitle[1] = "CC 1#pi: #nu_{l} p #rightarrow l^{-} p #pi^{+}";
 neutModeID[2] = 12;   neutModeName[2] = "ccppi0";  neutModeTitle[2] = "CC 1#pi: #nu_{l} n #rightarrow l^{-} p #pi^{0}";
 neutModeID[3] = 13;   neutModeName[3] = "ccnpip";  neutModeTitle[3] = "CC 1#pi: #nu_{l} n #rightarrow l^{-} n #pi^{+}";
 neutModeID[4] = 16;   neutModeName[4] = "cccoh";   neutModeTitle[4] = "CC coherent-#pi: #nu_{l} ^{16}O #rightarrow l^{-} ^{16}O #pi^{+}";
 neutModeID[5] = 17;   neutModeName[5] = "ccgam";   neutModeTitle[5] = "1#gamma from #Delta: #nu_{l} n #rightarrow l^{-} p #gamma";
 neutModeID[6] = 21;   neutModeName[6] = "ccmpi";   neutModeTitle[6] = "CC (1.3 < W < 2 GeV): #nu_{l} N #rightarrow l^{-} N' multi-#pi";
 neutModeID[7] = 22;   neutModeName[7] = "cceta";   neutModeTitle[7] = "CC 1#eta: #nu_{l} n #rightarrow l^{-} p #eta";
 neutModeID[8] = 23;   neutModeName[8] = "cck";     neutModeTitle[8] = "CC 1K: #nu_{l} n #rightarrow l^{-} #Lambda K^{+}";
 neutModeID[9] = 26;   neutModeName[9] = "ccdis";   neutModeTitle[9] = "CC DIS (2 GeV < W): #nu_{l} N #rightarrow l^{-} N' mesons";
neutModeID[10] = 31;  neutModeName[10] = "ncnpi0"; neutModeTitle[10] = "NC 1#pi: #nu_{l} n #rightarrow #nu_{l} n #pi^{0}";
neutModeID[11] = 32;  neutModeName[11] = "ncppi0"; neutModeTitle[11] = "NC 1#pi: #nu_{l} p #rightarrow #nu_{l} p #pi^{0}";
neutModeID[12] = 33;  neutModeName[12] = "ncppim"; neutModeTitle[12] = "NC 1#pi: #nu_{l} n #rightarrow #nu_{l} p #pi^{-}";
neutModeID[13] = 34;  neutModeName[13] = "ncnpip"; neutModeTitle[13] = "NC 1#pi: #nu_{l} p #rightarrow #nu_{l} n #pi^{+}";
neutModeID[14] = 36;  neutModeName[14] = "nccoh";  neutModeTitle[14] = "NC coherent-#pi: #nu_{l} ^{16}O #rightarrow #nu_{l} ^{16}O #pi^{0}";
neutModeID[15] = 38;  neutModeName[15] = "ncngam"; neutModeTitle[15] = "1#gamma from #Delta: #nu_{l} n #rightarrow #nu_{l} n #gamma";
neutModeID[16] = 39;  neutModeName[16] = "ncpgam"; neutModeTitle[16] = "1#gamma from #Delta: #nu_{l} p #rightarrow #nu_{l} p #gamma";
neutModeID[17] = 41;  neutModeName[17] = "ncmpi";  neutModeTitle[17] = "NC (1.3 < W < 2 GeV): #nu_{l} N #rightarrow #nu_{l} N multi-#pi";
neutModeID[18] = 42;  neutModeName[18] = "ncneta"; neutModeTitle[18] = "NC 1#eta: #nu_{l} n #rightarrow #nu_{l} n #eta";
neutModeID[19] = 43;  neutModeName[19] = "ncpeta"; neutModeTitle[19] = "NC 1#eta: #nu_{l} p #rightarrow #nu_{l} p #eta";
neutModeID[20] = 44;  neutModeName[20] = "nck0";   neutModeTitle[20] = "NC 1K: #nu_{l} n #rightarrow #nu_{l} #Lambda K^{0}";
neutModeID[21] = 45;  neutModeName[21] = "nckp";   neutModeTitle[21] = "NC 1K: #nu_{l} n #rightarrow #nu_{l} #Lambda K^{+}";
neutModeID[22] = 46;  neutModeName[22] = "ncdis";  neutModeTitle[22] = "NC DIS (2 GeV < W): #nu_{l} N #rightarrow #nu_{l} N' mesons";
neutModeID[23] = 51;  neutModeName[23] = "ncqep";  neutModeTitle[23] = "NC elastic: #nu_{l} p #rightarrow #nu_{l} p";
neutModeID[24] = 52;  neutModeName[24] = "ncqen";  neutModeTitle[24] = "NC elastic: #nu_{l} n #rightarrow #nu_{l} n";                              
*/
