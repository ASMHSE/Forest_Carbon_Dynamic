; ==================================================================================================
; Title:   WoodyCarbon.inc
; Author:  Héctor S. Enrique @ January 2026
; Version: 1.0.0
; Purpose: WoodyCarbon Simulations running FOMSI 
; Notes:    Framework version C.2.0, 2026
;               - First release.
; ==================================================================================================
turn_nothing           equ     0
turn_harvest_tree      equ     1
turn_harvest_shrub     equ     2
turn_sel_harvest_tree  equ     4
turn_sel_harvest_shrub equ     8
turn_FSP               equ    16    ; remove recruitment in FSP 


Object WoodyCarbon,,Primer
    
    RedefineMethod Init
    RedefineMethod Done
    
    VirtualMethod  Initial, POINTER, XWORD
    VirtualMethod  Run, XWORD
    
    VirtualMethod  Stabilization
    VirtualMethod  NoIntervention
    VirtualMethod  FullImpact
    VirtualMethod  LowImpact
    VirtualMethod  IntensiveForage
    
    VirtualMethod  ModelMeans, XWORD
    VirtualMethod  Comparisons
    
    DefineVariable model, $ObjPtr(FOMSI), NULL
    DefineVariable calculado, XWORD, 0
    DefineVariable UPNFmeans, POINTER, 0
    DefineVariable LowImeans, POINTER, 0
    DefineVariable FSPmeans, POINTER, 0
    DefineVariable MBGImeans, POINTER, 0
ObjectEnd


if IMPLEMENT

    .data
    Undisturbed Shrubland   {1000.0,                                          ; Area
                              ShrubConst, ShrubConst, 1,                    ; Shrubs num, terget, nsprouts (not used yet)
                              <15.0,  16.0,   7.0,   2.0,   0.8,   0.2>,    ; quebracho-blanco
                              <13.0,  21.0,  33.0,  42.0,  50.0,  58.0>,
                              <73.0, 150.0, 214.0, 286.0, 397.0, 465.0>,
                              < 6.0,   6.0,   3.0,   1.5,   0.8,   0.6>,    ; lorentzii
                              <11.0,  22.0,  33.0,  42.0,  51.0,  59.0>,
                              <78.0, 150.0, 215.0, 287.0, 392.0, 474.0>,
                              <14.0,  12.0,   4.0,   1.4,   0.5,   0.0>,    ; mistol
                              <12.0,  22.0,  32.0,  42.0,  54.0,   0.0>,
                              <62.0, 120.0, 171.0, 223.0, 247.0,   0.0>,
                              <14.2,  12.2,   4.2,   1.4,   0.0,   0.0>,    ; nigra  
                              <14.0,  23.0,  33.0,  44.0,   0.0,   0.0>,
                              <34.0,  70.0, 141.0, 175.0,   0.0,   0.0> }
                              
    FalseSP     Shrubland   {1000.0,                                         ; Area
                             ShrubConst, ShrubConst, 1,                     ; Shrubs num, terget, nsprouts (not used yet)
                              < 0.0,   8.0,   6.0,   6.0,   0.0,   0.0>,    ; quebracho-blanco
                              < 0.0,  21.0,  33.0,  42.0,   0.0,   0.0>,
                              < 0.0, 150.0, 220.0, 280.0,   0.0,   0.0>,
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0>,    ; lorentzii
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0>,
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0>,
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0>,    ; mistol
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0>,
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0>,
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0>,    ; nigra  
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0>,
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0> }
                  
Method WoodyCarbon.Init, uses xdi xsi

    SetObject xdi
    ACall xdi.Init, NULL
 
    New FOMSI
    mov  [xdi].model, xax
    OCall xax::FOMSI.Init, xdi
MethodEnd

Method WoodyCarbon.Done, uses xdi
    SetObject xdi
   	
    Destroy [xdi].model
MethodEnd

Method WoodyCarbon.Initial, uses xdi xsi, pShrubland:POINTER, genpop:XWORD

    SetObject xdi

    OCall [xdi].model::FOMSI.Initial, pShrubland, genpop


MethodEnd

Method WoodyCarbon.Stabilization, uses xdi
    local i : xword, turn: xword
    
    DbgText "WoodyCarbon.Stabilization"
    
    SetObject xdi
    YEARS = 1000
    invoke SendMessage, hProgressBar, PBM_SETRANGE32,0,YEARS
    mov turn, 0
    ForLpD i, 0, YEARS
        DbgDec xax
        OCall [xdi].model::FOMSI.CalculateModel, turn
        OCall [xdi].model::FOMSI.StoreModel, i
        invoke  SendMessage, hProgressBar, PBM_SETPOS, i,0
    Next i 
    mov   [xdi].calculado, 1    
MethodEnd

Method WoodyCarbon.NoIntervention, uses xdi
    local i : xword, turn: xword
    local fregTLS()

    SetObject xdi

    YEARS = 90
    invoke SendMessage, hProgressBar, PBM_SETRANGE32,0,YEARS
    ;OCall [xdi].model::FOMSI.StoreModel, 0
    
    mov turn, 0
    ForLp i, 1, YEARS
        DbgDec xax
        OCall [xdi].model::FOMSI.CalculateModel, turn
        OCall [xdi].model::FOMSI.StoreModel, i
        invoke  SendMessage, hProgressBar, PBM_SETPOS, i,0
    Next i
    OCall xdi.ModelMeans, YEARS
    _push xax
    .if [xdi].UPNFmeans
        Destroy [xdi].UPNFmeans
    .endif
    _popv [xdi].UPNFmeans
    mov   [xdi].calculado, 1    
MethodEnd

Method WoodyCarbon.FullImpact, uses xdi
    local i : xword, turn: xword
    local fregTLS()
    
    SetObject xdi

    YEARS = 90    
    invoke SendMessage, hProgressBar, PBM_SETRANGE32,0,YEARS

    ForLp i, 1, YEARS
        DbgDec xax
        ;inc xax
        mov   turn, 0
        mov   edx, 0
        mov   cx, 30
        div   cx
        .if dx == 0
            or    turn, turn_harvest_tree
            DbgText " ---------------------- Tree Cut "
        .endif
        mov   xax, i
        dec   xax
        mov   edx, 0
        mov   cx, 7
        div   cx
        .if dx == 0
            or    turn, turn_harvest_shrub
            DbgText " ---------------------- Shrub Cut "
        .endif        
        OCall [xdi].model::FOMSI.CalculateModel, turn
        OCall [xdi].model::FOMSI.StoreModel, i
        invoke  SendMessage, hProgressBar, PBM_SETPOS, i,0
    Next i 
    mov   [xdi].calculado, 1    
MethodEnd
Method WoodyCarbon.LowImpact, uses xdi
    local i : xword, turn: xword
    local fregTLS()
    
    SetObject xdi

    YEARS = 90    
    invoke SendMessage, hProgressBar, PBM_SETRANGE32,0,YEARS

    ForLp i, 1, YEARS
        DbgDec xax
        ;inc xax
        mov   turn, 0
        mov   edx, 0
        mov   cx, 30
        div   cx
        .if dx == 0
            or    turn, turn_sel_harvest_tree
            DbgText " ---------------------- Tree Cut Low"
        .endif
        mov   xax, i
        dec   xax
        mov   edx, 0
        mov   cx, 7
        div   cx
        .if dx == 0
            or    turn, turn_sel_harvest_shrub
            DbgText " ---------------------- Shrub Cut "
        .endif        
        DbgBin turn
        OCall [xdi].model::FOMSI.CalculateModel, turn
        OCall [xdi].model::FOMSI.StoreModel, i
        invoke  SendMessage, hProgressBar, PBM_SETPOS, i,0
    Next i 
    OCall xdi.ModelMeans, YEARS
    _push xax
    .if [xdi].LowImeans
        Destroy [xdi].LowImeans
    .endif
    _popv [xdi].LowImeans

    mov   [xdi].calculado, 1    

MethodEnd
Method WoodyCarbon.IntensiveForage, uses xdi
    local i : xword, turn: xword
    local fregTLS()
    DbgText "WoodyCarbon.IntensiveForage"
    SetObject xdi
    
    YEARS = 90
    invoke SendMessage, hProgressBar, PBM_SETRANGE32,0, YEARS

    OCall [xdi].model::FOMSI.InstallFSP
    
    mov turn, turn_FSP or turn_harvest_shrub
    ForLp i, 1, YEARS
        DbgDec xax
        OCall [xdi].model::FOMSI.CalculateModel, turn
        OCall [xdi].model::FOMSI.StoreModel, i
        invoke  SendMessage, hProgressBar, PBM_SETPOS, i,0
    Next i 
    OCall xdi.ModelMeans, YEARS
    _push xax
    .if [xdi].FSPmeans
        Destroy [xdi].FSPmeans
    .endif
    _popv [xdi].FSPmeans
    mov   [xdi].calculado, 1    
MethodEnd

Method WoodyCarbon.Run, uses xsi xbx xdi, runtype:xword
    local _time : xword

    SetObject xdi

    .if [xdi].calculado
        mov   [xdi].calculado, 0
    .endif
    OCall [xdi].model::FOMSI.Clean

    invoke GetTickCount
    mov   _time, xax
        
    mov  xax, runtype
    .if xax != 0
        .if xax > 2
	        invoke FileExist, offset StabFile
	        .if xax == NULL
	            invoke MessageBox,NULL,$OfsCStr("File Estable1.stm don't exists.",13,10,13,10, "Run Stabilization to create it."),
	                                   $OfsCStr("Missing stabilized population"),MB_OK
	            mov   [xdi].calculado, 0                       
	            ExitMethod                       
	        .endif
        .endif
        mov  xax, runtype
	    ; --------------------------------- Initialize model  
	    .if xax == 1 
	    	OCall xdi.Initial, addr Undisturbed, TRUE
	        OCall xdi.Stabilization
            OCall [xdi].model::FOMSI.SavePopulations, offset StabFile

	    .elseif xax == 2
	    
	    	OCall xdi.Initial, addr FalseSP, TRUE
	        OCall xdi.Stabilization
            OCall [xdi].model::FOMSI.SavePopulations, $OfsCStr("Estable2.stm")
	        
	    ; --------------------------------- Comparisons
	    .elseif xax == 3
            OCall xdi.Comparisons

	    ; --------------------------------- Strategy
	    .elseif xax > 3
	        OCall xdi.Initial, addr Undisturbed, FALSE 
            OCall [xdi].model::FOMSI.ReadPopulations, offset StabFile
	        .if xax     
                mov  xax, runtype
    		    .if xax == 4
			        OCall xdi.NoIntervention
			    .elseif xax == 5
			        OCall xdi.FullImpact
			    .elseif xax == 6    
			        OCall xdi.LowImpact
			    .elseif xax == 7    
			        OCall xdi.IntensiveForage
		        .endif
	        .endif
	    .endif
    .endif    

    invoke GetTickCount
    mov   xcx, _time
    sub   xax, xcx
    mov   _time, xax
    DbgDec _time

MethodEnd

Method WoodyCarbon.ModelMeans, uses xsi xdi, years : XWORD
    local fregTLS()
    ;DbgText "WoodyCarbon.ModelMeans" 
    SetObject xsi
    
    mov xdi, [xsi].model
	assume xdi: $ObjPtr(FOMSI)
            
    New FOMSIResult
    assume xax: ptr $Obj(FOMSIResult)
    DefType = 1
    FOMSI_results
    FOMSI_results2
    assume _ecx: ptr $Obj(FOMSIResult)
    SDLL_BeginLoop xdi,,FOMSIResult, _ecx,, _edx
	    DefType = 2
	    FOMSI_results
        FOMSI_results2
    SDLL_EndLoop _ecx
    DefType = 3
    FOMSI_results
    FOMSI_results2
    assume _ecx:nothing
    assume xax:nothing
MethodEnd

Method WoodyCarbon.Comparisons, uses xdi xbx xsi
    local buffer[248]:CHR
    local fregTLS(,320)
    
    SetObject xdi
        
    OCall xdi.Initial, addr Undisturbed, FALSE 
    OCall [xdi].model::FOMSI.ReadPopulations, offset StabFile
    OCall xdi.NoIntervention
    OCall [xdi].model::FOMSI.Clean
    OCall xdi.Initial, addr Undisturbed, FALSE 
    OCall [xdi].model::FOMSI.ReadPopulations, offset StabFile
    OCall xdi.LowImpact
    OCall [xdi].model::FOMSI.Clean
	OCall xdi.Initial, addr FalseSP, FALSE
    OCall [xdi].model::FOMSI.ReadPopulations, offset StabFile
    OCall xdi.IntensiveForage
    
    mov   cut, 0
    invoke StrCopy, hRes$, addr tabla2

    assume xax: ptr $Obj(FOMSIResult)
    assume _ecx: ptr $Obj(FOMSIResult)
    assume _edx: ptr $Obj(FOMSIResult)
    assume xbx: ptr $Obj(FOMSIResult)
    
    New FOMSIResult
    mov   _ecx, [xdi].UPNFmeans
    mov   _edx, [xdi].LowImeans
    mov   xbx, [xdi].FSPmeans

    DefType = 4
    FOMSI_results
    FOMSI_results2

    mov   _edx, xax
    _pushad
    lea   xsi, buffer 

        MostrarLinea macro texto, variable
            _pushad 64
            ifb <variable>
                WriteF xsi,&texto
            else
                WriteFr xsi,&texto, [_ecx].R8_&variable,[_edx].R8_&variable,[xbx].R8_&variable
            endif
            _popad 64
            _pushad 64
            invoke StrCat, hRes$, xsi
            _popad 64
        endm

    MostrarLinea "\par \cf3                        UPNF          MBGI           FSP  "
    _pushad 64
    invoke StrCat, hRes$, $OfsCStr(" \cf1 ")
    _popad 64
    MostrarLinea "\par Tree recruitment  ¦F82   ¦F82   ¦F82 ", TreeRecruitment
    MostrarLinea "\par Tree density      ¦F82   ¦F82   ¦F82 ", TreeDensity
    MostrarLinea "\par Tree basal area   ¦F82   ¦F82   ¦F82 ", TotalBasalArea
    MostrarLinea "\par Shrub recruitment ¦F82   ¦F82   ¦F82 ", ShrubRecruitment
    MostrarLinea "\par Shrub density     ¦F82   ¦F82   ¦F82 ", ShrubDensity

    MostrarLinea "\par Tree C storage    ¦F82   ¦F82   ¦F82  ", TreeCstorage
    MostrarLinea "\par Shrub C storage   ¦F82   ¦F82   ¦F82  ", ShrubCstorage
    MostrarLinea "\par Woody C storage   ¦F82   ¦F82   ¦F82  ", WoodyCstorage

    assume xax: nothing
    assume _ecx: nothing
    assume _edx: nothing
    assume xbx: nothing
    _popad
    .if [xdi].MBGImeans
        Destroy [xdi].MBGImeans
    .endif
    mov [xdi].MBGImeans, xax

MethodEnd    
endif