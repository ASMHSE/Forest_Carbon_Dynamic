; ==================================================================================================
; Title:      SmplSimWoody_Shared.inc
; Author:     Héctor S. Enrique
; Version:    C.1.1
; Purpose:    Basic ObjAsm Application
; Notes:      Version C.1.0, January 2026
;               - First release.
; ==================================================================================================


;General information
APP_TITLE                     textequ   <SmplSimWoody>
ABOUT_TEXT                    textequ   <SmplSimWoody>
COPYRIGHT                     textequ   <Héctor S. Enrique   January 2026> 
VER_FILEVERSION               equ       1,0,0,0
VER_FILEVERSION_STR           textequ   <1.0>
VER_PRODUCTVERSION            equ       1,0,0,0
VER_PRODUCTVERSION_STR        equ       <1.0>

;Build information - updated automatically -
BUILD_NUMBER                  equ       347
BUILD_DATE_STR                textequ   <12/02/2026>
BUILD_BITNESS                 equ       64
BUILD_MODE                    equ       RLS

;Menu IDs
IDM_EXIT                      equ       1000
IDM_HELPA                     equ       1001
IDM_HELPD                     equ       1002
IDM_ABOUT                     equ       1999

IDC_TEXT_ABOUT                equ       100

BUILD_BIN_FORMAT              equ       EXE
BUILD_BIN_SUFFIX_STR          textequ   <>
