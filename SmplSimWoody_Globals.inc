; ==================================================================================================
; Title:   SmplSimWoody_Globals.inc
; Author:  Héctor S. Enrique @ January 2026
; Version: 1.0.0
; Purpose:  SmplSimWoody - Forest Carbon Dynamic Simulation
; ==================================================================================================
include SmplSimWoody_Shared.inc

CStr szIcoName,   "ICON_APP" 
CStr szIcoEntry,  "ICON_ENTRY" 
%CStr szAboutText,"&ABOUT_TEXT", 13, 10, \ 
                  "Version &VER_PRODUCTVERSION_STR", 13, 10, \ 
                  "Designed with ObjAsm", 13, 10, \ 
                  "&COPYRIGHT", 13, 10, 13, 10, \ 
                  @CatStr(<!">, %BUILD_DATE_STR, \ 
                          <  -  Build: >, %BUILD_NUMBER, <->, %ASSEMBLER, <->, \
                          %TARGET_MODE_STR, <->, %TARGET_BITNESS, <!">)
.data
    % szAppTitle db "&APP_TITLE" 
	        if TARGET_BITNESS eq 32
	            db " in 32 bits"            
	        else
	            db " in 64 bits"            
	        endif
                db 0
        tabla   db 123,"\rtf1\ansi"
                db 123,"\colortbl ;"
                db "\red192\green192\blue192;"; 00C0C0C0h
                db "\red0\green158\blue78;"; 004E9B00h
                db "\red255\green255\blue128;"; 0080FFFFh
                db "\red255\green255\blue255;";
                ;db "\red144\green160\blue128;\red255\green0\blue0;\red255\green255\blue170;\red112\green112\blue112;\red255\green255\blue255;"
                db 125,13,10, "{\fonttbl{\f0\fnil\fprq1\fcharset0 Tahoma;}}"
                db "\pard \cf1 \qc \par"    
        cut     db " "     
		szTest db " \par\cf4\fs50\qc\pa\b * *  SmplSimWoody 1.0 * * \b0",
		          " \par\par\fs28 \cf3 A light implementation of",13,10,
		          " \par\fs32\par \cf1 FOrest Management SImulator",13,10,
		          " \par ( FOMSI, Filip \i et al.\i0   2025  )",13,10,13,10,
		          " \par\par\fs28 \cf2 HSE, January 2026",
		          " \par\par\par \i\fs26 \cf3 Wickedly Crafted In Microsoft Assembler <MASM>",0
          

        cierre$ db "}",0
        sep1$ db "  ::  ",0  

        tabla2   db 123,"\rtf1\ansi"
                db 123,"\colortbl ;"
                db "\red192\green192\blue192;"; 00C0C0C0h
                db "\red0\green158\blue78;"; 004E9B00h
                db "\red255\green255\blue128;"; 0080FFFFh
                db "\red255\green255\blue255;";
                ;db "\red144\green160\blue128;\red255\green0\blue0;\red255\green255\blue170;\red112\green112\blue112;\red255\green255\blue255;"
                db 125,13,10, "{\fonttbl{\f0\fnil\fprq1\fcharset0 Consolas;}}"
                db "\pard \cf1 \qc \par\par\par\par",0    

test2 db "\trowd" 
db "\pard\intbl\ql Cell 1-1\cell"
db "\pard\intbl\ql Cell 1-2\cell"
db "\pard\intbl\ql Cell 1-3\cell"
db "\row"
db "\pard \cf1 \qc \par",0    


    semicolon$ db ";",0
    backslash$ db "\",0
    red$       db "\red",0
    green$     db "\green",0
    blue$      db "\blue",0
    nulo$       db " ", 0



IDC_TEXT_ABOUT    equ     100
   
IDM_EXIT          equ     1000
IDM_HELPA         equ     1001
IDM_HELPD         equ     1002
IDM_ABOUT         equ     1999
  
SmplSimWoodyID         equ     2000

.data
  
hRes$  POINTER 0
hResE$ POINTER 0
;for SmplSimWoody
; Frequents ----------------------------------------------------------------------
CStr formato1, " %10.2f"
dos   real8  2.0
diez  real8 10.0
    
gral1 real8 0.0

