## Serial results

### Only Adjacency Matrix:
***loop starts @ col = 0***

Process Type      Num. of Vertices      Num. of Edges      Num. of Deleted Edges      Duration

============      ================      =============       ==============  

SERIAL               125                5500                  2660                    0.605171

SERIAL               100                4850                  707                    0.145814

SERIAL               200                19700                  19601                    0.0486502

SERIAL               300                44550                  7676                    31.1906

SERIAL               400                79400                  6184                    99.367

SERIAL               500                124250                  5561                    236.727


### Only Adjacency Matrix + OpenMP:
- ***loop starts @ col = 0***
- ***Only Outer loop is parallelised***

Process Type      Num. of Vertices      Num. of Edges      Num. of Deleted Edges      Duration

SERIAL               125                5500                  2605                    0.390853

SERIAL               100                4850                  641                    0.0760429

SERIAL               200                19700                  21970                    0.0402976

SERIAL               300                44550                  11837                    13.4957

SERIAL               400                79400                  8640                    46.5436

SERIAL               500                124250                  6404                    117.661


### Only Adjacency Matrix + OpenMP:
- ***loop starts @ col = 0***
- ***Only Outer loop is parallelised***
-  ***Add default(none) and opm critical***

Process Type      Num. of Vertices      Num. of Edges      Num. of Deleted Edges      Duration

SERIAL               125                5500                  2649                    0.296766

SERIAL               100                4850                  714                    0.0561219

SERIAL               200                19700                  19614                    0.0748561

SERIAL               300                44550                  10336                    14.4571

SERIAL               400                79400                  6782                    46.1124

SERIAL               500                124250                  3327                    113.88


### Only Adjacency Matrix + OpenMP:
- ***loop starts @ col = 0***
- ***Only Outer loop is parallelised***
- ***Add default(none) and REMOVE opm critical***


Process Type      Num. of Vertices      Num. of Edges      Num. of Deleted Edges      Duration

SERIAL               125                5500                  2570                    0.239513

SERIAL               100                4850                  704                    0.0520876

SERIAL               200                19700                  30701                    0.0696711

SERIAL               300                44550                  8737                    14.4681

SERIAL               400                79400                  10504                    44.4459

SERIAL               500                124250                  1031                    116.569


### Only Adjacency Matrix + OpenMP:
- ***loop starts @ col = 0***
- ***Only Outer loop is parallelised***
- ***Add default(none) and REMOVE opm critical***
- ***Add collapse(2)***


Process Type      Num. of Vertices      Num. of Edges      Num. of Deleted Edges      Duration

SERIAL               125                5500                  2674                    0.403849

SERIAL               100                4850                  680                    0.0949218

SERIAL               200                19700                  32040                    0.0392222

SERIAL               300                44550                  8257                    14.0541

SERIAL               400                79400                  11037                    44.2329

SERIAL               500                124250                  5464                    112.246


### Only Adjacency Matrix + OpenMP:
- ***loop starts @ col = 0***
- ***Only Outer loop is parallelised***
- ***Add default(none) and REMOVE opm critical***
- ***Add collapse(2)***
- ***Add reduction and task***

Process Type      Num. of Vertices      Num. of Edges      Num. of Deleted Edges      Duration

SERIAL               125                5500                  0                    0.356018

SERIAL               100                4850                  0                    0.00826016

SERIAL               200                19700                  0                    2.13446

SERIAL               300                44550                  0                    11.3645

SERIAL               400                79400                  0                    39.4461

SERIAL               500                124250                  0                    96.6107


### Only Adjacency Matrix + OpenMP:
- ***loop starts @ col = 0***
- ***Only Outer loop is parallelised***
- ***Add default(none) and REMOVE opm critical***
- ***Add collapse(2)***
- ***Add reduction and REMOVE task***

Process Type      Num. of Vertices      Num. of Edges      Num. of Deleted Edges      Duration

SERIAL               125                5500                  2782                    0.259148

SERIAL               100                4850                  648                    0.0806727

SERIAL               200                19700                  27565                    0.0442027

SERIAL               300                44550                  10678                    13.7411

SERIAL               400                79400                  6573                    45.3007

SERIAL               500                124250                  4034                    112.536

### Only Adjacency Matrix + OpenMP:
- ***loop starts @ col = 0***
- ***Only Outer loop is parallelised***
- ***Add default(none) and REMOVE opm critical***
- ***Add collapse(2)***
- ***Add reduction and task***


Process Type      Num. of Vertices      Num. of Edges      Num. of Deleted Edges      Duration

SERIAL               125                5500                  2720                    0.188811

SERIAL               100                4850                  671                    0.047011

SERIAL               200                19700                  27392                    0.0316661

SERIAL               300                44550                  12070                    11.4996

SERIAL               400                79400                  6687                    39.6263

SERIAL               500                124250                  6134                    97.2541


### Only Adjacency Matrix + OpenMP:
- ***loop starts @ col = 0***
- ***Only Outer loop is parallelised***
- ***Add default(none) and REMOVE opm critical***
- ***Add collapse(2)***
- ***Add reduction and task***



### Only Adjacency Matrix + OpenMP:
- ***loop starts @ col = 0***
- ***Only Outer loop is parallelised***
- ***Add default(none) and REMOVE opm critical***
- ***Add collapse(2)***
- ***Add reduction and task***



