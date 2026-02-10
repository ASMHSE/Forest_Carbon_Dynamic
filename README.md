# Forest_Carbon_Dynamic
SmplSimWoody 1.0. A Light FOMSI Simulator

## The Model

FOMSI (FOrest Model SImulator) is a deterministic model to simulate forest population dynamics and predict woody carbon stocks and sequestration rates for different silvopastoral systems. The initial conditions (e.g. recruitment rate) and management strategies during the simulation sequence determined the outputs. The only stochastic component in the model was plant mortality rate. The model comprises the assemblage of three general model components: a population, a production, and a management model. The first component comprises biophysical processes, represented by trees and shrubs recruitment, growth, and mortality (population model). The second component represents a forest exploitation based on population (production model). The third component of the model includes processes regulated by human intervention (management model). (Filip et al., 2025)

## The Exercise

To evaluate long term forest and woody carbon dynamics of two silvopastoral systems: Forest management integrated with livestock (herein MBGI) and pretended (or false) silvopastoral (FSP), and to compare these with undisturbed primary native forests (UPNF) (Filip et al., 2025).

## The Results
   
   Model details and parameters presented in article are enough complete to allow a succesfull implementation.
   
   The general model form perfectly represent woody population dynamics along time.

   But, to acomodate this results, recruitment mortality must be 55% for shrubs and 14% for A.nigra wich, anyway, are guessed parameters.

   ![FOMSI_10](https://github.com/user-attachments/assets/6bfb765f-e52d-4666-8dc6-5b12c7d4f779)


   Unfortunatly, results presented in the article apparently correspond to a different model, most likely same general model form with other parameters. No published graphic or table can be reproduced.
    
 |                    |     UPNF     |       MBGI    |       FSP   |
 | --- | ---: | ---: | ---: |
 | Tree recruitment   |       3.14   |        2.91   |       0.86  | 
 | Tree density       |     108.82   |       98.36   |      11.67  |
 | Tree basal area    |       3.26   |        2.92   |       0.55  |
 | Shrub recruitment  |     611.15   |      593.47   |      13.18  | 
 | Shrub density      |    4819.10   |     3603.61   |      39.60  | 
 | Tree C storage     |   15185.61   |    13511.11   |    2483.49  |
 | Shrub C storage    |    9121.07   |     5536.45   |     143.11  |
 | Woody C storage    |   24306.67   |    19047.56   |    2626.60  |

   It's obvious that most values are clearly lower than those presented in article. 
   Most notable disagreement is in recruitment row. That perhaps it's not just a typo but some mistake in model programming by authors. Hardly yearly recruitment could be 74% of total population when higher yearly recruitment parameter is 21% of mature population.

   Finally, even with different numbers, this results support discussions and considerations presented in the article.

note: 
     Published Table 3 say "mean weigthed C in the system" where have to say "mean weigthed total woody C storage", because herbaceous layer is not accounted. That is important because you should expect higher herbaceous C storage in disturbed systems, wich at least could balance a little disturbances.   
    

## The Program

[![](https://img.shields.io/badge/Assembler-UASM%20v2.56-green.svg?style=flat-square&logo=visual-studio-code&logoColor=white&colorB=1CC887)](http://www.terraspace.co.uk/uasm.html) 

Framework : ObjAsm C.2  - masters : [Main Developer](https://github.com/ObjAsm/ObjAsm-C.2) , [Fork](https://github.com/ASMHSE/ObjAsm-C.2/tree/master)

Math syntax macros: [SmplMath](https://github.com/ASMHSE/SmplMath)

Current buildings is for Windows platform, 64 bits, and ANSI outputs.

Programming is Object Oriented, and consist of WoodyCarbon Simulator object controlling FOMSI model object runnings, and a Single Document Interface applications descendent object who run the simulations and show graphics and tables. 

## Reference

Filip I. D., Peri P. L., Fernández P., Loto D., Kees S. M., Banegas N., Nasca J., Sacido M., Faverin C., Vibart R. Modelling woody carbon dynamics in tropical dry forests: A case study of the Dry Chaco Region. Ecological Modelling. 2026. Vol. 512. p. 111401.
