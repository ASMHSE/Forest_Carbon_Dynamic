; ==================================================================================================
; Title:   BUTTONW.inc
; Author:  G. Friedrich
; Version: 1.0.0
; Purpose: ObjAsm32 support for BUTTONW objects based on common controls Version 5.81.
; Notes:   Version 1.0.0, December 2003
;            - First release.
;		- modified from ComboBox, HSE, October 2015
; ==================================================================================================

;ButtonWID equ	2500

DEF_BUTTONW struc
  xCtlID      XWORD     ?         ;Contol ID
  dStyle      DWORD     ?         ;Style
  dExStyle    DWORD     ?         ;Extended style
  pTitle      PSTRING   ?         ;-> Window title
  union
    rect        RECT      {}
    struct
      sdPosX    SDWORD    ?         ;X position
      sdPosY    SDWORD    ?         ;Y position
      dWidth    DWORD     ?         ;Width
      dHeight   DWORD     ?         ;Height
    ends
  ends
DEF_BUTTONW ends
PDEF_BUTTONW typedef ptr DEF_BUTTONW

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Object:  BUTTONW
; Purpose: Implement a thin wrapper around the BUTTONW GDI object.

Object BUTTONW, ButtonWID, WinControl                 ;Class "BUTTONW"
  RedefineMethod    Done
  RedefineMethod    Init,         POINTER, HWND, PDEF_BUTTONW
  RedefineMethod    WndProc,      DWORD, WPARAM, LPARAM
ObjectEnd

; ==================================================================================================

if IMPLEMENT

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:    BUTTONW.Done
; Purpose:   Finalize the BUTTONW object.
; Arguments: None.
; Return:    Nothing.

Method BUTTONW.Done, uses xsi
    SetObject xsi
    Unsubclass BUTTONW                           ;Uses esi
    ACall xsi.Done
MethodEnd

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:    BUTTONW.Init
; Purpose:   Initialize the BUTTONW object.
; Arguments: Arg1: -> Owner object.
;            Arg2: Parent window HANDLE.
;            Arg3: -> DEF_BUTTONW initialization structure.
; Return:    Nothing.

Method BUTTONW.Init, uses xsi xbx, pOwner:POINTER, hParent:HWND, pDefStruc:PDEF_BUTTONW

    SetObject xsi
    mov xbx, pDefStruc
    assume xbx:PDEF_BUTTONW
    mov eax, [xbx].dStyle
    or eax, WS_VISIBLE or WS_CHILD

    invoke CreateWindowEx, [xbx].dExStyle, $OfsCStr("Button"), [xbx].pTitle, eax, \
                           [xbx].sdPosX, [xbx].sdPosY, [xbx].dWidth, [xbx].dHeight, \
                           hParent, [xbx].xCtlID, hInstance, 0

    assume xbx:NOTHING
   
    ACall xsi.Init, pOwner, xax
    invoke ShowWindow, [xsi].hWnd, SW_SHOW
    Subclass BUTTONW                             ;Uses esi
MethodEnd

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:    BUTTONW.WndProc
; Purpose:   Processing of window messages. Before invoking it, the window must be subclassed.
; Arguments: Arg1: Message identifier.
;            Arg2: First message parameter.
;            Arg3: Second message parameter.
; Return:    eax = This value is the result of the message processing and depends on the message ID.
; Note:      Window HANDLE is passed in pSelf (hidden argument).

Method BUTTONW.WndProc, uses xsi, uMsg:DWORD, wParam:WPARAM, lParam:LPARAM

  GetSubclassingInst BUTTONW, pSelf
  OCall xax::BUTTONW.Dispatch, pSelf, uMsg, wParam, lParam

MethodEnd

endif