; ==================================================================================================
; Title:      RichEditCol.inc
; Author:     G. Friedrich, Hécor S. Enrique
; Version:    C.1.2
; Purpose:    ObjAsm support for RichEditCol objects based on common controls Version 5.81.
; Notes:      Version C.1.0, October 2017
;               - First release.
;             Version C.1.1, October 2017
;               - Modified by HSE to store charformat
;             Version C.1.2, February 2021
;               - Modified by HSE to store parent and intercept some events.
; ==================================================================================================

DEF_RichEditCol struc
  xCtlID      XWORD     ?         ;Contol ID
  dStyle      DWORD     ?         ;Style
  dExStyle    DWORD     ?         ;Extended style
  pTitle      PSTRING   ?         ;-> Window title
  sdPosX      SDWORD    ?         ;X position
  sdPosY      SDWORD    ?         ;Y position
  dWidth      DWORD     ?         ;Width
  dHeight     DWORD     ?         ;Height
DEF_RichEditCol ends
PDEF_RichEditCol typedef ptr DEF_RichEditCol

if TARGET_STR_TYPE eq STR_TYPE_ANSI
  CStr szRichEditCol20, "RichEdit20A"
else
  CStr szRichEditCol20, "RichEdit20W"
endif


       WORDINFO struct
          WordLen dd ?		; the length of the word: used as a quick comparison
          pszWord dd ?		; pointer to the word
          pColor dd ?		; point to the dword that contains the color used to hilite the word
          NextLink dd ?		; point to the next WORDINFO structure
       WORDINFO ends


; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Object:     RichEditCol
; Purpose:    Implement a thin wrapper around the RichEditCol GDI object.

Object RichEditCol, RichEditColID, WinControl                 ;Class "RichEditCol"
  RedefineMethod    Done
  RedefineMethod    Init,         POINTER, HWND, PDEF_RichEditCol
  RedefineMethod    WndProc,      DWORD, WPARAM, LPARAM

  DefineVariable    charF, CHARFORMAT,  {}
  DefineVariable    hControl, HWND, 0  
  DefineVariable    hFont, HWND, 0  

  DefineVariable    rectI, RECT, {}
  DefineVariable    ASMColorArray1, dword, 0FF0000h,0805F50h,0FFh,666F00h,44F0h,5F8754h,4 dup(0FF0000h)
  DefineVariable    CommentColor1, dword, 808000h
  DefineVariable    SymbolColor1, dword, 0000D6D6h
  DefineVariable    ASMSyntaxArray1, dword, 256 dup(?)
  DefineVariable    RichEditVersion, dword, 0

  DefineVariable    changed, XWORD, FALSE
  DefineVariable    CtrlFlag, XWORD, FALSE
  DefineVariable    drawing, XWORD, FALSE

ObjectEnd


; ==================================================================================================

if IMPLEMENT

.data
    ifndef nulo
    nulo db 0
    endif 

.code
; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:     RichEditCol.Done
; Purpose:    Finalize the RichEditCol object.
; Arguments:  None.
; Return:     Nothing.

Method RichEditCol.Done, uses xsi
  SetObject xsi
  Unsubclass RichEditCol                                   ;Uses xsi
  ACall xsi.Done
MethodEnd

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:     RichEditCol.Init
; Purpose:    Initialize the RichEditCol object.
; Arguments:  Arg1: -> Owner object.
;             Arg2: Parent window HANDLE.
;             Arg3: -> DEF_RichEditCol initialization structure.
; Return:     Nothing.

Method RichEditCol.Init, uses xbx xsi, pOwner:POINTER, hParent:HWND, pDefStruc:PDEF_RichEditCol
  SetObject xsi

  mov xbx, pDefStruc
  assume xbx:PDEF_RichEditCol
  mov eax, [xbx].dStyle
  or eax, WS_CHILD or WS_VISIBLE
  invoke CreateWindowEx, [xbx].dExStyle, offset szRichEditCol20, [xbx].pTitle, eax, \
                         [xbx].sdPosX, [xbx].sdPosY, [xbx].dWidth, [xbx].dHeight, \
                         hParent, [xbx].xCtlID, hInstance, 0
  assume xbx:NOTHING
  ACall xsi.Init, pOwner, xax

  Subclass RichEditCol                             ;Uses xsi

MethodEnd

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:     RichEditCol.WndProc
; Purpose:    Processing of window messages. Before invoking it, the window must be subclassed.
; Arguments:  Arg1: Message identifier.
;             Arg2: First message parameter.
;             Arg3: Second message parameter.
; Return:     eax = This value is the result of the message processing and depends on the msg ID.
; Note:       Window HANDLE is passed in pSelf (hidden argument).

ClipboardInProc proc
    xor xax, 1
    ret
ClipboardInProc endp

Method RichEditCol.WndProc, uses xsi xdi xbx, uMsg:DWORD, wParam:WPARAM, lParam:LPARAM
    
    LOCAL hdc : XWORD
	LOCAL FirstChar : XWORD
	LOCAL rect : RECT
	LOCAL txtrange:TEXTRANGEA
	LOCAL buffer[1024*10]:BYTE
	LOCAL hRgn : XWORD
	LOCAL hOldRgn : XWORD
	LOCAL RealRect : RECT
	LOCAL pString : XWORD
	LOCAL BufferSize : DWORD
	LOCAL pt : POINT

    local hOldFont:XWORD
    local charF:CHARFORMAT
    local margin:dword
    
    local str1[2]:CHRA
    local xhWnd:XWORD
    local SymbolColor2:dword
    local id: XWORD
    
    local _RichEditVersion:DWORD
    if @WordSize eq 8
        local fregTLS(,72)
    endif
    local repaste : REPASTESPECIAL
    local editstream : EDITSTREAM
    
    GetSubclassingInst RichEditCol, pSelf
    SetObject xsi, RichEditCol, xax
    
    .if uMsg == EM_STREAMOUT
        ;int 3
    .endif
    OCall xsi::RichEditCol.Dispatch, pSelf, uMsg, wParam, lParam
    
MethodEnd

endif
