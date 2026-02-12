; ==================================================================================================
; Title:   Macs.inc
; Author:  Héctor S. Enrique - January 2026
; Version: 1.0.0
; Purpose: Support macros compilation 
; Notes:    Framework version C.2.0, 2026
;               - First release.
; ==================================================================================================

;######################################################
;    A wrapper to OCall
;    pushing register s before, ans poping after
;    ej:        invope <eax, ecx>, StrCat, addr [xsi].salida, addr [xsi].str_op
;            invope ecx, StrCat, addr [xsi].salida, addr [xsi].str_op
 
OCallp macro regis:REQ, params:VARARG
    local count
    
    argstr textequ <OCall >
    _PushAll <regis>
    count = 0
    FOR param1, <params>
        if count eq 0
           argstr CATSTR argstr, <param1>
            count = count+1 
        else
          argstr CATSTR argstr, <,param1> 
        endif
    ENDM
    argstr
    _PopAllRev <regis>          
    endm
;######################################################
;    A wrapper to invoke 
;    pushing register s before, ans poping after
;    ej:        invope <eax, ecx>, StrCat, addr [xsi].salida, addr [xsi].str_op
;            invope ecx, StrCat, addr [xsi].salida, addr [xsi].str_op
 
invope macro regis:REQ, params:VARARG
    local count
    
    argstr textequ <invoke >
    _PushAll <regis>
    count = 0
    FOR param1, <params>
        if count eq 0
           argstr CATSTR argstr, <param1>
            count = count+1 
        else
          argstr CATSTR argstr, <,param1> 
        endif
    ENDM
    argstr
    _PopAllRev <regis>          
    endm

freal8$ macro valor:req;, pos1:req, pos2:req, tipo:req

    fld valor
    invoke St0ToStr, addr buffer, 8, 2, 0
    fstp st
endm

ChargeR8 macro R8_value
    if TARGET_BITNESS eq 32
        lea   ecx, &R8_value
        mov   dword ptr [ecx], eax
        mov   dword ptr [ecx+4], edx
    else
        mov   &R8_value, xax
    endif

endm

DeclareRichEdit macro xname, xID
	IDM_re&xname equ &xID
	PropRE&xname DEF_RichEditCol {IDM_re&xname, PROPRESTYLE, 0, 0, 0, 0,200, 200}
endm

InitRichEdit macro xname, xparent, xgrid
	ifnb <xgrid>
	    OCall [xsi].re&xname::RichEditCol.Init, xsi, [xsi].&xparent.hWnd, offset PropRE&xname
	else
	    OCall [xsi].re&xname::RichEditCol.Init, xsi, [xsi].hWnd, offset PropRE&xname
	endif
endm
