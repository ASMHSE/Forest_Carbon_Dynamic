# Forest_Carbon_Dynamic
SmplSimWoody 1.0. A Light FOMSI Simulator

## The Model

FOMSI (FOrest Model SImulator) is a deterministic model to simulate forest population
dynamics and predict woody carbon stocks and sequestration rates for
different silvopastoral systems. The initial conditions (e.g. recruitment
rate) and management strategies during the simulation sequence
determined the outputs. The only stochastic component in the model
was plant mortality rate. The model comprises the assemblage of three
general model components: a population, a production, and a management
model. The first component comprises biophysical processes,
represented by trees and shrubs recruitment, growth, and
mortality (population model). The second component represents a forest
exploitation based on population (production model). The third
component of the model includes processes regulated by human intervention
(management model). (Filip et al., 2025)

## The Results

   The general model form perfectly represent woody population dynamics along time.

   But, to acomodate this results, recruitment mortality must be 55% for shrubs wich, anyway, it's a guessed parameter.

   graphic

   Unfortunatly, results presented in the article apparently correspond to a different model, most likely same general model form with other parameters. No published graphic or table can be reproduced.
    
 |                   |     UPNF   |       MBGI     |      FSP   |
 | --- | ---: | ---: | ---: |
 | Tree recruitment   |       3.14   |        2.91   |       0.83  | 
 | Tree density       |     104.39   |       94.45   |      11.63  |
 | Tree basal area    |       3.07   |        2.75   |       0.53  |
 | Shrub recruitment  |     613.94   |      589.53   |      13.06  | 
 | Shrub density      |    4796.42   |     3553.13   |      38.11  | 
 | Tree C storage     |   14450.80   |    12868.67   |    2401.46  |
 | Shrub C storage    |    9051.48   |     5463.02   |     142.66  |
 | Woody C storage    |   23502.27   |    18331.68   |    2544.12  |

   It's obvious that most values are clearly lower than those presented in article. 
   Most notable disagreement is in recruitment row. That perhaps it's not just a typo but some mistake in model programming by authors. Hardly yearly recruitment could be 74% of total population when higher yearly recruitment parameter is 21% of mature population.

note: 
     Published Table 3 say "mean weigthed C in the system" where have to say "mean weigthed total woody C storage", because herbaceous layer is not accounted. That is important because you should expect higher herbaceus C storage in disturbed systems, wich at least could balance a little disturbance.   
    

## The Program

[![](https://img.shields.io/badge/Assembler-UASM%20v2.56-green.svg?style=flat-square&logo=visual-studio-code&logoColor=white&colorB=1CC887)](http://www.terraspace.co.uk/uasm.html) 

Framework : ObjAsm C.2  - masters : [Main Developer](https://github.com/ObjAsm/ObjAsm-C.2) , [Fork](https://github.com/ASMHSE/ObjAsm-C.2/tree/master)

Math syntax macros: [SmplMath](https://github.com/ASMHSE/SmplMath)

Current buildings is for Windows platform, 64 bits, and ANSI outputs.

Programming is Object Oriented, and consist of WoodyCarbon Simulator object controlling FOMSI model object runnings, and a Single Document Interface applications descendent object who run the simulations and show graphics and tables. 

## Reference

Filip I. D., Peri P. L., Fernández P., Loto D., Kees S. M., Banegas N., Nasca J., Sacido M., Faverin C., Vibart R. Modelling woody carbon dynamics in tropical dry forests: A case study of the Dry Chaco Region. Ecological Modelling. 2026. Vol. 512. p. 111401.
