; ==================================================================================================
; Title:   SmplPlot_OOP.inc
; Author:  Héctor S. Enrique - January 2026
; Version: 1.0.0
; Purpose: Simple graphic implementation 
; Notes:    Framework version C.2.0, 2026
;               - First release.
; ==================================================================================================
CLSSTYLE equ CS_BYTEALIGNWINDOW or CS_BYTEALIGNCLIENT or CS_VREDRAW or CS_HREDRAW; or CS_DROPSHADOW

SG_Modo_Punto			equ	0   ; se perdió ??
SG_Modo_Linea			equ	1
SG_Modo_Rectangle		equ	2
SG_Modo_DoubleCircle	equ 3
SG_Modo_Circle			equ 4
SG_Modo_Diamante		equ 5
SG_Modo_Jewel			equ	0
SG_Modo_Esfera			equ	6
SG_Modo_Cub				equ	7
SG_Modo_CircleandLine	equ 8
SG_Modo_LineaDC			equ	9

SG_Modo_LineaCortada	equ	15

GD_Mask_miny    equ     1h
GD_Mask_maxy    equ     2h
GD_Mask_minx    equ     4h
GD_Mask_maxx    equ     8h

SmplSeriesDef struct
    _X      POINTER  0
    _Y1     POINTER  0
    _Y2     POINTER  0
    _Y3     POINTER  0
    _Y4     POINTER  0
    _Y5     POINTER  0
    _Y6     POINTER  0
SmplSeriesDef ends     
SmplGraphDef struct
    minx    REAL8  0.0
    maxx    REAL8  0.0
    miny    REAL8  0.0
    maxy    REAL8  0.0
    grady   REAL8  0.0
    nseries dword  0
;    _Next   POINTER 0
    _Title  POINTER 0
    _Series POINTER 0
    _Seried POINTER 0
    _X      dword  0
    _Y1     dword  0
    _Y2     dword  0
    _Y3     dword  0
    _Y4     dword  0
    _Y5     dword  0
    _Y6     dword  0
    _Mask   dword  0
SmplGraphDef ends

.data

PUNTITO struct
	x	QWORD	?
	y	QWORD	?
PUNTITO ends

Object Puntito,, Primer
	DefineVariable	x, real8, 0.0
	DefineVariable	y, real8, 0.0
	RedefineMethod	Init,QWORD, QWORD
ObjectEnd

Object L_Serie,,Primer
	DefineVariable  xName, CHR, 32 dup (0)
	DefineVariable  Color, dword, 0	
	DefineVariable  Brush, dword, 0	
	DefineVariable  Pen, dword, 0	
	DefineVariable  Modo, dword, 0	
ObjectEnd

Hit_vertical    equ  0
Hit_horizontal  equ  1

Object Hit,, Puntito
    DefineVariable  k, DWORD, 0 
    ;RedefineMethod  Init, QWORD, QWORD, DWORD
ObjectEnd

Object L_Legend,,Primer
	Embed series, LinkedList
ObjectEnd

Object SerieGraph, ,Primer
	DefineVariable  pName, POINTER, 0
	DefineVariable  Prioridad, dword, 0	
	DefineVariable  Color, dword, 0	
	DefineVariable  Brush, HWND, 0	
	DefineVariable  Pen, HWND, 0	
	DefineVariable  Modo, dword, 0	
	DefineVariable  Dot, dword, 0	

	Embed			Puntos, LinkedList	

	DefineVariable	maximox, REAL8, 360.0
	DefineVariable	maximoy, REAL8, 80.0
	DefineVariable	minimox, REAL8, 0.0
	DefineVariable	minimoy, REAL8, 0.0
	
  	RedefineMethod  Done
  	RedefineMethod  Init, DWORD, DWORD, DWORD    ;-> Prioridad, Color, Modalidad
 
  	VirtualMethod  CargaPunto, REAL8, REAL8    ;-> x, y
ObjectEnd

Object SmplPlot, ,  Window                    ;Single Document Interface App.
  	RedefineMethod  Done
  	RedefineMethod  Init,         POINTER, HWND       ;-> Owner, Parent

    RedefineMethod  OnSize,  WPARAM, LPARAM

	StaticMethod	Startup

	;Embed puntosC, LinkedList

	DefineVariable 	serieS, POINTER, 6 dup (NULL)

	;DefineVariable 	puntosC, POINTER, 6 dup (NULL)
	;DefineVariable 	puntosP, POINTER, 6 dup (NULL)
	DefineVariable 	etapasg, dword, 6 dup (0)	
  	
	;DefineVariable 	moscasC, dword, 0
	;DefineVariable 	moscasP, dword, 0
  
  	;VirtualMethod   dibuja, HWND
	VirtualMethod	grilla, HWND,DWORD,DWORD
	
	VirtualMethod  InitPlot
    
	VirtualEvent    OnCreate,	WM_CREATE
	VirtualEvent	OnPaint,    WM_PAINT
	VirtualEvent	OnClose,    WM_CLOSE
	VirtualEvent	OnCommand,	WM_COMMAND

	VirtualEvent	OnBkground,	WM_ERASEBKGND
		
	VirtualEvent	OnMouseDown, WM_LBUTTONDOWN
	VirtualEvent	OnMouseUp,    WM_LBUTTONUP
	VirtualEvent	OnMouseMove, WM_MOUSEMOVE
	
	
	DefineVariable	hFntGUI, HWND, NULL
	DefineVariable	BLUE_BRUSH, HWND, NULL
	DefineVariable	BASE2_BRUSH, HWND, NULL
	DefineVariable	EJES1_BRUSH, HWND, NULL
	
	DefineVariable	total , RECT, {}
	DefineVariable	grillaRT , RECT, {}
	DefineVariable	RT, RECT,{}
	;DefineVariable	cabecera , RECT, {}
	;DefineVariable	pie , RECT, {}
	;DefineVariable	eje1 , RECT, {}
	;DefineVariable	eje2 , RECT, {}

	DefineVariable	pxcabecera, DWORD,21
	DefineVariable	pxpie, dword, 36
	DefineVariable	pxeje1, dword, 75
	DefineVariable	pxeje2, dword,  10
		
	DefineVariable	ancho, DWORD, NULL
	DefineVariable	alto, DWORD, NULL
	DefineVariable	posix, DWORD, NULL
	DefineVariable	posiy, DWORD, NULL
	
	DefineVariable	inicio, DWORD, NULL
	DefineVariable	final, DWORD, NULL

	DefineVariable	escala0, REAL8, ?
	DefineVariable	escalax, REAL8, ?
	DefineVariable	escalay, REAL8, ?
	DefineVariable	escalam, REAL8, 30.0
	DefineVariable	intervalo, REAL8, 1.0

	DefineVariable	maximox, REAL8, 360.0
	DefineVariable	maximoy, REAL8, 80.0
	DefineVariable	minimox, REAL8, 0.0
	DefineVariable	minimoy, REAL8, 0.0
	DefineVariable	meanx, REAL8, 0.0
	DefineVariable	meany, REAL8, 0.0
	

	DefineVariable	calculado, BOOLEAN, FALSE
	
	DefineVariable 	punto, POINTER, NULL	
	DefineVariable	minmin, REAL8, 10000000000000000000.0
	DefineVariable	maxmax, REAL8, -10000000000000000000.0	

	DefineVariable 	ColorSeries, POINTER, 6 dup (NULL)
	DefineVariable 	ColorBas, DWORD, 6 dup (NULL)

	;VirtualMethod 	CargaPuntoP,DWORD,dword,REAL8,REAL8

	DefineVariable	arranque, dword , 0
	DefineVariable	arranqueP, dword , 0
	VirtualMethod 	BuscaMaximos

	;VirtualMethod 	graficaP,HDC
	;VirtualMethod 	dibujaP,HDC, dword
	
	DefineVariable 	stra1, byte, 80 dup (0)	
	
	DefineVariable 	serie, dword, 1

	;DefineVariable stra3, byte,  ("Que lo pario Mendieta")	

	;VirtualMethod	CargaPuntoC,DWORD,dword,REAL8,REAL8
	;VirtualMethod 	dibujaC,HDC
	;VirtualMethod 	graficaC,HDC
	
	VirtualMethod	PoneTitulo, HDC
;		PoneTitulo();
	VirtualMethod	PoneUnidadY, HDC 
	VirtualMethod	PoneSeries, HDC

	DefineVariable 	yadibujo, dword, FALSE

	DefineVariable	titulo, POINTER
	DefineVariable	tituloSerie, POINTER, 5 dup (NULL)
	DefineVariable	unidady, POINTER

	; ---- Multiples --------------------------------
	Embed			BaseSeries, LinkedList
  	VirtualMethod  	InitSerie, DWORD, DWORD, DWORD    ;-> Prioridad, Color, Modalidad

	VirtualMethod 	graficaS, HDC

	VirtualMethod 	graficaSdat, HDC
;	VirtualMethod 	graficaSesp, HDC

	VirtualMethod 	dibujaS,HDC, HWND
	
	DefineVariable  modo, dword, 0 		; store SerieGraph mode
	DefineVariable  activeColor, HWND, 0 		; store SerieGraph color
	DefineVariable  activePen, HWND, 0 		; store SerieGraph pen
	DefineVariable  activeBrush, HWND, 0 		; store SerieGraph brush
	
	VirtualMethod 	Clear
	; pizarron
	DefineVariable	hpizarra, dword, 0
	VirtualMethod	selecciona, LPARAM
	
	DefineVariable	ssx , dword, 0	;	guarda 	x
	DefineVariable	ssy , dword, 0	;			y
	DefineVariable	sss , POINTER, 0	;			serie
	DefineVariable	ssp , POINTER, 0	;			punto
	
	DefineVariable	mouseactivo, dword, 0
	DefineVariable	mousepoin, POINT, {}
	DefineVariable	especialactivo, dword, 0
	DefineVariable	especialpoin, dword, 0

	;---------------------------------------- 18/08/2017
	;DefineVariable 	moscasS, dword, 0
	Embed	legend, L_Legend
	
	DefineVariable lastx, dword, 0
	DefineVariable lasty, dword, 0

	DefineVariable  Dot, dword, 0	
	DefineVariable  dotp, dword, 0	; guarda puntero a SmplPlot
	DefineVariable  definition, SmplGraphDef, {} 
ObjectEnd                               ;Ends object definition

.data
.code