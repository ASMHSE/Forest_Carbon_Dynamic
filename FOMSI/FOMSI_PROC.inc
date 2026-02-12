; ==================================================================================================
; Title:   FOMSI_PROC.inc
; Author:  Héctor S. Enrique - January 2026
; Version: 1.0.0
; Purpose: FOMSI Model implementation 
; Notes:    Framework version C.2.0, 2026
;               - First release.
; ==================================================================================================
    .data
        TreeBigNum  dd 07FFFFFFFh

        PopulationT  xword 0
        PopulationD  xword 0
        Population1  xword 0
        Population2  xword 0
        Population3  xword 0
        Population4  xword 0
        Population5  xword 0
    .code

;----------------------------------------------------------
;   Woody
;----------------------------------------------------------
    regpointer textequ <xbx>

	Method Woody.Init, uses xbx
	    SetObject xbx
	    mov [xbx].chain.SDLL_ITEM.pPrevItem, 0
	    mov [xbx].chain.SDLL_ITEM.pNextItem, 0
	MethodEnd

    Method Woody.Decomposition, uses xbx
        local porcentualPreservation:real8
        
        SetObject xbx
        fSlvW Age += 1
        fSlv8 porcentualPreservation =   exp(-0.197)
        ;fSlvW Aboveground *= porcentualPreservation        
        fSlvW Belowground *= porcentualPreservation        
    MethodEnd
    
;----------------------------------------------------------
;   WoodyPopulation
;----------------------------------------------------------
    regpointer textequ <xdi>

    Method WoodyPopulation.Done, uses xdi
        local fregTLS()
        SetObject xdi

        SDLL_DoneH xdi,, Woody, xax

    MethodEnd

    Method WoodyPopulation.Init, uses xdi
        local fregTLS()
        SetObject xdi

        SDLL_InitH xdi,,Woody
        SDLL_CleanH xdi,,Woody, xax
    MethodEnd
    
    Method WoodyPopulation.Store, uses xdi xbx xsi, pStream:$ObjPtr(Stream)
        local fregTLS()
        
        SetObject xdi
        
        mov xsi, pStream
        OCall xsi::Stream.BinWrite64, Density
        OCall xsi::Stream.BinWriteX, MaturePopulation
        OCall xsi::Stream.BinWriteX, Population

        assume xbx:$ObjPtr(Woody)
        SDLL_BeginLoop xdi,,$Obj(Woody), xbx    
            OCall xsi::Stream.BinWrite64, [xbx].R8_Age
            OCall xsi::Stream.BinWrite64, [xbx].R8_DBH
        SDLL_EndLoop xbx
    MethodEnd

    Method WoodyPopulation.Load, uses xdi xbx xsi, pStream:$ObjPtr(Stream);, pDeserInfo:PDESER_INFO
        local i : xword
        
        SetObject xdi
        
        mov xsi, pStream
        OCall xsi::Stream.BinRead64
        ChargeR8 Density
        OCall xsi::Stream.BinReadX
        mov   MaturePopulation, xax
        OCall xsi::Stream.BinReadX
        mov   Population, xax
        DbgDec Population

        assume xbx:$ObjPtr(Woody)
        ForLpD i, 0, Population
            New   Woody
            mov   xbx, xax
            OCall xsi::Stream.BinRead64
            ChargeR8 [xbx].R8_Age
            OCall xsi::Stream.BinRead64
            ChargeR8 [xbx].R8_DBH
            SDLL_AppendH xdi,, Woody, xbx,, xcx
        Next i
    MethodEnd

;----------------------------------------------------------
;   WoodyPopulationD
;----------------------------------------------------------
    regpointer textequ <xdi>

    Method WoodyPopulationD.Decomposition, uses xdi xbx
        local suma:real8
        local fSlvTLS()
        local fregTLS()

        SetObject xdi
        assume xbx:$ObjPtr(Woody)
        ;fSlv8 AbovegrCarbon = 0
        fSlv8 BelowgrCarbon = 0
        mov Population, 0
        fSlv8 suma = 0
        mov PopulationT, 0
        SDLL_BeginLoop xdi,,$Obj(WoodyD), xbx    
            OCall xbx::Woody.Decomposition
            .if fGT([xbx].R8_Age, 25)
	            _push xbx
	            SDLL_RemoveH xdi,,Woody,xbx,, xax, xcx, clean
	            _pop xbx
	            Destroy xbx

	        .else
                ;fSlvW AbovegrCarbon += [xbx].R8_Aboveground    
                fSlvW BelowgrCarbon += [xbx].R8_Belowground
	            inc   Population
	            fSlvW suma += [xbx].R8_Age    
            .endif
            inc PopulationT
        SDLL_EndLoop xbx
        
        ;DbgFloat suma
        ;DbgDec PopulationT
        ;DbgDec Population

        ;fSlvW AbovegrCarbon *= CarbonInBiomass
        fSlvW BelowgrCarbon *= CarbonInBiomass
        .if fGT(suma,0)
            fSlv8  = suma/ Population
        .else
            fldz
        .endif
        fstp MeanAge
        ;fSlv8 TotalCarbon = AbovegrCarbon + BelowgrCarbon
    MethodEnd
    
    Method WoodyPopulationD.Store, uses xdi xbx xsi, pStream:$ObjPtr(Stream)
        local i : xword
        local fregTLS()
        
        SetObject xdi
        
        mov xsi, pStream
        
        OCall xsi::Stream.BinWriteX, Population
        assume xbx:$ObjPtr(Woody)
        SDLL_BeginLoop xdi,,$Obj(Woody), xbx    
            OCall xsi::Stream.BinWrite64, [xbx].R8_Age
            OCall xsi::Stream.BinWrite64, [xbx].R8_DBH
            ;OCall xsi::Stream.BinWrite64, [xbx].R8_Aboveground     ; not useful now
            OCall xsi::Stream.BinWrite64, [xbx].R8_Belowground
        SDLL_EndLoop xbx
    MethodEnd    

    Method WoodyPopulationD.Load, uses xdi xbx xsi, pStream:$ObjPtr(Stream);, pDeserInfo:PDESER_INFO
        local i : xword
        local fregTLS()
        
        SetObject xdi
        mov   xsi, pStream
        OCall xsi::Stream.BinReadX
        mov   Population, xax
        DbgDec Population
        assume xbx:$ObjPtr(Woody)
        ForLpD i, 0, Population
            ;DbgDec i 
            New   Woody
            mov   xbx, xax
            OCall xsi::Stream.BinRead64
            ChargeR8 [xbx].R8_Age
            OCall xsi::Stream.BinRead64
            ChargeR8 [xbx].R8_DBH
            ;OCall xsi::Stream.BinRead64                    ; not useful now
            ;ChargeR8 [xbx].R8_Aboveground
            OCall xsi::Stream.BinRead64
            ChargeR8 [xbx].R8_Belowground

            SDLL_AppendH xdi,,Woody, xbx,, xcx
        Next i
    MethodEnd

    Method WoodyPopulation.Clean, uses xdi xbx
        local fSlvTLS()
        local fregTLS()

        SetObject xdi

        SDLL_CleanH xdi,,Woody, xax
    MethodEnd
;----------------------------------------------------------
;   Woody and WoodyPopulation 0-4
;----------------------------------------------------------
 
    Specie = 0
    repeat 5
   
        include WSpecie.inc
        include WPopulation.inc
 
        Specie = Specie + 1
    endm
        
;----------------------------------------------------------
;   FOMSI
;----------------------------------------------------------
    Method FOMSI.Init, uses xsi, pSim:POINTER
        SetObject xsi
        
        m2m [xsi].pSimulator, pSim, xax

        OCall [xsi].DeadWood::WoodyPopulationD.Init
        Specie = 0
        repeat 5
            OCall [xsi].@CatStr(<WP>,%Specie)::@CatStr(<WoodyPopulation>,%Specie).Init
            Specie = Specie + 1
        endm

        SDLL_InitH xsi,,FOMSIResult

    MethodEnd    

    Method FOMSI.Initial, uses xsi xbx xdi, pShrubland:POINTER, genpop:XWORD
        local fregTLS()
        
        SetObject xsi
        regpointer textequ <xsi>

        assume xax: ptr Shrubland
            mov xax, pShrubland
            fSlv8 Area = [xax].Superficie
        assume xax: nothing
        .if genpop == TRUE
	        Specie = 0
	        repeat 5    
	            OCall [xsi].@CatStr(<WP>,%Specie)::@CatStr(<WoodyPopulation>,%Specie).Initial, pShrubland
	            Specie = Specie + 1
	        endm
        .endif        
        ;DbgObject xsi::FOMSI

        ;DbgObject [xsi].WP1::WoodyPopulation1  

    MethodEnd

    regpointer textequ <xdi>
    Method FOMSI.CalculateModel, uses xsi xdi, turn :xword

        SetObject xsi
        
        OCall [xsi].DeadWood::WoodyPopulationD.Decomposition
        lea xdi, [xsi].DeadWood
        assume xdi:$ObjPtr(WoodyPopulationD)
        ;fSlv8 [xsi].R8_DeadwoodCarbon = TotalCarbon / [xsi].R8_Area  ; not useful now
        fSlv8 [xsi].R8_DeadCarbon = BelowgrCarbon / [xsi].R8_Area        
        fSlv8 [xsi].R8_Stumps = Population/ [xsi].R8_Area
        fSlv8 [xsi].R8_DeadMeanAge = MeanAge
        
        fSlv8 [xsi].R8_AbovegroundWoodyCarbon = 0
        fSlv8 [xsi].R8_BelowgroundWoodyCarbon = 0
        fSlv8 [xsi].R8_TimberProduction = 0
        fSlv8 [xsi].R8_TreeDensity = 0
        fSlv8 [xsi].R8_TreeRecruitment = 0
        fSlv8 [xsi].R8_TotalBasalArea = 0
        fSlv8 [xsi].R8_Mortality = 0
        fSlv8 [xsi].R8_TreeRecruitment2Pop = 0
        fSlv8 [xsi].R8_AbovegroundTreeCarbon = 0
        fSlv8 [xsi].R8_BelowgroundTreeCarbon = 0
        
        Specie = 0
        repeat 5
            OCall @CatStr(<[xsi].WP>,%Specie)::@CatStr(<WoodyPopulation>,%Specie).CalculatePopulation, turn
            lea xdi, @CatStr(<[xsi].WP>,%Specie)
            assume xdi:$ObjPtr(@CatStr(<WoodyPopulation>,%Specie))
                if Specie eq 0
                    fSlvW [xsi].R8_AbovegroundWoodyCarbon += AbovegrCarbon*ShrubResScale
                    fSlvW [xsi].R8_BelowgroundWoodyCarbon += BelowgrCarbon*ShrubResScale
                    fSlv8 [xsi].R8_ShrubDensity = Density*ShrubResScale
                    fSlv8 [xsi].R8_ShrubRecruitment = Recruitment*ShrubResScale
                    fSlv8 [xsi].R8_ShrubRecruitment2Pop = Recruit2PopFlow*ShrubResScale
                    fSlv8 [xsi].R8_AbovegroundShrubCarbon = AbovegrCarbon*ShrubResScale
                    fSlv8 [xsi].R8_BelowgroundShrubCarbon = BelowgrCarbon*ShrubResScale
                    fSlv8 [xsi].R8_MeanShrubAge = MeanAge*ShrubResScale
                    fSlv8 [xsi].R8_CalcShrubAge = CalcAge*ShrubResScale
                else    
                    fSlvW [xsi].R8_AbovegroundWoodyCarbon += AbovegrCarbon
                    fSlvW [xsi].R8_BelowgroundWoodyCarbon += BelowgrCarbon
                    fSlvW [xsi].R8_TimberProduction += Timber
                    fSlvW [xsi].R8_TreeDensity += Density
                    fSlvW [xsi].R8_TotalBasalArea += BasalArea
                    fSlvW [xsi].R8_TreeRecruitment += Recruitment
                    fSlvW [xsi].R8_TreeRecruitment2Pop += Recruit2PopFlow
                    fSlvW [xsi].R8_AbovegroundTreeCarbon += AbovegrCarbon
                    fSlvW [xsi].R8_BelowgroundTreeCarbon += BelowgrCarbon
                endif
                if Specie eq 1
                    fSlv8 [xsi].R8_QuebrachoBlanco = Density
                    fSlv8 [xsi].R8_QBlancoH = MeanHeight
                elseif Specie eq 2
                    fSlv8 [xsi].R8_QuebrachoColorado = Density
                    fSlv8 [xsi].R8_QColoradoH = MeanHeight
                elseif Specie eq 3
                    fSlv8 [xsi].R8_Mistol = Density
                    fSlv8 [xsi].R8_MistolH = MeanHeight
                elseif Specie eq 4
                    fSlv8 [xsi].R8_Algarrobo = Density
                    fSlv8 [xsi].R8_AlgarroboH = MeanHeight
                endif    
                fSlvW [xsi].R8_Mortality += [xdi].XD_MortalityFlow
            assume xdi:nothing
            Specie = Specie + 1
        endm

        fSlvW [xsi].R8_AbovegroundWoodyCarbon /= [xsi].R8_Area
        fSlvW [xsi].R8_BelowgroundWoodyCarbon /= [xsi].R8_Area
        fSlvW [xsi].R8_TimberProduction /= [xsi].R8_Area
        fSlvW [xsi].R8_TotalBasalArea /= [xsi].R8_Area
        fSlvW [xsi].R8_ShrubRecruitment /= [xsi].R8_Area
        fSlvW [xsi].R8_TreeRecruitment /= [xsi].R8_Area
        fSlvW [xsi].R8_ShrubRecruitment2Pop /= [xsi].R8_Area
        fSlvW [xsi].R8_TreeRecruitment2Pop /= [xsi].R8_Area
        
        fSlvW [xsi].R8_AbovegroundShrubCarbon /= [xsi].R8_Area  ; in Mg C / ha 
        fSlvW [xsi].R8_BelowgroundShrubCarbon /= [xsi].R8_Area
        fSlvW [xsi].R8_AbovegroundTreeCarbon /= [xsi].R8_Area
        fSlvW [xsi].R8_BelowgroundTreeCarbon /= [xsi].R8_Area

    MethodEnd

    Method FOMSI.StoreModel, uses xsi xdi, year:XWORD
        local i : xword, fyear:REAL8 
        local buffer[1024] : CHR   
        
        regpointer textequ <xsi>

        SetObject xsi
        
        fSlv8 Year = year
        
        New FOMSIResult
        push xsi
        cld
        lea xdi, [xax].$Obj(FOMSIResult).R8_Year
        lea xsi, [xsi].R8_Year
        mov xcx , results_size*8                 ; Size
        rep movsb
        pop xsi
        
        assume xax:$ObjPtr(FOMSIResult)
        regpointer textequ <xax>
            fSlv8 BelowgroundTreeCarbonFull = BelowgroundTreeCarbon + DeadCarbon  
            fSlv8 ShrubDensityScaled = ShrubDensity / 10
            fSlv8 TreeRecruitmentScaled = TreeRecruitment * 10
            fSlv8 TreeRecruitment2PopScaled = TreeRecruitment2PopScaled * 10
            fSlv8 TreeCstorage = AbovegroundTreeCarbon + BelowgroundTreeCarbonFull
            fSlv8 ShrubCstorage = AbovegroundShrubCarbon + BelowgroundShrubCarbon
            fSlv8 WoodyCstorage = TreeCstorage + ShrubCstorage
            fSlv8 ShrubRecruitmentScaled = ShrubRecruitment / 2
        assume xax: nothing 
        
        SDLL_AppendH xsi,,FOMSIResult,xax,, xcx
    MethodEnd


    Method FOMSI.InstallFSP, uses xsi xbx
        local fregTLS()
        local i:xword
                
        SetObject xsi

        Specie = 2
        repeat 3
            OCall [xsi].@CatStr(<WP>,%Specie)::@CatStr(<WoodyPopulation>,%Specie).Clean
            Specie = Specie + 1
        endm
        OCall [xsi].WP1::WoodyPopulation1.Select, 20

        lea xcx, [xsi].WP1
        assume xcx:$ObjPtr(WoodyPopulation1)       
            lea xbx, [xcx].R8_RecruitmentStage
            ForLp i, 1, [xcx].XD_nRecruitmentStage
                fSlv8 real8 ptr [xbx+xax*8] = 0
            Next i
        assume xcx:nothing
    MethodEnd
    
    Method FOMSI.Clean, uses xsi
        local fregTLS()
                
        SetObject xsi
        
        OCall [xsi].DeadWood::WoodyPopulationD.Clean
        Specie = 0
        repeat 5
            OCall [xsi].@CatStr(<WP>,%Specie)::@CatStr(<WoodyPopulation>,%Specie).Clean
            Specie = Specie + 1
        endm

        SDLL_CleanH xsi,, FOMSIResult, xax
        assume _ecx:nothing
    MethodEnd
    
    Method FOMSI.Done, uses xsi
        local fregTLS()
                
        SetObject xsi
        
        OCall [xsi].DeadWood::WoodyPopulationD.Done
        Specie = 0
        repeat 5
            OCall [xsi].@CatStr(<WP>,%Specie)::@CatStr(<WoodyPopulation>,%Specie).Done
            Specie = Specie + 1
        endm
        SDLL_DoneH xsi,, FOMSIResult, xax
    MethodEnd

    Method FOMSI.ReadPopulations, uses xsi xdi, pFile:POINTER
        SetObject xsi

        mov xdi, offset $ObjTmpl(DiskStream)
        OCall xdi::DiskStream.Init, NULL, pFile,0,0,0,0,0,0
        lea  xax, [xsi].R8_Year
        OCall xdi::Stream.BinRead,xax, results_size*8
        DbgDec eax

        OCall [xsi].DeadWood::WoodyPopulationD.Load, xdi
        Specie = 0
        repeat 5
            OCall [xsi].@CatStr(<WP>,%Specie)::@CatStr(<WoodyPopulation>,%Specie).Load, xdi
            Specie = Specie + 1
        endm
        DbgText "-FOMSI.ReadPopulations"
        OCall xdi::DiskStream.Done

    MethodEnd
    Method FOMSI.SavePopulations, uses xsi xdi, pFile:POINTER
        SetObject xsi
    
        mov xdi, offset $ObjTmpl(DiskStream)
        OCall xdi::DiskStream.Init, NULL, pFile,0,0,0,0,0,0
        
        lea  xax, [xsi].R8_Year
        OCall xdi::Stream.BinWrite,xax, results_size*8

        OCall [xsi].DeadWood::WoodyPopulationD.Store, xdi
        Specie = 0
        repeat 5
            OCall [xsi].@CatStr(<WP>,%Specie)::@CatStr(<WoodyPopulation>,%Specie).Store, xdi
            Specie = Specie + 1
        endm
        OCall xdi::DiskStream.Truncate
        OCall xdi::DiskStream.Done
       
    MethodEnd
    