; ==================================================================================================
; Title:   FOMSI_OOP.inc
; Author:  Héctor S. Enrique - January 2026
; Version: 1.0.0
; Purpose: FOMSI Model implementation 
; Notes:    Framework version C.2.0, 2026
;               - First release.
; ==================================================================================================
	if 1                                    ; for fast calculations
	    ShrubConst  textequ <47.0>
	    iShrubConst textequ <47>
	    fSlvRegConst ShrubResScale, 100.0
	else                                    ; for exact calculations 
	    ShrubConst  textequ <4700.0>        ; hardly you can see any diference
	    iShrubConst textequ <4700>
	    fSlvRegConst ShrubResScale, 1.0
	endif      

    Shrubland struct
        Superficie   real8   0.0
        specie0_num  real8   0.0
        specie0_tspr real8   0.0
        specie0_nspr xword   0
        Specie = 1
        repeat 4 
            %@CatStr(<specie>,%Specie,<_num>) real8   6 dup(0.0)
            %@CatStr(<specie>,%Specie,<_dbh>) real8   6 dup(0.0)      
            %@CatStr(<specie>,%Specie,<_age>) real8   6 dup(0.0)
            Specie = Specie + 1
        endm 
    Shrubland ends

    ClassesVar LifeSpan,           40.0 , 60.0  , 60.0  , 50.0  , 40.0 
    ClassesVar nRecruitmentStage,   1   ,  8    ,  8    ,  4    ,  4  
    ClassesVar RecruitmentRateMin, 10.0 ,  5.0  ,  2.0  ,  2.0  ,  2.0 
    ClassesVar RecruitmentRateMax, 21.0 , 15.0  , 11.0  , 20.0  , 11.0 
;    ClassesVar RecruitmentMort,     0.3 ,  0.08 ,  0.08 ,  0.16 ,  0.16 ; not reasonable results with this numbers 
    ClassesVar RecruitmentMort,     0.55 ,  0.08 ,  0.08 ,  0.16 ,  0.14  ; perfect  
  
    ClassesVar MortalityRate,       0.05,  0.013,  0.014,  0.013,  0.018
    ClassesVar WoodDensity,         --- ,  0.9  ,  1.2  ,  0.9  ,  0.8

    ClassesVar TargetDensity,     ShrubConst, 26.0  , 18.0  , 32.0  , 32.0
        
    Object Woody,, Primer
            RedefineMethod Init
            VirtualMethod  Decomposition
            SENTINEL_I
            regpointer textequ <xbx>
            estado      Age
            estado      DBH
            DefinemR8   Aboveground
            DefinemXD   WoodyMortality              
            DefinemXD   WoodyHarvest
            ;DefinemXD   WoodyReduction              ; for Population Stabilization 
            DefinemR8   Belowground
            DefinemR8   Basalmetros
    ObjectEnd    

    Object WoodyPopulation,, Streamable
            RedefineMethod Init
            VirtualMethod  Clean
            RedefineMethod Done

            RedefineMethod Load, $ObjPtr(Stream);, PDESER_INFO
            RedefineMethod Store, $ObjPtr(Stream)       ;-> Stream

            SENTINEL_P
            regpointer textequ <xdi>

            DefinemXD   Population
            DefinemXD   MaturePopulation
            DefinemR8   Density

            DefinemR8   CarbonInBiomass, 0.5

            DefinemR8   AbovegrCarbon
            DefinemR8   BelowgrCarbon
            DefinemR8   TotalCarbon
    ObjectEnd    

    Object WoodyPopulationD,, WoodyPopulation
        RedefineMethod Load, $ObjPtr(Stream);, PDESER_INFO
        RedefineMethod Store, $ObjPtr(Stream)       ;-> Stream

        VirtualMethod  Decomposition
        DefinemR8 MeanAge
    ObjectEnd            

    Specie = 0
    repeat 5
        Object Woody%Specie, , Woody
            RedefineMethod  Init, REAL8, REAL8
            VirtualMethod  CalculateIndividual, XWORD
            
            if Specie eq 0
                DefinemR8   BelowgroundRatio, 0.68
            else
                DefinemR8   Height
                DefinemR8   BelowgroundRatio, 0.28
            endif
            ClassVar    R8, MortalityRate
            TreesVar    R8, WoodDensity
            if Specie eq 1
                DefinemXD  MaxCat, 8
                DefinemR8a GrowthRate, 0.136,0.156,0.136,0.177,0.114,0.068,0.133,0.073,0.112
            elseif Specie eq 2
                DefinemXD  MaxCat, 8
                DefinemR8a GrowthRate, 0.213,0.214,0.183,0.155,0.167,0.172,0.105,0.245,0.180
            elseif Specie eq 3
                DefinemXD  MaxCat, 5
                DefinemR8a GrowthRate, 0.163,0.162,0.251,0.124,0.390,0.373
            elseif Specie eq 4
                DefinemXD  MaxCat, 4
                DefinemR8a GrowthRate, 0.278,0.132,0.160,0.140,0.180
            endif
            
            if Specie eq 0
                DefinemXD   WoodySprout
                DefinemR8   AgeMaturity     , 1.0
                DefinemR8   TargetAgeHarvest, 7.0
            else
                DefinemR8   DBHMaturity, 20.0
                DefinemR8   DBHCut     , 35.0
                DefinemXD   CategoryDBH
                DefinemR8   MeanHeight
                ;DefinemXD   CategoryTarget          ; for Population Stabilization
                
            endif
            ClassVar    R8, LifeSpan
        ObjectEnd
        
        Object WoodyPopulation%Specie,,WoodyPopulation
       ;     RedefineMethod Init
;            RedefineMethod Clean
            RedefineMethod Load, $ObjPtr(Stream);, PDESER_INFO
            RedefineMethod Store, $ObjPtr(Stream)

            VirtualMethod  Initial, POINTER
            VirtualMethod  CalculatePopulation, XWORD
            VirtualMethod  Select, XWORD
            
;            VirtualMethod  StabilizePopulation, XWORD

            regpointer textequ <xdi>
            DefinemR8   RecruitmentRate
            ;DefinemXD   RecruitmentFlow
            DefinemXD   HarvestFlow
            DefinemXD   MortalityFlow
            DefinemXD   Recruit2PopFlow
            if Specie eq 0
                DefinemXD   SproutFlow
            endif
            DefinemXD   PopulationCheck
    
            DefinemR8   InitialPopulation
            ClassVar    R8, TargetDensity
            
            ClassVar    XD, nRecruitmentStage
            ClassArray  R8, RecruitmentStage
            ClassVar    R8, RecruitmentMort
            ClassVar    R8, RecruitmentRateMin    
            ClassVar    R8, RecruitmentRateMax    

            DefinemR8   Timber
            DefinemR8   Recruitment
            if Specie eq 0
                ;DefinemR8   Sprouting                ; count
                DefinemXD   TargetSprouting, iShrubConst
                DefinemR8   MeanAge
                DefinemR8   CalcAge
            else
                DefinemR8   BasalArea
                DefinemR8   MeanHeight
                ;DefinemXD   ReductionCats   
                ;DefinemXD   ReductionLimit, 6 dup (0.0)
                ;DefinemR8   ReductionRate , 6 dup (0.0)
                ;DefinemXD   CatsPopulation, 6 dup (0.0)
            endif
        ObjectEnd
    
        Specie = Specie + 1
    endm    

    FOMSI_results macro
        DefVar Year
	    DefVar AbovegroundShrubCarbon
	    DefVar BelowgroundShrubCarbon
	    DefVar AbovegroundTreeCarbon
	    DefVar BelowgroundTreeCarbon
	    DefVar AbovegroundWoodyCarbon        ;  5
	    DefVar BelowgroundWoodyCarbon
	    DefVar TimberProduction
	    DefVar ShrubDensity
	    DefVar TreeDensity
	    DefVar ShrubRecruitment              ; 10
	    DefVar TreeRecruitment
	    DefVar ShrubRecruitment2Pop
	    DefVar TreeRecruitment2Pop
	    DefVar TotalBasalArea
	    DefVar Area                          ; 15
	    DefVar Mortality
	    DefVar MeanShrubAge
        DefVar CalcShrubAge
        DefVar QuebrachoBlanco
        DefVar QuebrachoColorado             ; 20
        DefVar Mistol
        DefVar Algarrobo
        DefVar QBlancoH
        DefVar QColoradoH             
        DefVar MistolH                      ; 25
        DefVar AlgarroboH
        DefVar Stumps
        DefVar DeadCarbon
        DefVar DeadMeanAge                  
	endm
	results_size equ 30      
	
    FOMSI_results2 macro
	    DefVar TreeCstorage                 ; 35
	    DefVar ShrubCstorage
	    DefVar WoodyCstorage
    endm
Object FOMSIResult,, Primer
    regpointer textequ <xsi>
    SENTINEL_I
    DefType = 0
    FOMSI_results
    DefinemR8 ShrubDensityScaled            ; 30
    DefinemR8 TreeRecruitmentScaled
    DefinemR8 TreeRecruitment2PopScaled
    DefinemR8 BelowgroundTreeCarbonFull
    DefinemR8 ShrubRecruitmentScaled         
    FOMSI_results2
ObjectEnd

Object FOMSI,,Streamable
    RedefineMethod Init, POINTER
    RedefineMethod Done

    VirtualMethod  CalculateModel, XWORD
    VirtualMethod  Clean
    VirtualMethod  Initial, POINTER, XWORD
    VirtualMethod  InstallFSP
    ;VirtualMethod  ShowModel, XWORD
    VirtualMethod  StoreModel, XWORD
    
    VirtualMethod  ReadPopulations, POINTER
    VirtualMethod  SavePopulations, POINTER

    DefineVariable pSimulator, POINTER, 0                 
    DefineVariable pGraphData, POINTER, 0                 

    regpointer textequ <xsi>
    SENTINEL_P                          ; for results
    DefType = 0
    FOMSI_results
    DefineVariable spacer, POINTER, 10 dup (0)                 
    Embed DeadWood, WoodyPopulationD
    Specie = 0
    repeat 5
        @CatStr(<Embed WP>%Specie, < ,WoodyPopulation>,%Specie)
        Specie = Specie + 1
    endm     
    ;Users
    ;Trees DBH_tree_target
    
ObjectEnd


