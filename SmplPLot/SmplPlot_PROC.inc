; ==================================================================================================
; Title:   SmplPlot_PROC.inc
; Author:  Héctor S. Enrique - January 2026
; Version: 1.0.0
; Purpose: Simple graphic implementation 
; Notes:    Framework version C.2.0, 2026
;               - First release.
; ==================================================================================================

tmg   textequ <5> 
limite equ 088h 
sombreado textequ <0.50> 

.data 
    CStr szSmplPlot, "SmplPlot"                           ;Creates szDemoApp02 string in .const

    rcClient 	RECT {}	  ;                // client area rectangle 
    ptClientUL	POINT {} ;              // client upper left corner 
    ptClientLR	POINT {} ;              // client lower right corner 

    ptsBegin	POINTS {} ;        // beginning point 
    ptsEnd		POINTS {} ;          // new endpoint 
    ptsPrevEnd	POINTS {} ;      // previous endpoint 
    ;static BOOL fPrevLine = FALSE; // previous line flag 

	gral2 real8 0.0
	pasada dword 0
.code 

Method Puntito.Init, uses xsi, x1 : QWORD, y1 : QWORD 
	SetObject xsi 
	finit 
	fld   x1 
	fstp  [xsi].x 
	fld   y1 
	fstp  [xsi].y 
MethodEnd 
; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:    ProjectWnd.Init
; Purpose:   Initialize the ProjectWnd object.
; Arguments: Arg1: -> Owner object.
;            Arg2: Parent window HANDLE.
; Return:    Nothing.

Method SmplPlot.Init, uses xsi, pOwner : POINTER, hParent : HWND 
	SetObject xsi 
	ACall xsi.Init, pOwner, NULL        ;hWnd is set in WndProc
	invoke CreateWindowEx, WS_EX_CLIENTEDGE, offset szSmplPlot, offset szSmplPlot, \ 
	        WS_CHILD, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, \ 
	        hParent, NULL, hInstance, pSelf 
	invoke ShowWindow, [xsi].hWnd, SW_SHOWNORMAL 

MethodEnd 

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:    ProjectWnd.Startup
; Purpose:   Register the object with the OS.
; Arguments: None.
; Return:    Nothing.

Method SmplPlot.Startup 

	local WC : WNDCLASSEX 
	
	mov   WC.cbSize, sizeof WNDCLASSEX 
	mov   WC.style, CS_HREDRAW or CS_VREDRAW or CS_BYTEALIGNWINDOW or CS_BYTEALIGNCLIENT 
	m2m   WC.lpfnWndProc, $MethodAddr(SmplPlot.WndProc), xdx 
	m2z   WC.cbClsExtra 
	m2z   WC.cbWndExtra 
	m2m   WC.hInstance, hInstance, xdx 
	mov   WC.hbrBackground, NULL  ; COLOR_BTNFACE + 1
	m2z   WC.lpszMenuName 
	c2m   WC.lpszClassName, offset szSmplPlot, xax 
	m2z   WC.hIcon 
	m2z   WC.hIconSm 
	mov   WC.hCursor, $invoke(LoadCursor, NULL, IDC_ARROW) 
	
	invoke RegisterClassEx, addr WC 

MethodEnd 

	CalculaMarcos macro 
	
	    invoke GetClientRect, [xsi].hWnd, addr [xsi].total 
	
	    mov   eax, [xsi].total.bottom ;  by - 31
	    sub   eax, [xsi].pxcabecera 
	    sub   eax, [xsi].pxpie 
	    mov   [xsi].alto, eax 
	
	    mov   eax, [xsi].total.right 
	    sub   eax, [xsi].pxeje1 
	    sub   eax, [xsi].pxeje2 
	    mov   [xsi].ancho, eax 
	
	    mov   eax, [xsi].total.bottom 
	    sub   eax, [xsi].pxpie 
	    mov   [xsi].posiy, eax ;= by - 31;
	
	    mov   eax, [xsi].pxeje1 
	    mov   [xsi].posix, eax ;	  = ax + 75;
	    mov   [xsi].RT.left, eax 
	
	    mov   eax, [xsi].total.right 
	    sub   eax, [xsi].pxeje2 
	    mov   [xsi].RT.right, eax 
	
	  ;mov		eax , [xsi].posiy
	  ;sub		eax , [xsi].alto
	    mov   eax, [xsi].pxcabecera 
	    mov   [xsi].RT.top, eax 
	
	    add   eax, [xsi].alto 
	  ;add		eax , [xsi].pxcabecera
	    mov   [xsi].RT.bottom, eax 
	
	endm  

Method SmplPlot.OnMouseDown, uses xsi, wParam : WPARAM, lParam : LPARAM 
	local fregTLS() 
	
	SetObject xsi 
	
	
	OCall [xsi].BaseSeries::LinkedList.GetFirst 
	.if xax !=NULL 
	    invope xsi, GetClientRect, [xsi].hWnd, addr [xsi].total 
	
	 ; Capture mouse input. 
	    invope xsi, SetCapture, [xsi].hWnd 
	;	CalculaMarcos
	
	    fSlv8 ptClientUL.x = [xsi].RT.left    ; 
	    fSlv8 ptClientUL.y = [xsi].RT.top    ; 
	
	        ; Add one to the right and bottom sides, because the 
	        ; coordinates retrieved by GetClientRect do not 
	        ; include the far left and lowermost pixels. 
	
	    fSlv8 ptClientLR.x = [xsi].RT.right + 1 ; 
	    fSlv8 ptClientLR.y = [xsi].RT.bottom + 1    ;+ptClientUL.y; 
	
	    invope xsi, ClientToScreen, [xsi].hWnd, addr ptClientUL 
	    invope xsi, ClientToScreen, [xsi].hWnd, addr ptClientLR 
	
	    invope xsi, SetRect, addr [xsi].RT, ptClientUL.x, ptClientUL.y, ptClientLR.x, ptClientLR.y 
	    invope xsi, ClipCursor, addr [xsi].RT    ;RT;(&rcClient); 
	
	    mov   [xsi].mouseactivo, TRUE 
	
	    OCall xsi.selecciona, lParam 
	    invoke RedrawWindow, [xsi].hWnd, NULL, NULL, \ 
	    RDW_INVALIDATE or RDW_UPDATENOW or RDW_ALLCHILDREN 
	        ;invoke InvalidateRect, [xsi].hWnd, addr [xsi].RT, FALSE  ;RDW_ERASE or              
	.endif 
	xor   eax, eax 
MethodEnd 
Method SmplPlot.OnMouseMove, uses xsi, wParam : WPARAM, lParam : LPARAM 

	local hdc : HDC 
	
	SetObject xsi 
	
	  ; When moving the mouse, the user must hold down 
	     ; the left mouse button to draw lines. 
	.if (wParam & MK_LBUTTON) 
	
	    OCall xsi.selecciona, lParam 
	    invoke RedrawWindow, [xsi].hWnd, NULL, NULL, \ 
	    RDW_INVALIDATE or RDW_UPDATENOW or RDW_ALLCHILDREN 
	        ;invoke InvalidateRect, [xsi].hWnd, addr [xsi].RT,FALSE ;RDW_ERASE or              
	
	.endif 
	MethodEnd 
	Method SmplPlot.OnMouseUp, uses xsi, wParam : WPARAM, lParam : LPARAM 
	SetObject xsi 
	
	    ;// The user has finished drawing the line. Reset the 
	    ;// previous line flag, release the mouse cursor, and 
	    ;,// release the mouse capture. 
	
	    ;fPrevLine = FALSE; 
	
	invoke ClipCursor, NULL 
	
	invoke ReleaseCapture 
	
	mov   [xsi].mouseactivo, FALSE 
	invoke RedrawWindow, [xsi].hWnd, NULL, NULL, \ 
	RDW_INVALIDATE or RDW_UPDATENOW or RDW_ALLCHILDREN 
	xor   eax, eax 
MethodEnd 


Method SmplPlot.OnSize, uses xsi, wParam : WPARAM, lParam : LPARAM 

	SetObject xsi 
	
	CalculaMarcos 
	
	invoke DefWindowProc, [xsi].hWnd, WM_SIZE, lParam, wParam 

MethodEnd 
; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:    ProjectWnd.OnPaint
; Purpose:   Event procedure for WM_PAINT message.
; Arguments: Arg1: First message parameter.
;            Arg2: Second message parameter.
; Return:    eax = Zero if handled.

Method SmplPlot.OnPaint, uses xsi xdi xbx, wParam : WPARAM, lParam : LPARAM 
	local PS : PAINTSTRUCT 
	local hDC : HDC 
	
	local memdc : HDC 
	local hbm : HWND 
	local hOld : HWND 
	;---------------------------------- demo
	local hMainDIB : HWND, hOldDIB : HWND 
	local pMainDIB :POINTER 
	local LCDBITMAPINFO : BITMAPINFOHEADER 
	local i : dword, j : dword 
	local cdYSize : dword, cdXSize : dword 
	local var_pixel : dword 
	local retBMP : BITMAP 
	local pbmi : dword 
	
	local bmiColors : RGBQUAD 
	local srcedibbmap : BITMAPINFOHEADER 
	local BytesPerScanLine : dword 
	local PadBytesPerScanLine : dword 
	
	 ;local array3D : dword
	 ;----------------------------------
	;DbgText "SmplPlot.OnPaint" 
	SetObject xsi 
	
	invoke BeginPaint, [xsi].hWnd, addr PS 
	mov   hDC, xax 
	
	    ;counter_begin 1000, HIGH_PRIORITY_CLASS
	
	invoke CreateCompatibleDC, hDC 
	mov   memdc, xax 
	
	invoke GetClientRect, [xsi].hWnd, addr [xsi].total 
	
	invoke CreateCompatibleBitmap, hDC, [xsi].total.right, [xsi].total.bottom 
	mov   hbm, xax 
	invoke SelectObject, memdc, xax 
	mov   hOld, xax 
	
	invoke SelectObject, memdc, [xsi].BLUE_BRUSH 
	
	invoke PatBlt, memdc, 0, 0, [xsi].total.right, [xsi].total.bottom, PATCOPY 
	
	invoke SelectObject, memdc, [xsi].hFntGUI 
	
	.if [xsi].calculado == TRUE 
	
	    OCall xsi.BuscaMaximos 
	
	    OCall xsi.grilla, memdc, 1, 100 
	
	    OCall xsi.graficaS, memdc 

		.if [xsi].mouseactivo
			OCall xsi.graficaSdat, memdc
		.endif   

	.endif 
	mov   edi, PS.rcPaint.left 
	mov   ebx, PS.rcPaint.top 
	mov   __ecx, PS.rcPaint.right 
	mov   __edx, PS.rcPaint.bottom 
	sub   __ecx, edi                                                               ;ECX = width
	sub   __edx, ebx 
	invoke BitBlt, hDC, edi, ebx, __ecx, __edx, memdc, edi, ebx, SRCCOPY 
	
	invoke SelectObject, memdc, hOld 
	invoke DeleteObject, hbm 
	invoke DeleteDC, memdc 
	
	invoke EndPaint, [xsi].hWnd, addr PS 
	    ;invoke DefWindowProc,[xsi].hWnd, WM_PAINT, wParam, lParam
MethodEnd
	 
Method SmplPlot.OnClose, uses xsi, wParam : WPARAM, lParam : LPARAM 
	
	SetObject xsi 
	
	 ;DbgText "Closing"
	
	OCall xsi.Clear 
	
	xor   eax, eax 
	    ;invoke DefWindowProc,[xsi].hWnd, WM_CLOSE, wParam, lParam
MethodEnd 

Method SmplPlot.OnCommand, uses xsi, wParam : WPARAM, lParam : LPARAM 

	 ;DbgText "Command"
	 ;DbgDec wParam
	
	SetObject xsi 
	
	.if wParam == WM_USER 
	
	    OCall xsi.Clear 
	  ;invoke RedrawWindow, [xsi].hWnd, NULL, NULL, \
	         ;              RDW_INVALIDATE or RDW_UPDATENOW or RDW_ALLCHILDREN
	    xor   eax, eax 
	.else 
	    invoke DefWindowProc, [xsi].hWnd, WM_COMMAND, wParam, lParam 
	.endif 
MethodEnd 

;======================================================================0
	comparaMaximo macro 
	    .if inicio == TRUE 
	        compvalR [xbx].x, [xsi].minimox 
	        .if ( ah & FP_LESSTHAN) 
	            fld   [xbx].x 
	            fstp  [xsi].minimox 
	        .endif 
	        compvalR [xsi].maximox, [xbx].x 
	        .if ( ah & FP_LESSTHAN) 
	            fld   [xbx].x 
	            fstp  [xsi].maximox 
	        .endif 
	
	        compvalR [xbx].y, [xsi].minimoy 
	        .if ( ah & FP_LESSTHAN) 
	            fld   [xbx].y 
	            fstp  [xsi].minimoy 
	        .endif 
	
	        compvalR [xsi].maximoy, [xbx].y 
	        .if ( ah & FP_LESSTHAN) 
	            fld   [xbx].y 
	            fstp  [xsi].maximoy 
	        .endif 
	    .else 
	        mov   inicio, TRUE 
	    .endif 
	    fld   [xsi].maximox 
	    fadd  [xsi].minimox 
	    fdiv  dos 
	    fstp  [xsi].meanx 
	
	    fld   [xsi].maximoy 
	    fadd  [xsi].minimoy 
	    fdiv  dos 
	    fstp  [xsi].meany 
	endm  

Method SmplPlot.BuscaMaximos, uses xsi xbx 
	local v1 : dword, v1tope : dword 
	local inicio : dword 
	local fregTLS()
	local fSlvTLS() 
	
	;DbgText "SmplPlot.BuscaMaximos"
	
	SetObject xsi 
	
	fld   [xsi].minmin 
	fst   [xsi].minimoy 
	fstp  [xsi].minimox 
	
	fld   [xsi].maxmax 
	fst   [xsi].maximoy 
	fstp  [xsi].maximox 
	
	mov   inicio, TRUE 
	
	lea xax,[xsi].BaseSeries

	OCall [xsi].BaseSeries::LinkedList.GetFirst 
	mov   xcx, xax 
	buscase1: 
	.if xcx != NULL
	    SetObject xax, LinkedList, [xcx].$Obj(SerieGraph).Puntos 
	    mov   xcx, xax 
	    _push xax 
	    OCall xax::LinkedList.GetFirst 
	    mov   xcx, xax 
	  buscase2: 
	    .if xcx != NULL 
	        SetObject xbx, Puntito, xcx 
	        comparaMaximo 
	        _pop  xax 
	        _push xax 
	        OCall xax::LinkedList.GetNext 
	        mov   xcx, xax 
	        jmp   buscase2 
	    .endif 
	    _pop  xax 
	    OCall [xsi].BaseSeries::LinkedList.GetNext 
	    mov   xcx, xax 
	    jmp   buscase1 
	.endif 
	@if [xsi].definition.miny fNE 0.0 || [xsi].definition.maxy fNE 0.0
	    fSlv8 [xsi].minimoy = [xsi].definition.miny
	    fSlv8 [xsi].maximoy = [xsi].definition.maxy
	@endif 


MethodEnd 

Method SmplPlot.selecciona, uses xsi xbx, cursorCoords : LPARAM 
	local RT : RECT 
	LOCAL A1 : DWORD, A2 : DWORD 
	LOCAL a1 : dword, a3 : dword, a4 : dword, v2 : dword 
	local hpen1 : HWND, hpen2 : HWND, hpen0 : HWND 
	
	local a1c : dword, a3c : dword 
	
	local q34 : POINTER 
	local v1 : dword, v1tope : dword 
	local inicio0 : dword 
	local estecolor : dword 
	local estepen : dword 
	local hPenv : HWND 
	
	local gx : real8, gy : real8 
	local distancia : real8 
	local cdx : dword, cdy :dword 
	local ssx : dword, ssy :dword, sss : dword 
	local value_c : real8, value_s : real8 
	
	local mousepoint : POINT 
	local guardaserie : POINTER, guardapunto : POINTER 
	local fSlvTLS() 
	local fregTLS() 
	
	SetObject xsi 
	
	fSlv8 value_s = 1.0e7 
	
	mov   xcx, cursorCoords 
	mov   edx, ecx 
	movzx ecx, cx         ; mx
	shr   edx, 16 
	movzx edx, dx         ; my
	
	mov   mousepoint.x, ecx 
	mov   mousepoint.y, edx 
	
	
	OCall [xsi].BaseSeries::LinkedList.GetFirst 
	mov   xcx, xax 
	buscase1e: 
	.if xcx != NULL 
	    mov   guardaserie, xcx 
	    SetObject xax, LinkedList, [xcx].$Obj(SerieGraph).Puntos 
	    mov   xcx, xax 
	    _push xax 
	    OCall xax::LinkedList.GetFirst 
	    mov   xcx, xax 
	  buscase2e: 
	    .if xcx != NULL 
	        .if inicio0 == TRUE 
	            _push xcx 
	            mov   guardapunto, xcx 
	
	            SetObject xbx, Puntito, xcx 
	
	            fld   [xsi].minimoy 
	            fsub  [xbx].y 
	            fmul  [xsi].escalay 
	            fiadd [xsi].posiy 
	            fist  a1 ; = (int) Math.floor(posiy + (minimoy - y)* escalay);
	            fisub mousepoint.y 
	            fabs  
	            fist  cdy 
	
	            fld   [xbx].x 
	            fsub  [xsi].minimox 
	            fmul  [xsi].escalax 
	            fiadd [xsi].posix 
	    ;fiadd	[xsi].pxeje1
	            fist  a3 ; = (int) Math.floor(posix + ( tl -minimox) * escalax-escalax);
	            fisub mousepoint.x 
	            fabs  
	            fist  cdx 
	            fadd  
	            fsqrt 
	            fabs  
	            fstp  value_c 
	            .if fLT(value_c, value_s) 
	                _pop  xcx 
	                _pop  xcx 
	                _push xcx 
	                _push xcx 
	                fld   value_c 
	                fstp  value_s 
	                m2m   [xsi].ssx, a3, eax ;[ebx].x
	                m2m   [xsi].ssy, a1, eax ;[ebx].y
	                m2m   [xsi].ssp, guardapunto, xax 
	                m2m   [xsi].sss, guardaserie, xax 
	
	            .endif 
	            _pop  xcx 
	
	        .else 
	            mov   inicio0, TRUE 
	        .endif 
	
	        _pop  xax 
	        _push xax 
	        OCall xax::LinkedList.GetNext 
	        mov   xcx, xax 
	        jmp   buscase2e 
	    .endif 
	    _pop  xax 
	    OCall [xsi].BaseSeries::LinkedList.GetNext 
	    mov   xcx, xax 
	  ;_pop eax
	    jmp   buscase1e 
	.endif 
	

MethodEnd 


;##########################################################################################

Method SmplPlot.grilla, uses xsi, hDC : HWND, ini : DWORD, fin : DWORD 
	local TXT : RECT 
	local A1 : DWORD, A2 : DWORD 
	local a5 : dword, a3 : dword, a4 : dword, v2 : dword 
	local hpen1 : HWND, hpen2 : HWND, hpen0 : HWND 
	local q1 : REAL8 
	local v2R : REAL8, a5R : REAL8, afR : REAL8 
	local RTt : RECT 
	 ;local 	intervalo : REAL8 
	local raya : REAL8 
	local rangox : REAL8 
	local A2B : dword 
	
	local buffer[32] : CHR 
	
	local fSlvTLS() 
	
	SetObject xsi 
	if 1 
	    mov   eax, [xsi].pxeje1 
	    mov   [xsi].posix, eax ;	  = ax + 75;
	    mov   [xsi].RT.left, eax 
	
	    mov   eax, [xsi].total.right 
	    sub   eax, [xsi].pxeje2 
	    mov   [xsi].RT.right, eax 
	
	    mov   eax, [xsi].posiy 
	    sub   eax, [xsi].alto 
	    mov   [xsi].RT.top, eax 
	 ;add	eax , 50
	    mov   eax, [xsi].alto 
	    add   eax, [xsi].pxcabecera 
	 ;sub		eax , [xsi].pxpie
	    m2m   [xsi].RT.bottom, eax 
	endif 
	fld   [xsi].maximox 
	fsub  [xsi].minimox 
	 ;fadd		one
	 ;fisub	[xsi].eje1.right
	fild  [xsi].ancho 
	fdivr   ;escalax = ancho / (maximox - minimox);
	fstp  [xsi].escalax 
	;	DbgFloat [xsi].escalax
	
	fld   [xsi].maximoy 
	fsub  [xsi].minimoy 
	fild  [xsi].alto 
	 ;fisub	[xsi].pxcabecera
	
	 ;fisub	[xsi].cabecera.top
	 ;fisub	[xsi].pie.top
	 ;fisub	[xsi].pie.top
	 ;DbgDec [xsi].alto
	fdivr   ;escalax = ancho / (maximox - minimox);
	fstp  [xsi].escalay 
	
	 ;.if [xsi].mouseactivo== FALSE
	invoke FillRect, hDC, addr [xsi].RT, [xsi].BASE2_BRUSH 
	 ;.endif
	invoke CreatePen, PS_SOLID, 1, $RGB(98,98,98) 
	mov   hpen1, xax 
	invoke CreatePen, PS_DOT, 1, $RGB(128,128,128) 
	mov   hpen2, xax 
	invoke CreatePen, PS_SOLID, 1, $RGB(128,128,128) 
	mov   hpen0, xax 
	invoke SelectObject, hDC, hpen1 
	
	invoke SetBkMode, hDC, TRANSPARENT 
	
	invoke SetTextColor, hDC, $RGB(250,250,250)
	 
	@if [xsi].definition.grady fNE 0.0
	    fSlv4 a3 = round(([xsi].definition.maxy-[xsi].definition.miny)/[xsi].definition.grady)
	@else
	    mov   a3, 10
	@endif     
	mov   eax, [xsi].pxeje1 
	add   eax, [xsi].ancho 
	mov   a4, eax 
    ; grilla Horizontal
	
	ForLp v2, 0, a3, eax 
	    
	    finit 
	   ;fp_roundnearest
	    fild  [xsi].alto 
	   ;fisub	[xsi].pxcabecera
	    fidiv a3 
	    fimul v2 
	    fiadd [xsi].pxcabecera 
	   ;fiadd	[xsi].pxpie
	   ;fadd
	    fistp A1 
	    invoke MoveToEx, hDC, [xsi].pxeje1, A1, NULL 
	    invoke LineTo, hDC, a4, A1 
	
	    fld   [xsi].maximoy 
	    fld   st 
	    fsub  [xsi].minimoy 
	    fidiv  a3 
	    fimul v2 
	    fsub  
	    fstp  [xsi].escala0 
	
	    freal8$ [xsi].escala0 ;,8, 2
	
	    mov   eax, A1 
	    sub   eax, 15 
	    mov   TXT.top, eax 
	    add   eax, 30 
	    mov   TXT.bottom, eax 
	
	    mov   eax, [xsi].posix 
	    mov   TXT.right, eax 
	    sub   eax, 75 
	    mov   TXT.left, eax 
	
	    invoke DrawText, hDC, addr buffer, -1, addr TXT, DT_CENTER or DT_VCENTER or DT_SINGLELINE 
	
	Next  v2 
	
	invoke SelectObject, hDC, hpen2 
	
	fld   [xsi].minimox 
	fistp [xsi].inicio 
	
	fld   [xsi].maximox 
	fistp [xsi].final 
	
	mov   eax, [xsi].posiy 
	sub   eax, [xsi].alto 
	sub   eax, [xsi].RT.top 
	mov   a4, eax 
	
	   ;DbgFloat [xsi].escalax
	mov   eax, [xsi].posiy 
	sub   eax, [xsi].RT.top 
	mov   a3, eax 
	
	mov   eax, [xsi].inicio 
	   ;sub		eax , 1
	mov   a5, eax 
	
	fild  a5 
	fstp  a5R 
	
	fild  [xsi].final 
	fstp  afR 
	   ;DbgFloat  a5R
	   ;DbgFloat  afR
	fld   [xsi].maximox 
	fsub  [xsi].minimox 
	fstp  rangox 
	
	.if fLT( rangox, 11.0) 
	
	    fSlv8 [xsi].intervalo = ([xsi].maximox -[xsi].minimox)/10.0 
	;		f2f raya , [xsi].intervalo
	;		mov A2B, true
	.else 
	
	    fSlv8 [xsi].intervalo = [xsi].escalam ;1.0
	;		f2f raya , [xsi].escalam
	;		mov	A2B, false	
	.endif 
	
	ForLpR v2R, [xsi].minimox, [xsi].maximox, [xsi].intervalo   ;a5R, afR, 1.00
	
	  ;	//if (((v2 - inicio + 1) - Math.floor((v2 - inicio +1)	/ escalam)* escalam) == 0){
	   ;if (((v2 - inicio + 1) % escalam ) == 0){
	
	   ;					(int) Math.floor(posiy),
	
	    finit 
	    fld   v2R 
	    fsub  [xsi].minimox 
	    fld   st 
	    fmul  [xsi].escalax 
	  ;fsub		[xsi].escalax
	    fiadd [xsi].pxeje1 
	    Fistp A1 ;	(int) Math.floor(posix + (v2 - inicio + 1)* escalax), 
	
	    fld   st 
	    fdiv  raya 
	    fInt  
	    fmul  raya 
	    fsub  
	    fistp A2 
	
	    invoke MoveToEx, hDC, A1, [xsi].total.top, NULL 
	    invoke LineTo, hDC, A1, [xsi].RT.bottom ;a4
	
	    fld   v2R 
	    fRnd  
	    fstp  [xsi].escala0 
	    freal8$ [xsi].escala0 
	
	    mov   eax, [xsi].total.top 
	    mov   RTt.top, eax 
	    add   eax, [xsi].pxcabecera 
	    mov   RTt.bottom, eax 
	   ;sub		eax , 50
	    mov   eax, A1 
	    sub   eax, 15 
	    mov   RTt.left, eax 
	    add   eax, 30 
	    mov   RTt.right, eax 
	
	  ;	invoke DrawText, hDC, addr [xsi].stra1, -1, addr [xsi].RT ,DT_CENTER or DT_VCENTER or DT_SINGLELINE
	    invoke DrawText, hDC, addr buffer, -1, addr RTt, DT_CENTER or DT_VCENTER or DT_SINGLELINE 
	     ;(int) Math.floor(posix + (v2 - inicio + 1) * escalax-10),
	   ;(int) Math.floor(posiy-alto-10)
	  ;.endif
	
	NextR v2R 
	
	 ;invoke Rectangle, hDC, RT.left, RT.bottom, RT.right, RT.top
	
	OCall xsi.PoneTitulo, hDC 
	
	OCall xsi.PoneUnidadY, hDC 
	
	OCall xsi.PoneSeries, hDC 
	
	invoke DeleteObject, hpen0 
	invoke DeleteObject, hpen1 
	invoke DeleteObject, hpen2 
	
	xor   eax, eax 
MethodEnd 

Method SmplPlot.OnCreate, uses xsi, wParam : WPARAM, lParam : LPARAM 
	DbgText "SmplPlot.OnCreate" 
	SetObject xsi 
	
	invoke GetStockObject, SYSTEM_FONT  ;ANSI_FIXED_FONT;DEFAULT_GUI_FONT;OEM_FIXED_FONT	;SYSTEM_FIXED_FONT;  	; 
	mov   [xsi].hFntGUI, xax 
	
	invoke DefWindowProc, [xsi].hWnd, WM_CREATE, wParam, lParam 
	invoke SendMessage, [xsi].hWnd, WM_SETFONT, [xsi].hFntGUI, TRUE 
	
	mov   [xsi].BLUE_BRUSH, $invoke( CreateSolidBrush , $RGB(0,0,128)) 
	mov   [xsi].BASE2_BRUSH, $invoke( CreateSolidBrush , $RGB(0,0,0)) 
	mov   [xsi].EJES1_BRUSH, $invoke( CreateSolidBrush , $RGB(128,128,128)) 
	
	mov   [xsi].ColorSeries, $invoke( CreateSolidBrush , $RGB(0,255,0)) 
	mov   [xsi+XWORD].ColorSeries, $invoke( CreateSolidBrush , $RGB(255,0,0)) 
	mov   [xsi+XWORD*2].ColorSeries, $invoke( CreateSolidBrush , $RGB(0,255,255)) 
	mov   [xsi+XWORD*3].ColorSeries, $invoke( CreateSolidBrush , $RGB(255,0,255)) 
	mov   [xsi+XWORD*4].ColorSeries, $invoke( CreateSolidBrush , $RGB(255,128,128)) 
	mov   [xsi+XWORD*5].ColorSeries, $invoke( CreateSolidBrush , $RGB(255,255,255)) 
	
	mov   [xsi].ColorBas, $RGB(0,255,0) 
	mov   [xsi+DWORD].ColorBas, $RGB(255,0,0) 
	mov   [xsi+DWORD*2].ColorBas, $RGB(0,255,255) 
	mov   [xsi+DWORD*3].ColorBas, $RGB(255,0,255) 
	mov   [xsi+DWORD*4].ColorBas, $RGB(255,128,128) 
	mov   [xsi+DWORD*5].ColorBas, $RGB(255,255,255) 


MethodEnd 


Method SmplPlot.PoneTitulo, uses xsi, hDC : HWND 

	local RTl : RECT 
	
	SetObject xsi 
	
	fSlv4 RTl.left = [xsi].pxeje1 
	
	fSlv4 RTl.right = [xsi].pxeje1 + ([xsi].ancho *0.35) 
	   ;fSlv4 RT.top = [xsi].posix +v1*30 
	
	fSlv4 RTl.top = [xsi].total.bottom - [xsi].pxpie 
	fSlv4 RTl.bottom = [xsi].total.bottom 
	
	invoke DrawText, hDC, [xsi].titulo, -1, addr RTl, DT_CENTER or DT_VCENTER or DT_SINGLELINE 
	
MethodEnd 

Method SmplPlot.PoneUnidadY, uses xsi, hDC : HDC 
	;	ofIg.setColor(Color.white);
	;		ofIg.drawString(UnidadY, (int) Math.round(posix - 65),
	;				(int) Math.round(posiy + 20));
	local RTl : RECT 
	
	SetObject xsi 
	mov   RTl.left, 0 
	fSlv4 RTl.right = [xsi].pxeje1 
	fSlv4 RTl.top = [xsi].total.bottom - ([xsi].pxpie)*3/4-2 
	fSlv4 RTl.bottom = [xsi].total.bottom - ([xsi].pxpie)/4+2 
	
	invoke DrawText, hDC, [xsi].unidady, -1, addr RTl, DT_CENTER or DT_VCENTER or DT_SINGLELINE 

MethodEnd 

Method SmplPlot.PoneSeries, uses xsi, hDC : HWND 
	
	local v1 : dword 
	local RTl : RECT
	local ancho1 : dword, ancho2 : dword, ancho3 : dword 
	
	SetObject xsi 
	.if [xsi].definition._Series
	
	    fSlv4 ancho1 = [xsi].ancho * (1-0.6/[xsi].definition.nseries) 
	    fSlv4 ancho2 = [xsi].pxeje1 + ([xsi].ancho-ancho1)/2
	    fSlv4 ancho3 = ancho1/[xsi].definition.nseries 
	    ForLpD v1, 0, [xsi].definition.nseries, eax 
	
	        fSlv4 RTl.left = ancho2 + ancho3*v1 
	        fSlv4 RTl.right = RTl.left + 10 
	        fSlv4 RTl.top = [xsi].total.bottom - ([xsi].pxpie)*2/3 
	        fSlv4 RTl.bottom = [xsi].total.bottom - ([xsi].pxpie)/3 
	        invoke FillRect, hDC, addr RTl, [xsi].EJES1_BRUSH  
	
	        inc   RTl.left 
	        dec   RTl.right 
	        inc   RTl.top 
	        dec   RTl.bottom 
	
	        mov   eax, v1 
	        invoke FillRect, hDC, addr RTl, [xsi+xax*XWORD].ColorSeries 
	
	        fSlv4 RTl.left = RTl.left + 15 
	        fSlv4 RTl.right = ancho2 + ancho3*(v1+1)-3 
	        fSlv4 RTl.top = [xsi].total.bottom - ([xsi].pxpie)*3/4-2 
	        fSlv4 RTl.bottom = [xsi].total.bottom - ([xsi].pxpie)/4+2 
	
	        mov   __ecx, v1
	        inc   __ecx
	        mov   _edx, [xsi].definition._Series 
	        invoke DrawText, hDC, [_edx+_ecx*XWORD], -1, addr RTl, DT_VCENTER or DT_SINGLELINE 
	    Next  v1 
	.endif 
MethodEnd 

Method SmplPlot.InitSerie, uses xsi xcx, xprior : dword, dcolor : dword, xmod : dword 
	local fregTLS() 
	
	SetObject xsi 
	
	;DbgText "Inicia serie 3" 
	New   SerieGraph 
	_push xax 
	;mov   xcx, xax 
	;DbgHex xax, "           Inicia serie 3"
	OCall xax::SerieGraph.Init, xprior, dcolor, xmod        ;Initialize Puntito instance values
	_pop  xax 
	_push xax 

    ;DbgHex xax,"   en InitSerie"
	OCall [xsi].BaseSeries::LinkedList.Append, xax
	lea xax,[xsi].BaseSeries
	;DbgHex xax , "   BaseSeries"
	_pop  xax 

MethodEnd 
if 0 
    MethodO SmplPlot.InitSerie, uses xsi xcx, xprior : dword, xcolor : dword, xmod : dword, xdot : dword 

    SetObject xsi 
    _push xsi 
    New   SerieGraph 
    _push xax 
    mov   xcx, xax 
    OCallO xcx::SerieGraph.Init, xprior, xmod, xcolor, xdot    ;Initialize Puntito instance values
    _pop  xcx 
    _pop  xsi 
    _push xcx 
    OCall [xsi].BaseSeries::LinkedList.Append, xcx 
    _pop  xax 

    MethodEnd 
endif 
; Grafica un dato en particular, con lineas a los ejes
Method SmplPlot.graficaSdat, uses xsi xbx, memdc :HDC 
	LOCAL a1 : dword, a3 : dword, a1c : dword, a3c : dword 
	local hPenv : HWND, hpen0 : HWND 
	local TXT : RECT 
	local buffer[32] : CHR 
	local fregTLS() 
	
	SetObject xsi 
	
	invope xsi, CreatePen, PS_SOLID, 1, $RGB(255,255,255)   ;estecolor;[ecx].Color
	mov   hpen0, xax 
	invope xsi, SelectObject, memdc, xax   ;estepen
	mov   hPenv, xax 
	_push xsi 
	
	invope xsi, MoveToEx, memdc, [xsi].ssx, [xsi].RT.top, NULL 
	invope xsi, LineTo, memdc, [xsi].ssx, [xsi].RT.bottom 
	
	invope xsi, MoveToEx, memdc, [xsi].pxeje1, [xsi].ssy, NULL 
	mov   eax, [xsi].pxeje1 
	add   eax, [xsi].ancho 
	invope xsi, LineTo, memdc, eax, [xsi].ssy 
	
	mov   eax, [xsi].ssx 
	sub   eax, 4 
	mov   a3, eax 
	add   eax, 8 
	mov   a3c, eax 
	
	mov   eax, [xsi].ssy 
	sub   eax, 4 
	mov   a1, eax 
	add   eax, 8 
	mov   a1c, eax 
	
	_pop  xsi 
	
	invope xsi, Ellipse, memdc, a3, a1, a3c, a1c 
	
	mov   xdx, [xsi].ssp 
	freal8$ [xdx].$Obj(Puntito).x 
	
	labelHeight equ 30 
	labelWidth equ 100 
	
	mov   ecx, [xsi].ssy 
	.if ecx > 150 
	    mov   eax, [xsi].total.top 
	    add   eax, [xsi].pxcabecera ;RT.top
	    add   eax, 5 
	    mov   TXT.top, eax 
	    add   eax, labelHeight 
	    mov   TXT.bottom, eax 
	.else 
	    mov   eax, [xsi].RT.bottom 
	    sub   eax, 5 
	    mov   TXT.bottom, eax 
	    sub   eax, labelHeight 
	    mov   TXT.top, eax 
	.endif 
	
	mov   ecx, [xsi].ssx 
	mov   edx, [xsi].pxeje1 
	add   edx, [xsi].ancho 
	sub   edx, labelWidth/2 
	mov   ebx, [xsi].pxeje1 
	add   ebx, labelWidth/2 
	.if ecx <  ebx 
	    mov   ebx, [xsi].pxeje1 
	    mov   TXT.left, ebx 
	    add   ebx, labelWidth 
	    mov   TXT.right, ebx 
	.elseif ecx > edx 
	    mov   edx, [xsi].pxeje1 
	    add   edx, [xsi].ancho 
	    mov   TXT.right, edx 
	    sub   edx, labelWidth ;/2-5
	    mov   TXT.left, edx 
	.else 
	    add   ecx, labelWidth/2 
	    mov   TXT.right, ecx 
	    sub   ecx, labelWidth 
	    mov   TXT.left, ecx 
	.endif 
	
	invope xsi, RoundRect, memdc, TXT.left, TXT.top, TXT.right, TXT.bottom, 14, 14 
	invope xsi, DrawText, memdc, addr buffer, -1, addr TXT, DT_CENTER or DT_VCENTER or DT_SINGLELINE 
	
	mov   xdx, [xsi].ssp 
	freal8$ [xdx].$Obj(Puntito).y 
	
	mov   ecx, [xsi].ssy 
	sub   ecx, labelHeight/2 
	mov   TXT.bottom, ecx 
	add   ecx, labelHeight 
	mov   TXT.top, ecx 
	
	mov   ecx, [xsi].ssx 
	.if ecx > 350 
	    mov   eax, [xsi].pxeje1 
	    add   eax, 5 
	    mov   TXT.left, eax 
	    add   eax, labelWidth 
	    mov   TXT.right, eax 
	.else 
	    mov   eax, [xsi].pxeje1 
	    add   eax, [xsi].ancho 
	    sub   eax, 5 
	    mov   TXT.right, eax 
	    sub   eax, labelWidth 
	    mov   TXT.left, eax 
	.endif 
	invope xsi, RoundRect, memdc, TXT.left, TXT.top, TXT.right, TXT.bottom, 14, 14 
	invope xsi, DrawText, memdc, addr buffer, -1, addr TXT, DT_CENTER or DT_VCENTER or DT_SINGLELINE 
	
	.if [xsi].definition._Seried
		mov   ecx, [xsi].ssy 
		.if ecx > 150 
		    sub   ecx, 90 
		.else 
		    add   ecx, 60 
		.endif 
		_push ecx 
		
		mov   ecx, [xsi].ssx 
		.if ecx > 350 
		    sub   ecx, 50 
		.else 
		    add   ecx, 150 
		.endif 
		mov   TXT.right, ecx 
		sub   ecx, 100 
		mov   TXT.left, ecx 
		
		_pop  eax 
		sub   eax, 15 
		mov   TXT.top, eax 
		add   eax, 30 
		mov   TXT.bottom, eax 
		
		invope xsi, RoundRect, memdc, TXT.left, TXT.top, TXT.right, TXT.bottom, 14, 14 
		
		mov   xdx, [xsi].sss 
		invope xsi, DrawText, memdc, [xdx].$Obj(SerieGraph).pName, -1, addr TXT, DT_CENTER or DT_VCENTER or DT_SINGLELINE 
	.endif
	invope xsi, SelectObject, memdc, hPenv 
	invope xsi, DeleteObject, hpen0 
MethodEnd 

Method SmplPlot.Clear, uses xsi 

	local v1 : dword 
	local fregTLS() 
	
	SetObject xsi 
	
	_push xsi 
	
	OCall [xsi].BaseSeries::LinkedList.GetFirst 
	mov   xcx, xax 
	buscase1c: 
	.if xcx != NULL 
	    _push xcx 
	    OCall xcx::SerieGraph.Done 
	    _pop  xcx 
	
	    OCall [xsi].BaseSeries::LinkedList.GetNext 
	    mov   xcx, xax 
	
	    jmp   buscase1c 
	.endif 
	
	_pop  xsi 
	
MethodEnd 
	
Method SmplPlot.OnBkground,, wParam : WPARAM, lParam : LPARAM 
	
	xor   eax, eax 
MethodEnd 

Method SmplPlot.Done, uses xsi 
	local v1 : xword 
	local fregTLS() 
	
	    ;DbgText "SmplPlot.Done"
	
	SetObject xsi 
	    ;comment Ç
	
	_push xsi 
	
	 ;OCall xsi.Clear
	
	;	invoke SendMessage, [xsi].hWnd, WM_COMMAND, WM_USER,0
	
	_pop  xsi 
	_push xsi 
	 ;invoke SendMessage, [xsi].hWnd, WM_CLOSE,0,0
	   ;DbgText "Ahora limìa el resto"
	
	ForLp v1, 0, 5 
	    mov   xcx, v1 
	    _pop  xsi 
	    _push xsi 
	    invoke DeleteObject, [xsi].ColorSeries[xcx*XWORD] 
	    _pop  xsi 
	    _push xsi 
	Next  v1 
	_pop  xsi 
	
	OCall [xsi].BaseSeries::LinkedList.Done 
	
	invoke DeleteObject, [xsi].BLUE_BRUSH 
	invoke DeleteObject, [xsi].BASE2_BRUSH 
	invoke DeleteObject, [xsi].EJES1_BRUSH 
	
	ACall xsi.Done 
	
MethodEnd 

Method SmplPlot.InitPlot, uses xsi 
	SetObject xsi 
	
	OCall [xsi].BaseSeries::LinkedList.Clear 

MethodEnd
 
include tipoS.inc
