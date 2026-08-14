; ==================================================================================================
; Title:   WPopulation.inc
; Author:  Héctor S. Enrique - January 2026
; Version: 1.0.0
; Purpose: FOMSI Model implementation 
; Notes:    Framework version C.2.0, 2026
;               - First release.
; ==================================================================================================
        regpointer textequ <xdi>

        Method @CatStr(<WoodyPopulation>,%Specie).Init, uses xdi xbx
            local fregTLS()
            
            DbgText "specie Init"
            SetObject xdi
            
            SDLL_InitH xdi,,@CatStr(<Woody>,%Specie)
        MethodEnd    
    
        Method @CatStr(<WoodyPopulation>,%Specie).Initial, uses xdi xbx, pShrubland:POINTER
            local i : dword, j:dword, k:dword
            local _Age:real8,_DBH:real8
            local fregTLS()
            
            SetObject xdi
            
            assume xsi:$ObjPtr(FOMSI)
            
            assume _ecx: ptr Shrubland

            fSlv8 InitialPopulation = 0

            if Specie gt 0
                
                ForLpD i, 0, 6, ebx
                    mov _ecx, pShrubland
                    %lea xax, @CatStr(<[_ecx].specie>,%Specie,<_num>) 
                    fSlv k = real8 ptr [xax+xbx*8]*[xsi].R8_Area
                    %lea xax, @CatStr(<[_ecx].specie>,%Specie,<_age>) 
                    fSlv8 _Age = real8 ptr [xax+xbx*8]
                    %lea xax, @CatStr(<[_ecx].specie>,%Specie,<_dbh>) 
                    fSlv8 _DBH = real8 ptr [xax+xbx*8]
                    ForLpD j, 0, k, ecx
                        New @CatStr(<Woody>,%Specie)
                        _push xax
                        OCall xax::@CatStr(<Woody>,%Specie).Init, _Age, _DBH
                        _pop xax
                        SDLL_AppendH xdi,, @CatStr(<Woody>,%Specie), xax,, xcx
                    Next j
                    ;lea xax, ReductionLimit                    ; for fast stabilization
                    ;m2m XWORD ptr [xax+xbx*XWORD], k, xcx
                    ;.if k > 0
                    ;    mov ReductionCats, xbx 
                    ;.endif
                    fSlvW InitialPopulation += k
                Next i
            else
                mov _ecx, pShrubland
                fSlv k = @CatStr(<[_ecx].specie>,%Specie,<_num>)*[xsi].R8_Area
                ForLpD j, 0, k, ecx
                    New @CatStr(<Woody>,%Specie)
                    _push xax
                    OCall xax::@CatStr(<Woody>,%Specie).Init, FP8(1.0), FP8(0.0)
                    _pop xax
                    SDLL_AppendH xdi,, @CatStr(<Woody>,%Specie), xax,, xcx
                Next j
                fSlvW InitialPopulation = k
            endif
            fSlv8 Density = InitialPopulation / [xsi].R8_Area
            fSlv8 Population = InitialPopulation
            fSlv8 PopulationCheck = InitialPopulation
            assume _ecx:nothing 
            assume xsi:nothing             

            @CatStr(<DbgText !" WoodyPopulation>,%Specie,< initialized !">)
            

        MethodEnd

            MoveToDeadWood macro
                _push xbx
                SDLL_RemoveH xdi,,@CatStr(<Woody>,%Specie),xbx,, xax, xcx, clean
                _pop xbx
                if Specie eq 0
                    Destroy xbx
                else
                    fSlv8 [xbx].R8_Age = 0
                    fSlv8 [xbx].R8_Aboveground = 0  ; fire is assumed
                    assume xdx:$ObjPtr(WoodyPopulationD)
                    lea   xdx, [xsi].DeadWood
                    inc   [xdx].XD_Population
                    if Specie gt 0
                        inc @CatStr(<Population>, %Specie)
                    endif
                    mov   xax, xbx
                    SDLL_AppendH xdx,, Woody, xax,, xcx
                    assume xdx:nothing
                endif    
            endm
            
        Method @CatStr(<WoodyPopulation>,%Specie).CalculatePopulation, uses xdi, turn :xword
            local i : XWORD
            local NetFlow : SXWORD
            local _Age : real8 , _DBH : real8, SumAge : real8, Total : xword
            local CatBasalTotal[25*8] : real8 
            local fSlvTLS()
            local fregTLS(,72)
            
            SetObject xdi
            assume xsi:$ObjPtr(FOMSI)
            assume xbx:$ObjPtr(%@CatStr(<Woody>,%Specie))

            ;----------------------------------------
            ; Reseting Population Variables            
            ;----------------------------------------
            mov HarvestFlow, 0
            mov MortalityFlow, 0
            if Specie eq 0
                mov SproutFlow, 0
            endif
            .if fLT(Density,TargetDensity)
                fSlv8 RecruitmentRate = RecruitmentRateMax/100
            .else
                fSlv8 RecruitmentRate = RecruitmentRateMin/100
            .endif                 
            
            fSlv8 = MaturePopulation*RecruitmentRate
            fst RecruitmentStage
            fstp Recruitment
            
            fSlv8 AbovegrCarbon = 0
            fSlv8 BelowgrCarbon = 0
            if Specie gt 0
                fSlv8 BasalArea = 0
                fSlv8 MeanHeight = 0
                fSlv8 MaxAge = 0 
            endif
            mov   Population, 0
            mov   MaturePopulation, 0
            mov   Total, 0
            fSlv8 SumAge = 0
            ;----------------------------------------
            ; Processing Population            
            ;----------------------------------------
            SDLL_BeginLoop xdi,,$Obj(@CatStr(<Woody>,%Specie)), xbx    
                OCall xbx::@CatStr(<Woody>,%Specie).CalculateIndividual, turn
                
                .if [xbx].XD_WoodyMortality == 0 && [xbx].XD_WoodyHarvest == 0
                    inc  Population
                    if Specie gt 0                    
                        .if fGT([xbx].R8_DBH, [xbx].R8_DBHMaturity)
                            inc   MaturePopulation
                        .endif
                    else
                        .if fGT([xbx].R8_Age, [xbx].R8_AgeMaturity)
                            inc   MaturePopulation
                        .endif
                    endif    
                .endif
                .if [xbx].XD_WoodyMortality == 1
                    inc   MortalityFlow
                .endif
                
                .if [xbx].XD_WoodyHarvest == 1
                    inc   HarvestFlow
                .endif
                    
                fSlvW AbovegrCarbon += [xbx].R8_Aboveground
                fSlvW BelowgrCarbon += [xbx].R8_Belowground
                fSlvW SumAge += [xbx].R8_Age                    ; to test a problem with shrub carbon storage
                    
                if Specie gt 0             
                    fSlvW BasalArea += PI*([xbx].R8_DBH^2)/40000
                    ;.if fGT([xbx].R8_Height,0)
	                    fSlvW MeanHeight += [xbx].R8_Height
		                inc Total
	                ;.endif
	                .if fGT([xbx].R8_Age, MaxAge)
	                    fSlv8 MaxAge = [xbx].R8_Age 
	                .endif
                endif
                
            SDLL_EndLoop xbx
            
            fSlvW AbovegrCarbon *= CarbonInBiomass
            fSlvW BelowgrCarbon *= CarbonInBiomass
            
            if Specie gt 0
                .if Total > 0
                    fSlvW MeanHeight /= Total
                .else
                    fSlv8 MeanHeight = 0
                .endif
            
                ;----------------------------------------
                ; Uneven age management
                ;----------------------------------------
                test turn, turn_sel_harvest_tree
                je nobrassiolo
                ;----------------------------------------
                ; Total Basal Area by Category (5cm)
                ;----------------------------------------
                lea xcx, CatBasalTotal
                ForLpD i, 0, 25
                    fSlv8 real8 ptr [xcx+xax*8] = 0 
                Next i
                SDLL_BeginLoop xdi,,$Obj(@CatStr(<Woody>,%Specie)), xbx
                    mov   xax, [xbx].XD_CategoryDBH
                    .if xax > 3
                        sub xax, 3
                        fSlvW real8 ptr [xcx+xax*8] += [xbx].R8_Basalmetros 
                    .endif
                SDLL_EndLoop xbx
                ;----------------------------------------
                ; Available Basal Area by Category (5cm)
                ;----------------------------------------
                ForLpD i, 0, 25
                    fSlvW real8 ptr [xcx+xax*8] *= 0.3
                Next i
                SDLL_BeginLoop xdi,,$Obj(@CatStr(<Woody>,%Specie)), xbx
                    mov   xax, [xbx].XD_CategoryDBH
                    .if xax > 3
                        sub xax, 3
                        .if fGE(real8 ptr [xcx+xax*8], [xbx].R8_Basalmetros)
                            fSlvW real8 ptr [xcx+xax*8] -= [xbx].R8_Basalmetros
                            mov  [xbx].XD_WoodyHarvest, 1
                        .endif 
                    .endif
                SDLL_EndLoop xbx
    
                nobrassiolo:
            else
                ;----------------------------------------
                ; Low Impact shrub
                ;----------------------------------------
                test turn, turn_sel_harvest_shrub
                je nobrassiolo
                ;----------------------------------------
                ; Available Shrub number
                ;----------------------------------------
                fSlv8 CatBasalTotal = Population * 0.3
                SDLL_BeginLoop xdi,,$Obj(@CatStr(<Woody>,%Specie)), xbx
                    .if fGT(CatBasalTotal, 0)
                        .if fGE([xbx].R8_Age, [xbx].R8_TargetAgeHarvest)
                            fSlvW CatBasalTotal -= 1
                            mov  [xbx].XD_WoodyHarvest, 1
                        .endif
                    .endif 
                SDLL_EndLoop xbx
    
                nobrassiolo:    

                ;----------------------------------------
                ; Full Impact/FSP shrub
                ;----------------------------------------
                test turn, turn_harvest_shrub
                je noremotion
    
                SDLL_BeginLoop xdi,,$Obj(@CatStr(<Woody>,%Specie)), xbx
                    .if fGE([xbx].R8_Age, [xbx].R8_TargetAgeHarvest)
                        mov  [xbx].XD_WoodyHarvest, 1
                    .endif
                SDLL_EndLoop xbx
    
                noremotion:
            endif  
            
            ;----------------------------------------
            ; Cleaning Population
            ;----------------------------------------
            if Specie gt 0
                fSlv8 Timber = 0
            endif
            if Specie gt 0
            mov @CatStr(<Population>, %Specie),0
            endif
            SDLL_BeginLoop xdi,,$Obj(@CatStr(<Woody>,%Specie)), xbx

                .if [xbx].XD_WoodyMortality == 1
                    MoveToDeadWood
                    jmp @ready
                .endif
                
                .if [xbx].XD_WoodyHarvest == 1
                    if Specie eq 0
                        .if [xbx].XD_WoodySprout == 0   ; sprout only one time
                            mov   xax, SproutFlow
                            .if xax < TargetSprouting
                                inc   SproutFlow
	                            fSlv8 [xbx].R8_Age = 3;
	                            inc [xbx].XD_WoodySprout
	                            mov [xbx].XD_WoodyHarvest, 0
	                            jmp @ready
                            .endif
                        .endif
                    else
                        fSlvW Timber += [xbx].R8_Aboveground
                    endif
                    MoveToDeadWood
		        .endif
              @ready:
            SDLL_EndLoop xbx
            
            ;----------------------------------------
            ; Check Population Dynamic
            ;----------------------------------------
            if Specie gt 0
                fSlv NetFlow = Recruit2PopFlow - MortalityFlow - HarvestFlow
            else
                fSlv NetFlow = Recruit2PopFlow - MortalityFlow - HarvestFlow + SproutFlow
            endif
            fSlvW PopulationCheck += NetFlow
            
            ;DbgDec Population
            ;DbgDec PopulationCheck
            ;DbgWriteF " Population ¦UX and checck ¦UX", Population, PopulationCheck
            ;----------------------------------------
            ; Population Calculation
            ;----------------------------------------
            
            lea xcx, RecruitmentStage
            mov xax, nRecruitmentStage
            fSlv Recruit2PopFlow = round (real8 ptr [xcx+xax*8])
            ForLp i, 1, nRecruitmentStage
                fSlv8 real8 ptr [xcx+xax*8] = real8 ptr [xcx+xax*8-8]*(1.0-RecruitmentMort)
            Next i
            
            test turn, turn_FSP
            jne FSPisactive
            fSlv8 _Age = nRecruitmentStage
            fSlv8 _DBH = 5.1
            ForLpD i, 0, Recruit2PopFlow
                New @CatStr(<Woody>,%Specie)
                _push xax
                OCall xax::@CatStr(<Woody>,%Specie).Init, _Age, _DBH
                _pop xax
                SDLL_AppendH xdi,, @CatStr(<Woody>,%Specie), xax,, xcx
                inc   Population
            Next i           
            FSPisactive:
            
            fSlv8 Density = Population / [xsi].R8_Area
            
            if Specie eq 0
                fSlv8 MeanAge = SumAge / Population / ShrubResScale
                fSlv8 CalcAge = (AbovegrCarbon/Population/CarbonInBiomass-0.74)/0.1/ ShrubResScale
            else
                .if fGT(Population,0.0)    
                    fSlv8  = SumAge / Population
                .else
                    fldz
                .endif
                fstp MeanAge
            endif

            assume xbx: nothing 
            assume xsi: nothing 
            if Specie gt 0
            ;     DbgDec @CatStr(<Population>, %Specie)
            endif             
        MethodEnd

	   Method @CatStr(<WoodyPopulation>,%Specie).Clean, uses xdi xbx
	        local i : xword
	        
	        SetObject xdi
	        fSlv8 Density = 0
	        mov Population, 0
	        mov MaturePopulation, 0
            lea xbx, RecruitmentStage
            ForLp i, 0, nRecruitmentStage
                fSlv8 real8 ptr [xbx+xax*8] = 0
            Next i
	        ACall xdi.Clean
	        
        MethodEnd    
        
 	    Method @CatStr(<WoodyPopulation>,%Specie).Select, uses xdi xbx, wanted:xword
	        local RejectionProbF : real8, RejectionRate : real8
	        local mature : xword
	        local fSlvTLS()
	        local fregTLS()
	        
	        SetObject xdi

            assume xsi:$ObjPtr(FOMSI)
	        fSlv4 wanted = wanted * [xsi].R8_Area
	        mov xax, Population
	        .if wanted < xax
	            ;----------------------------------------
	            ; Count mature individuals Calculation
	            ;----------------------------------------
	            assume xbx:$ObjPtr(@CatStr(<Woody>,%Specie))
	            mov mature, 0
	            SDLL_BeginLoop xdi,,$Obj(@CatStr(<Woody>,%Specie)), xbx
	                if Specie gt 0
	                    .if fGT([xbx].R8_DBH, [xbx].R8_DBHMaturity)
                    else
	                    .if fGT([xbx].R8_Age, [xbx].R8_AgeMaturity)
                    endif
	                        inc   mature
	                    .endif
	            SDLL_EndLoop xbx
	            
	            mov xax, mature
	            .if wanted < xax
		            ;----------------------------------------
		            ; Randomly select mature individuals
		            ;----------------------------------------
                    fSlv8 RejectionRate = wanted/mature
		            SDLL_BeginLoop xdi,,$Obj(@CatStr(<Woody>,%Specie)), xbx
	                if Specie gt 0
	                    .if fGT([xbx].R8_DBH, [xbx].R8_DBHMaturity)
                    else
	                    .if fGT([xbx].R8_Age, [xbx].R8_AgeMaturity)
                    endif
                            invoke Random32, TreeBigNum
                            fSlv8 RejectionProbF = eax /(TreeBigNum-1)
				            .if fLT(RejectionProbF, RejectionRate)
                                MoveToDeadWood
                            .endif
                        .else        
                            MoveToDeadWood
	                    .endif
		            SDLL_EndLoop xbx
		         .else   
		            ;---------------------------------------------------------
		            ; Select all mature individuals and randomly reject others
		            ;---------------------------------------------------------
                    fSlv8 RejectionRate =  (wanted-mature)/(Population-mature)    	                
		            SDLL_BeginLoop xdi,,$Obj(@CatStr(<Woody>,%Specie)), xbx
	                if Specie gt 0
	                    .if fLE([xbx].R8_DBH, [xbx].R8_DBHMaturity)
                    else
	                    .if fLE([xbx].R8_Age, [xbx].R8_AgeMaturity)
                    endif
	                        invoke Random32, TreeBigNum
	                        fSlv8 RejectionProbF = eax /(TreeBigNum-1)
				            .if fGE(RejectionProbF, RejectionRate)
	                            MoveToDeadWood
	                        .endif
	                    .endif        
		            SDLL_EndLoop xbx
	            .endif
	            assume xbx:nothing
            .endif
	        
        MethodEnd    

	   Method @CatStr(<WoodyPopulation>,%Specie).Store, uses xdi xbx xsi, pStream:$ObjPtr(Stream)
	        local i : xword
	        
	        SetObject xdi
	        mov xsi, pStream
            lea xbx, RecruitmentStage
            ForLp i, 0, nRecruitmentStage, _ecx
                OCall xsi::Stream.BinWrite64, real8 ptr [xbx+_ecx*8]
                mov xax, i
            Next i
	        ACall xdi.Store, xsi
        MethodEnd    
 
	    Method @CatStr(<WoodyPopulation>,%Specie).Load, uses xdi xbx xsi, pStream:$ObjPtr(Stream);, pDeserInfo:PDESER_INFO
	        local i : xword
	        local fregTLS()
	        
	        SetObject xdi
	        
	        ;lea   xax, RecruitmentStage
	        ;DbgHex xax
	        ;lea   xax, RecruitmentMort
	       ; DbgHex xax
	        mov   xsi, pStream
            ; ---------------------------------------------------------  Special
            ;DbgLine "load" 
            lea xbx, RecruitmentStage
            ForLp i, 0, nRecruitmentStage 
                OCall xsi::Stream.BinRead64
                mov xcx, i
                ChargeR8 real8 ptr [xbx+xcx*8]  ; no in 32 bits
            ;    DbgFloat real8 ptr [xbx+xcx*8],, "load"
            Next i
            ; ---------------------------------------------------------  General
	        DbgLine
	        OCall xsi::Stream.BinRead64
	        ChargeR8 Density
            DbgFloat Density
	        OCall xsi::Stream.BinReadX
	        mov   MaturePopulation, xax
            DbgDec PopulationCheck
	        OCall xsi::Stream.BinReadX
	        mov   Population, xax
            DbgDec Population
	        mov   PopulationCheck, xax

	        assume xbx:$ObjPtr(@CatStr(<Woody>,%Specie))
	        ForLpD i, 0, Population
	        ;    DbgDec i
	            New   @CatStr(<Woody>,%Specie)
	            mov   xbx, xax
	            OCall xsi::Stream.BinRead64
	            ChargeR8 [xbx].R8_Age
	            OCall xsi::Stream.BinRead64
	            ChargeR8 [xbx].R8_DBH
	            SDLL_AppendH xdi,, @CatStr(<Woody>,%Specie), xbx,, xcx
	        Next i


	    MethodEnd