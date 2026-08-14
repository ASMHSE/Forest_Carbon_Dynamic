; ==================================================================================================
; Title:   SmplSimWoody.asm
; Author:  Héctor S. Enrique @ January 2026
; Version: 1.0.0
; Purpose:  SmplSimWoody - Forest Carbon Dynamic Simulation
; ==================================================================================================
part equ 2

% include @Environ(OBJASM_PATH)\Code\Macros\Model.inc   ;Include & initialize standard modules
SysSetup OOP, WIN64, ANSI_STRING;, DEBUG(WND,INFO,RESGUARD)            ;Load OOP files and OS related objects
;SysSetup OOP, WIN32, ANSI_STRING;, DEBUG(WND,INFO)            ;Load OOP files and OS related objects

% include &IncPath&Windows\CommCtrl.inc
% includelib &LibPath&Windows\Comctl32.lib
% include &IncPath&Windows\Richedit.inc
% include &IncPath&Windows\oleauto.inc

% includelib &LibPath&Windows\oleaut32.lib
% includelib &LibPath&Windows\shell32.lib
% includelib &LibPath&Windows\shlwapi.lib

;Load or build the following objects
MakeObjects Primer, Stream, DiskStream
MakeObjects WinPrimer, Button, Hyperlink
MakeObjects Window, Dialog, DialogModal, DialogAbout
MakeObjects WinControl, ComboBox, Progressbar
MakeObjects WinApp, SdiApp


include \masm32\macros\SmplMath\math.inc
fSlvSelectBackEnd FPU

    @reg32_64 ecx, r10
    @reg32_64 edx, r11
    @reg32_64 eax, r12

include \masm32\macros\SmplMath\freg_notin32.inc
include \masm32\macros\SmplMath\accs\macros.inc

% include &MacPath&\fMath.inc
include macs.inc

include macros\ModMacs.inc
include macros\SDLLH.inc
include macros\WriteFTr.inc

.data

    hProgressBar HWND 0
    hlibRE       HWND 0

MakeObjects FOMSI\FOMSI
MakeObjects FOMSI\WoodyCarbon
MakeObjects SmplPlot\SmplPlot
ButtonWID equ 16001
MakeObjects Controls\ButtonW
RichEditColID equ 16002
MakeObjects Controls\RichEditCol

include SmplSimWoody_Globals.inc                              ;Application globals
include SmplSimWoody_Main.inc                                 ;SmplSimWoodyobject

.code
start proc                                                  ;Program entry point
    SysInit                                                 ;Runtime model initialization
    
    DbgClear
    ResGuard_Start                                         ;ResGuard initialization
    
    invoke SysAllocStringByteLen, NULL, 250000
    mov hRes$, xax
    invoke LoadLibrary, $OfsCStr("RichEd20.dll") 
    mov   hlibRE, xax
        OCall $ObjTmpl(SmplSimWoody)::SmplSimWoody.Init           ;Initialize application

        OCall $ObjTmpl(SmplSimWoody)::SmplSimWoody.Run            ;Execute the application
        
        OCall $ObjTmpl(SmplSimWoody)::SmplSimWoody.Done           ;Finalize it
    invoke SysFreeString, hRes$
   	invoke FreeLibrary, hlibRE

    ResGuard_Show                                           ;Show ResGuard results
    ResGuard_Stop                                           ;Stop ResGuard results
    
    SysDone                                                  ;Runtime model finalization
    
    invoke ExitProcess, 0                                    ;Program exit returning 0 to the OS
start endp
end
