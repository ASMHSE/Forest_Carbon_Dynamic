# Forest_Carbon_Dynamic
SmplSimWoody 1.0. A Light FOMSI Simulator

## The Model

FOMSI (FOrest Model SImulator) is a deterministic model to simulate forest population dynamics and predict woody carbon stocks and sequestration rates for different silvopastoral systems. The initial conditions (e.g. recruitment rate) and management strategies during the simulation sequence determined the outputs. The only stochastic component in the model was plant mortality rate. The model comprises the assemblage of three general model components: a population, a production, and a management model. The first component comprises biophysical processes, represented by trees and shrubs recruitment, growth, and mortality (population model). The second component represents a forest exploitation based on population (production model). The third component of the model includes processes regulated by human intervention (management model). (Filip et al., 2025)

## The Exercise

To evaluate long term forest and woody carbon dynamics of two silvopastoral systems: Forest management integrated with livestock (herein MBGI) and pretended (or false) silvopastoral (FSP), and to compare these with undisturbed primary native forests (UPNF) (Filip et al., 2025).

## The Results
   
   Model details and parameters presented in article are enough complete to allow a succesfull implementation.
   
   Look like the general model form represent woody population dynamics along time as intended.

   But, to acomodate this results, recruitment mortality must be 55% for shrubs and 14% for A.nigra wich, anyway, are guessed parameters.
   Beside, newly recruited trees in a year depends on mature tree population, not entire tree population.

   ![FOMSI_10](https://github.com/user-attachments/assets/6bfb765f-e52d-4666-8dc6-5b12c7d4f779)


   Unfortunatly, results presented in the article apparently correspond to a different model, most likely same general model form with other parameters. No published graphic or table can be reproduced.
    
 |                    |     UPNF     |       MBGI    |       FSP   |
 | --- | ---: | ---: | ---: |
 | Tree recruitment   |       3.21   |        2.97   |       0.82  | 
 | Tree density       |     107.55   |       97.86   |      11.46  |
 | Tree basal area    |       3.14   |        2.83   |       0.52  |
 | Shrub recruitment  |     600.50   |      578.33   |      14.94  | 
 | Shrub density      |    4758.72   |     3520.73   |      47.31  | 
 | Tree C storage     |   13383.92   |    11964.65   |    2080.32  |
 | Shrub C storage    |    9005.76   |     5422.01   |     152.45  |
 | Woody C storage    |   22389.67   |    17386.66   |    2232.76  |


   It's obvious that most values are clearly lower than those presented in article. 
   Most notable disagreement is in recruitment row. That perhaps it's a severe mistake. Hardly yearly recruitment could be 74% of total population when higher yearly recruitment parameter is 21% of mature population.

   Finally, even with different numbers, this results support discussions and considerations presented in the article.

note: 
     1 - Published Table 3 say "mean weigthed C in the system" where have to say "mean weigthed total woody C storage", because herbaceous layer is not accounted. That is important because you should expect higher herbaceous C storage in disturbed systems, wich at least could balance a little disturbances.
      2 - Here dead roots carbon is included in Tree C storage. Article don't explicit were this carbon is accounted.
    

## The Program

[![](https://img.shields.io/badge/Assembler-UASM%20v2.56-green.svg?style=flat-square&logo=visual-studio-code&logoColor=white&colorB=1CC887)](http://www.terraspace.co.uk/uasm.html) 

Framework : ObjAsm C.2  - masters : [Main Developer](https://github.com/ObjAsm/ObjAsm-C.2) , [Fork](https://github.com/ASMHSE/ObjAsm-C.2/tree/master)

Math syntax macros: [SmplMath](https://github.com/ASMHSE/SmplMath)

Programming is Object Oriented, and consist of WoodyCarbon Simulator object controlling FOMSI model object runnings, and a Single Document Interface application descendent object who run the simulations and show graphics and tables. 

Current buildings are for Windows platform, 32 or 64 bits, and ANSI outputs.

note: file to store stabilized population is different for each bitness.

## Reference

Filip I. D., Peri P. L., Fernández P., Loto D., Kees S. M., Banegas N., Nasca J., Sacido M., Faverin C., Vibart R. Modelling woody carbon dynamics in tropical dry forests: A case study of the Dry Chaco Region. Ecological Modelling. 2026. Vol. 512. p. 111401.
