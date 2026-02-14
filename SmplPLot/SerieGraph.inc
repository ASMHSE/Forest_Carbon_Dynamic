; ==================================================================================================
; Title:   SerieGraph.inc
; Author:  Héctor S. Enrique - January 2026
; Version: 1.0.0
; Purpose: Simple graphic implementation 
; Notes:    Framework version C.2.0, 2026
;               - First release.
; ==================================================================================================

Method SerieGraph.Init, uses xsi, xprior:dword,xcolor:dword, xmod:dword
	SetObject xsi
	
	m2m	[xsi].Prioridad, xprior, eax
	m2m	[xsi].Modo, xmod, eax
	m2m [xsi].Color, xcolor	, eax

	;DbgHex xcolor
	mov [xsi].Brush, $invoke( CreateSolidBrush , xcolor)
	;DbgDec [xsi].Brush
	mov [xsi].Pen, $invoke( CreatePen, PS_SOLID, 1, xcolor)
	;DbgDec [xsi].Pen
	SDLL_InitH xsi,, Puntito
MethodEnd

Method SerieGraph.CargaPunto , uses xsi xax, x : REAL8, y : REAL8
	SetObject xsi
	New Puntito
	OCall xax::Puntito.Init, x, y      			;Initialize Puntito instance values
    SDLL_AppendH xsi,, Puntito, xax,,xcx 
MethodEnd

Method SerieGraph.Clear, uses xsi xbx
    local fregTLS()
    ;DbgText "SerieGraph.Clear"
	SetObject xsi
 	invoke DeleteObject, [xsi].Brush
 	;DbgApiError
 	invoke DeleteObject, [xsi].Pen
 	;DbgApiError
	SDLL_CleanH xsi,, Puntito, xbx
MethodEnd
Method SerieGraph.Done, uses xsi xbx
    local fregTLS()
	SetObject xsi
 	invoke DeleteObject, [xsi].Brush
 	;DbgApiError
 	invoke DeleteObject, [xsi].Pen
 	;DbgApiError
    assume xbx : $ObjPtr(Puntito)
	SDLL_DoneH xsi,, Puntito, xbx
	assume xbx : nothing
MethodEnd
