; ==================================================================================================
; Title:   WSpecie.inc
; Author:  Héctor S. Enrique - January 2026
; Version: 1.0.0
; Purpose: FOMSI Model implementation 
; Notes:    Framework version C.2.0, 2026
;               - First release.
; ==================================================================================================
        regpointer textequ <xbx>
        Method @CatStr(<Woody>,%Specie).CalculateIndividual, uses xbx, turn :xword
            local MortProbN : dword, MortProbF : real8
            local fSlvTLS()
            
            SetObject xbx

            fSlv8 Age = Age + 1 ;dAge 
            
            if Specie gt 0    
                .if fLT(DBH, 25)
                    mov xax, 0
                .else    
                    fSlv CategoryDBH = down((DBH-20)/5)
                    mov xax, CategoryDBH
                    .if  xax > MaxCat
                        mov xax, MaxCat
                    .endif
                .endif
                lea xcx, GrowthRate
                fSlvW DBH += real8 ptr [xcx+xax*8]
            endif

            if Specie eq 1
                fSlv8 Height = 0.9+0.47*DBH-0.0049*DBH^2
            elseif Specie eq 2
                fSlv8 Height = -0.577+5.5*ln(DBH)
            elseif Specie eq 3
                fSlv8 Height = -0.17+2.38*ln(DBH)
            elseif Specie eq 4
                fSlv8 Height = exp(2.04-4.39/DBH)
            endif

            if Specie eq 0
                fSlv8 Aboveground = 0.74 + 0.10 * Age
            else
                fSlv8 Aboveground = exp(-2.187+0.916*ln(WoodDensity*DBH^2*Height))
                ;fSlv8 Aboveground = 0.112*(WoodDensity*DBH^2*Height)^0.916          ; Chave et al. 2005
            endif
            fSlv8 Belowground = Aboveground * BelowgroundRatio    

            fSlv8 Basalmetros = PI* [xbx].R8_DBH^2 / 40000        
            
            if Specie eq 0
                test turn, turn_harvest_shrub
                je noharvest
                .if fGE(Age, TargetAgeHarvest)
                    mov   WoodyHarvest, 1
                    ExitMethod
                .endif
            else
                test turn, turn_harvest_tree
                je noharvest
                .if fGE(DBH, DBHCut)
                    mov   WoodyHarvest, 1
                    ExitMethod
                .endif
            endif
          noharvest:
            ;if Specie gt 0
            ;    test turn, turn_stabilization
            ;    je nolimit
            ;    fSlv CategoryTarget = down((DBH-5)/10)
            ;  nolimit:    
            ;endif  
            invoke Random32, TreeBigNum
            fSlv8 MortProbF = eax /(TreeBigNum-1)
            .if fLT(MortProbF, MortalityRate)
                mov   WoodyMortality, 1
            .else
            if 1
	            if Specie gt 0
	                .if fGT(DBH, LifeSpan)
	                    mov   WoodyMortality, 1
	                .endif
	            else
	                .if fGT(Age, LifeSpan)
	                    mov   WoodyMortality, 1
	                .endif
	            endif
	            endif
            .endif
        MethodEnd
    
        Method @CatStr(<Woody>,%Specie).Init, uses xbx, _a:REAL8, _d:REAL8
            SetObject xbx
            fSlv8 Age = _a
            fSlv8 DBH = _d
        MethodEnd
