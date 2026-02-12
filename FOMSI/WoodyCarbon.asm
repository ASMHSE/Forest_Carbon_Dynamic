; ==================================================================================================
; Title:   WoodyCarbon.asm
; Author:  Héctor S. Enrique @ January 2026
; Version: 1.0.0
; Purpose: WoodyCarbon Simulations
; Notes:    Framework version C.2.0, 2026
;               - First release.
; ==================================================================================================

NOMINMAX = 1
%include @Environ(OBJASM_PATH)\\Code\\Macros\\Model.inc
SysSetup OOP, CON64, ANSI_STRING, DEBUG(WND, INFO);, RESGUARD)

% include &MacPath&fMath.inc
% include &MacPath&BStrings.inc

% include &IncPath&Windows\ShellApi.inc
% include &IncPath&Windows\shlwapi.inc
% include &IncPath&Windows\WinConTypes.inc
% include &IncPath&Windows\tlhelp32.inc

% includelib &LibPath&Windows\shell32.lib
% includelib &LibPath&Windows\shlwapi.lib

% include &MacPath&SDLL.inc

MakeObjects Primer, Stream;, Collection, SortedCollection, StrCollectionA
MakeObjects DiskStream
MakeObjects ConsoleApp

include \masm32\macros\SmplMath\math.inc
fSlvSelectBackEnd FPU

    @reg32_64 ecx, r10
    @reg32_64 edx, r11
    @reg32_64 eax, r12

include \masm32\macros\SmplMath\accs\macros.inc


.data                                                   ;Define global variables in the Data segment
    cBuff       CHR 100 dup(0)


include macros\ModMacs.inc
include macros\SDLLH.inc
include macros\WriteFTr.inc

MakeObjects D:\masm32\projects\agF\ResulS\SmplGraph\SmplBrCast

SmplGraphData_reps equ 15
SmplGraphDefs_reps equ 1
        
MakeObjects D:\masm32\projects\agF\ResulS\SmplGraph\SmplBrStruct
MakeObjects .\FOMSI

    .data?

        align 8
        FOMSIGraphDefs SmplGraphDefs {?}
        align 8
        FOMSIGraphStore SmplGraphStore {?}

    .data
    Undisturbed Shrubland   {1000.0,                                        ; Area
                             4700.0,                                        ; Shrubs num
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
                              
    FalseSP     Shrubland   {1000.0,                                        ; Area
                             4700.0,                                        ; Shrubs num
                              < 0.0,   8.0,   6.0,   6.0,   0.0,   0.0>,    ; quebracho-blanco
                              < 0.0,   0.0,  33.0,  42.0,   0.0,   0.0>,
                              < 0.0,   0.0, 220.0, 280.0,   0.0,   0.0>,
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0>,    ; lorentzii
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0>,
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0>,
                              < 0.2,   0.0,   0.0,   0.0,   0.0,   0.0>,    ; mistol
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0>,
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0>,
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0>,    ; nigra  
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0>,
                              < 0.0,   0.0,   0.0,   0.0,   0.0,   0.0> }


include WoodyCarbon.inc

.code                                                   ;Begin Code segment

 


; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
start proc                                              ;Here is the program entry point
    SysInit                                               ;Runtime initialization of the OOP model
    ;ifndef SILENT
    ;    DbgClearAll
    ;endif

    OCall $ObjTmpl(WoodyCarbon)::WoodyCarbon.Init           ;Initialize application
    OCall $ObjTmpl(WoodyCarbon)::WoodyCarbon.Initial, addr Undisturbed  ;Initialize model

    ;OCall $ObjTmpl(WoodyCarbon)::WoodyCarbon.ReadInputData  ;Execute data load
    OCall $ObjTmpl(WoodyCarbon)::WoodyCarbon.Run            ;Execute application

    OCall $ObjTmpl(WoodyCarbon)::WoodyCarbon.PrintLn, $OfsCStr(10,13,"Press any key to continue...")
    .repeat
        OCall $ObjTmpl(WoodyCarbon)::WoodyCarbon.GetInputChar
    .until eax != NULL

    OCall $ObjTmpl(WoodyCarbon)::WoodyCarbon.Done           ;Finalize application

    SysDone                                               ;Runtime finalization of the OOP model

    invoke ExitProcess, 0                                 ;Exit program returning 0 to Windows OS
start endp
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

end

Paper fail to describe how sprouting play in shrubs population dyamics. 

Not properly defined what is the model and what are the rules.