; ==================================================================================================
; Title:   TipoS.inc
; Author:  Héctor S. Enrique - January 2026
; Version: 1.0.0
; Purpose: Simple graphic implementation 
; Notes:    Framework version C.2.0, 2026
;               - First release.
; ==================================================================================================
Method SmplPlot.graficaS, uses xsi xdi xbx, hDC:HWND
 	local inicio0 : dword
	local estebrush:HWND
	local estepen:HWND
	local estedot:HWND
	local estecolor:HWND
    local hPenv:HWND
    local hBrshv:HWND
    local fregTLS()
    
   	SetObject xsi

    assume xdi : $ObjPtr(SerieGraph)
    SDLL_BeginLoop xsi,,SerieGraph, xdi 
		mov [xsi].dotp, 0
		m2m estecolor, [xdi].Color, xax 
		.if [xdi].Modo == SG_Modo_Punto	; Polygon
			invoke CreatePen, PS_SOLID, 1, $RGB(155,155,155)
			mov estepen, xax
		.else
			m2m estepen, [xdi].Pen, xax
		.endif
		m2m estebrush, [xdi].Brush, xax
		m2m [xsi].Dot, [xdi].Dot, xax

		m2m [xsi].modo, [xdi].Modo,xax
		m2m [xsi].activeColor, estecolor, xax
		m2m [xsi].activePen, estepen, xax
		m2m [xsi].activeBrush, estebrush, xax
		
		mov [xsi].yadibujo, FALSE

		invoke SelectObject, hDC, estepen
		mov hPenv, xax
		
		invoke SelectObject, hDC, estebrush
		mov hBrshv, xax
		;.if [xsi].arranque == 0
			mov inicio0 , TRUE
		;.else
			;mov inicio0 , FALSE
		;.endif
        assume xbx : $ObjPtr(Puntito)
        SDLL_BeginLoop xdi,,Puntito, xbx
			.if inicio0 == TRUE
		   		.if [xsi].modo == 0 ||[xsi].modo == 6||[xsi].modo == 7 
		   			OCall xsi.dibujaS, hDC, estecolor
				.else		   			
		   			OCall xsi.dibujaS, hDC, estebrush
		   		.endif
			.else
				mov inicio0 , TRUE
			.endif
        SDLL_EndLoop xbx		
		assume xbx:nothing

		invoke SelectObject, hDC, hPenv
		invoke SelectObject, hDC, hBrshv
		
		.if [xdi].Modo == SG_Modo_Punto		; Polygon
			invoke DeleteObject, estepen
		.endif
    SDLL_EndLoop xdi		
	assume xdi:nothing

MethodEnd

Method SmplPlot.dibujaS, uses xsi xbx, hDC:HWND, xcolor:HWND
   
    local RT:RECT
    LOCAL A1:DWORD, A2:DWORD
	LOCAL a1:dword, a3:dword, a4:dword, v2:dword    

	local a1c:dword, a3c:dword
	local a1b:dword, a3b:dword
	
	local q34:POINTER
	local fSlvTLS()
	
	local vt1:POINT
	local vt2:POINT
	local vt3:POINT
	local vt4:POINT
	
    local hBrush0:HBRUSH,hBrushV:HBRUSH

	local ScreenWidth: dword
	
	local a3R:real4
	
	;DbgText "dibujaS"
		
	;SetObject xbx, Puntito, _ecx
	assume xbx : $ObjPtr(Puntito)
	SetObject xsi
	
	fld		[xsi].minimoy
	fsub	[xbx].y
	fmul	[xsi].escalay
	fiadd	[xsi].posiy

	fistp	a1; = (int) Math.floor(posiy + (minimoy - y)* escalay);
	
	fld		[xbx].x
	fsub	[xsi].minimox
	fmul	[xsi].escalax
	fiadd	[xsi].posix
	;fiadd	[xsi].pxeje1
	fistp	a3; = (int) Math.floor(posix + ( tl -minimox) * escalax-escalax);
	
	mov ecx, [xsi].modo
	.switch ecx
	
		.case SG_Modo_Linea	;	Linea
				
			mov		eax , a3
			add		eax , 7
			mov		a3c , eax;= a3-1;
			mov		eax , a1
			add 	eax , 17
			mov		a1c , eax;=a1-1;
		
			m2m	RT.left   , a3, eax
			m2m	RT.bottom , a1c, eax
			m2m	RT.right  , a3c, eax
			m2m	RT.top    , a1, eax
	
			.if [xsi].yadibujo == FALSE
				
				.if fEQ( [xbx].y, -1 )
					mov [xsi].yadibujo, FALSE
				.else				
					mov [xsi].yadibujo, TRUE
					invoke MoveToEx, hDC, a3, a1 , NULL
				.endif
			.else
				.if fEQ( [xbx].y,-1.0)
					mov [xsi].yadibujo, FALSE
				.else				
					invoke LineTo, hDC, a3 , a1
				.endif
			.endif
	
		.case SG_Modo_LineaCortada	;	Linea
				
			mov	 eax , a3
			add  eax , 7
			mov  a3c , eax;= a3-1;
			mov  eax , a1
			add  eax , 17
			mov  a1c , eax;=a1-1;
		
			m2m	  RT.left   , a3, eax
			m2m	  RT.bottom , a1c, eax
			m2m	  RT.right  , a3c, eax
			m2m	  RT.top    , a1, eax
	
			.if [xsi].yadibujo == FALSE
				
				.if fEQ( [xbx].y, -1 )
					mov [xsi].yadibujo, FALSE
				.else				
					mov [xsi].yadibujo, TRUE
					invoke MoveToEx, hDC, a3, a1 , NULL
				.endif
			.else
				.if fEQ( [xbx].y,-1.0)
					mov [xsi].yadibujo, FALSE
				.else				
					invoke LineTo, hDC, a3 , a1
				.endif
			.endif

		.case SG_Modo_Rectangle	
	
			sub	  a3 , 2
			mov	  eax , a3
			add	  eax , 5
			mov	  a3c , eax;= a3-1;
			mov	  eax , a1
			sub	  eax , 2
			mov	  a1, eax
			add	  eax , 5 
			mov	  a1c, eax;=a1-1;
			
			m2m	  RT.left , a3, eax
			m2m	  RT.bottom , a1c, eax
			m2m	  RT.right, a3c, eax
			m2m	  RT.top , a1, eax
		
		    invoke FillRect, hDC, addr RT, xcolor
		
		.case SG_Modo_DoubleCircle
	
			mov	  eax , a3
			add	  eax , 10
			mov	  a3c , eax;= a3-1;
			mov	  eax , a1
			sub	  eax , 5
			mov	  a1, eax
			add	  eax , 10 
			mov	  a1c, eax;=a1-1;
			
			m2m	  RT.left , a3,eax
			m2m	  RT.bottom , a1c,eax
			m2m	  RT.right, a3c,eax
			m2m	  RT.top , a1,eax
		
		    invoke Ellipse, hDC, a3,a1,a3c,a1c
	
			invoke CreateSolidBrush, $RGB(0,0,0);estecolor;[ecx].Color
			mov hBrush0, xax
			invoke SelectObject, hDC, xax
			mov hBrushV, xax
	
		    add   a3 , 3
		    add   a1 , 3
		    sub   a1c , 3
		    sub   a3c , 3
		    invoke Ellipse, hDC, a3,a1,a3c,a1c
	
			invoke SelectObject, hDC, hBrushV
			invoke DeleteObject, hBrush0
			
		.case 4	; Circle
		
			sub	  a3 , 3
			mov	  eax , a3
			add	  eax , 7
			mov	  a3c , eax;= a3-1;
			mov	  eax , a1
			sub	  eax , 3
			mov	  a1, eax
			add	  eax , 7 
			mov	  a1c, eax;=a1-1;
			
			m2m	  RT.left , a3, eax
			m2m	  RT.bottom , a1c, eax
			m2m	  RT.right, a3c, eax
			m2m	  RT.top , a1, eax
		
		    invoke Ellipse, hDC, a3,a1,a3c,a1c
		
		.case 5; Diamante
		
			mov	  eax, a3	
			sub	  eax , 4
			mov	  a3b, eax
			add	  eax , 8
			mov	  a3c , eax;= a3-1;
	
			mov	  eax , a1
			sub	  eax , 4
			mov	  a1b, eax
			add	  eax , 8 
			mov	  a1c, eax;=a1-1;
			
			m2m	  vt1.x, a3b, eax
			m2m	  vt1.y, a1, eax
			m2m	  vt2.x, a3, eax
			m2m	  vt2.y, a1b, eax
			m2m	  vt3.x, a3c, eax
			m2m	  vt3.y, a1, eax
			m2m	  vt4.x, a3, eax
			m2m	  vt4.y, a1c, eax
		
		    invoke Polygon, hDC, addr vt4, 4
		
		.case SG_Modo_CircleandLine
	
			sub	  a3 , 5
			mov	  eax , a3
			add	  eax , 10
			mov	  a3c , eax;= a3-1;
			mov	  eax , a1
			sub	  eax , 5
			mov	  a1, eax
			add	  eax , 10 
			mov	  a1c, eax;=a1-1;
			
			m2m	  RT.left , a3, eax
			m2m	  RT.bottom , a1c, eax
			m2m	  RT.right, a3c, eax
			m2m	  RT.top , a1, eax
		
		    invoke Ellipse, hDC, a3,a1,a3c,a1c
	
			invoke CreateSolidBrush, $RGB(0,0,0);estecolor;[ecx].Color
			mov hBrush0, xax
			invoke SelectObject, hDC, xax
			mov hBrushV, xax
	
		    add   a3 , 3
		    add   a1 , 3
		    sub   a1c , 3
		    sub   a3c , 3
		    invoke Ellipse, hDC, a3,a1,a3c,a1c
	
			invoke SelectObject, hDC, hBrushV
			invoke DeleteObject, hBrush0
			.if [xsi].yadibujo == FALSE
				.if fEQ( [xbx].y, -1.0 )
					mov [xsi].yadibujo, FALSE
				.else				
					mov [xsi].yadibujo, TRUE
				.endif
			.endif
			
		.case SG_Modo_LineaDC	;	Linea
				
			mov	  eax , a3
			add	  eax, 7
			mov	  a3c , eax;= a3-1;
			mov	  eax , a1
			add   eax, 17
			mov	  a1c, eax;=a1-1;
		
			m2m	  RT.left , a3, eax
			m2m	  RT.bottom , a1c, eax
			m2m	  RT.right, a3c, eax
			m2m	  RT.top , a1, eax
	
			.if [xsi].yadibujo == FALSE
				.if fEQ( [xbx].y, -1 )
					mov [xsi].yadibujo, FALSE
				.else				
					mov [xsi].yadibujo, TRUE
					invoke MoveToEx, hDC, a3, a1 , NULL
				.endif
			.else
				.if fEQ( [xbx].y,-1.0)
					mov [xsi].yadibujo, FALSE
				.else				
					invoke LineTo, hDC, a3 , a1
				.endif
			.endif
	.endsw	
	assume xbx:nothing
MethodEnd
