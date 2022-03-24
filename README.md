# GUNDAM inputs for the ND280 OA2020 analysis

## Getting setup

This repository does not include the OA2020 MC files. One can
find the corresponding instruction on the official T2K website:
[https://t2k.org/](https://t2k.org/).

Once downloaded on your host machine, you need to edit the path
of those files. These paths are defined in 
`inputs/datasets/configDataSets.yaml`. For example:


```bash
filePathList:
  - "/Users/ablanche/Documents/Work/Output/results/gundam/common/OA2020/NDMC_psycheInfoAdded/run4aMCsplines.root"
  - "/Users/ablanche/Documents/Work/Output/results/gundam/common/OA2020/NDMC_psycheInfoAdded/run5MCsplines.root"
```

## Running the fitter

Go to the root of this repository, and launch GUNDAM with the
following command:

```c++
gundamFitter -c fitOa2020.yaml -t 6
```

The fitter will then run with 6 parallel threads.


