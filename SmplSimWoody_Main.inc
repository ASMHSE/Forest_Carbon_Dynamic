; ==================================================================================================
; Title:   SmplSimWoody_Main.inc
; Author:  Héctor S. Enrique @ January 2026
; Version: 1.0.0
; Purpose:  SmplSimWoody - Forest Carbon Dynamic Simulation
; ==================================================================================================
   
CLSSTYLE equ CS_BYTEALIGNWINDOW or CS_BYTEALIGNCLIENT or CS_VREDRAW or CS_HREDRAW; or CS_DROPSHADOW
   
CStr szSmplSimWoody,"SmplSimWoody"
   
Object SmplSimWoody , SmplSimWoodyID, SdiApp;      ;Single Document Interface App.
	RedefineMethod  Init         ;Init method refefinition
	RedefineMethod  Done   

	StaticMethod    Startup

	VirtualEvent    OnCreate,   WM_CREATE
	VirtualEvent    OnCommand,  WM_COMMAND
	RedefineMethod  OnSize,     WPARAM, LPARAM
	VirtualEvent    OnPaint,    WM_PAINT
	VirtualMethod   Execute
	VirtualMethod   SelectGraphDef
	VirtualMethod   Show

	DefineVariable	cualquiera, dword, 0
	DefineVariable	selector, POINTER, 0
	Embed           smplplot, SmplPlot
	Embed           woodymodel, WoodyCarbon
	Embed	        chMostrarA, ComboBox
	Embed	        chMostrarB, ComboBox
	Embed	        chMostrarC, ComboBox

	Embed           btRun, BUTTONW
	Embed           progressbar, Progressbar
	Embed           reRichCol, RichEditCol
	
	DefineVariable  MuestraA, xword, 0
	DefineVariable  MuestraB, xword, 1
	DefineVariable  MuestraC, xword, 0
	DefineVariable  lock1, xword, 0
	DefineVariable  showing, xword, 1
	
ObjectEnd
if IMPLEMENT
.data
    PROPCBSTYLE equ CBS_DROPDOWNLIST
    
	IDM_chMostrarA	equ	514
	PropCBMostrarA	DEF_COMBOBOX { IDM_chMostrarA, PROPCBSTYLE, 0, NULL,{ <0, 0, 0, 0>}}
	IDM_chMostrarB	equ	515
	PropCBMostrarB	DEF_COMBOBOX { IDM_chMostrarB, PROPCBSTYLE, 0, NULL,{ <0, 0, 0, 0>}}
	IDM_chMostrarC	equ	516
	PropCBMostrarC	DEF_COMBOBOX { IDM_chMostrarC, PROPCBSTYLE, 0, NULL,{ <0, 0, 0, 0>}}
	
	
    PROPBTSTYLE equ 0
    
    btRun$          db "Run",0
	IDM_btRUN 	    equ	517
	PropBTRun	    DEF_BUTTON{ IDM_btRUN, PROPBTSTYLE, 0, offset btRun$,{ <0, 0, 0, 0>}}

	PROGSTYLE equ WS_CHILD or WS_VISIBLE
	
	CStr txProgress, "Progress"
	IDM_Progress    equ 518 
	PropProgress    DEF_PROGRESSBAR {IDM_Progress,{<0,0,0,0>},0,100,10,PROGSTYLE, offset txProgress}

	PROPRESTYLE equ ES_MULTILINE or WS_VSCROLL or WS_HSCROLL; or WS_BORDER or WS_TABSTOP

    DeclareRichEdit RichCol, 519

	RtfFileName db 256 dup(0)
	str_extention_rtf db "rtf1",0

    lgfnt LOGFONT <15,0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0,"Consolas">


    graf0$ db "Density",0
	    ser0_0$ db "years",0
	    ser0_1$ db "Trees/ha",0
	    ser0_2$ db "Shrubs/ha/10",0
	    ser0_3$ db "R.trees/ha*10",0
	    ser0_4$ db "R.shrubs/ha/2",0
	    ser0   POINTER offset ser0_0$,offset ser0_1$,offset ser0_2$,offset ser0_3$,offset ser0_4$
	    sar0_1$ db "Trees",0
	    sar0_2$ db "Shrubs",0
	    sar0_3$ db "R.trees",0
	    sar0_4$ db "R.shrubs",0
	    sar0   POINTER offset sar0_1$,offset sar0_2$,offset sar0_3$,offset sar0_4$
    GraphDef0 SmplGraphDef {0.0,0.0,0.0,600.0,100.0,4,offset graf0$, offset ser0,offset sar0, 0, 9,30,31,34,0,0,NULL}

    graf1$ db "BasalArea",0
	    ser1_0$ db "years",0
	    ser1_1$ db "Basal Area (m^2 / ha)",0
   	    ser1   POINTER offset ser1_0$,offset ser1_1$
    GraphDef1 SmplGraphDef {0.0,0.0,0.0,10.0,0.0,1,offset graf1$,offset ser1,NULL, 0,14, 0, 0, 0,0,0,NULL}
    
    graf2$ db "Carbon Stock",0
	    ser2_0$ db "years",0
	    ser2_1$ db "Trees abv",0
	    ser2_2$ db "Trees blw",0
	    ser2_3$ db "Shrubs abv",0
	    ser2_4$ db "Shrubs blw",0
	    ser2_5$ db "Dead Roots",0
   	    ser2   POINTER offset ser2_0$,offset ser2_1$,offset ser2_2$,offset ser2_3$,offset ser2_4$,offset ser2_5$
    GraphDef2 SmplGraphDef {0.0,0.0,0.0,0.0,0.0,5,offset graf2$, offset ser2, (offset ser2+POINTER), 0, 3, 4, 1, 2,28,0,NULL}

    graf3$ db "Timber Production",0
    GraphDef3 SmplGraphDef {0.0,0.0,0.0,0.0,0.0,1,offset graf3$,NULL,NULL, 0, 7, 0, 0, 0,0,0,NULL}

    graf4$ db "Trees",0
	    ser4_0$ db "years",0
	    ser4_1$ db "Q.Blanco/ha",0
	    ser4_2$ db "Q.Colorado/ha",0
	    ser4_3$ db "Mistol/ha",0
	    ser4_4$ db "A.Negro/ha",0
	    ser4   POINTER offset ser4_0$,offset ser4_1$,offset ser4_2$,offset ser4_3$,offset ser4_4$
	    sar4_1$ db "Q.Bl.",0
	    sar4_2$ db "Q.Col.",0
	    sar4_3$ db "Mist.",0
	    sar4_4$ db "Alg.",0
	    sar4   POINTER offset sar4_1$,offset sar4_2$,offset sar4_3$,offset sar4_4$
    GraphDef4 SmplGraphDef {0.0,0.0,0.0,0.0,0.0,4,offset graf4$,offset ser4,offset sar4, 0,19,20,21,22,0,0,NULL}

    graf5$ db "Shrub Mean Age",0
    GraphDef5 SmplGraphDef {0.0,0.0,0.0,0.0,0.0,2,offset graf5$,NULL,NULL, 0,17,18, 0, 0,0,0,NULL}
    
    graf6$ db "Stumps",0
    GraphDef6 SmplGraphDef {0.0,0.0,0.0,100.0,10.0,1,offset graf6$,NULL,NULL, 0, 27, 0, 0, 0,0,0,NULL}

    graf7$ db "DeadMeanAge",0
    GraphDef7 SmplGraphDef {0.0,0.0,0.0,0.0,0.0,1,offset graf7$,NULL,NULL, 0, 29, 0, 0, 0,0,0,NULL}

    graf8$ db "Trees Height",0
	    ser8_0$ db "years",0
	    ser8_1$ db "Q.Bl.",0
	    ser8_2$ db "Q.Col.",0
	    ser8_3$ db "Mist.",0
	    ser8_4$ db "Alg.",0
	    ser8   POINTER offset ser8_0$, offset ser8_1$,offset ser8_2$,offset ser8_3$,offset ser8_4$
    GraphDef8 SmplGraphDef {0.0,0.0,0.0,0.0,0.0,4,offset graf8$,offset ser8,(offset ser8+POINTER), 0,23,24,25,26,0,0,NULL}
    
    GraphDefs POINTER offset GraphDef0,offset GraphDef1,offset GraphDef2,offset GraphDef3,
                      offset GraphDef4,offset GraphDef5,offset GraphDef6,offset GraphDef7,
                      offset GraphDef8   

.code

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

Method SmplSimWoody.Startup
    local WC:WNDCLASSEX

  mov WC.cbSize, sizeof(WNDCLASSEX)
  mov WC.style, CLSSTYLE
  m2m WC.lpfnWndProc, $MethodAddr(SdiApp.WndProc), xax
  m2z WC.cbClsExtra
  m2z WC.cbWndExtra
  m2m WC.hInstance, hInstance, xax
  invoke LoadBitmap, hInstance, $OfsCStr("BMP_BACKGROUND")
  mov WC.hbrBackground, $invoke(CreatePatternBrush, xax)
  c2m WC.lpszMenuName, $OfsCStr("MENU_APP"), xax
  c2m WC.lpszClassName, offset szSmplSimWoody, xax
  mov WC.hIcon, $invoke(LoadIcon, hInstance, offset szIcoName)
  mov WC.hCursor, $invoke(LoadCursor, 0, IDC_ARROW)
  m2z WC.hIconSm

    invoke RegisterClassEx, addr WC
    
MethodEnd


; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:    SmplSimWoody.Init
; Purpose:   Initialize the SDI application object.
; Arguments: None.
; Return:    Nothing.

Method SmplSimWoody.Init, uses xsi
    local dWndPosX:DWORD, dWndPosY:DWORD, dWndWidth:DWORD, dWndHeight:DWORD

    SetObject xsi
    ACall xsi.Init

    ;invoke LoadCommonControls, ICC_COOL_CLASSES or ICC_BAR_CLASSES    ;Rebar & Statusbar
    
    mov dWndWidth, 640                                  ;Default creation width
    mov dWndHeight, 480                                 ;Default creation height
    mov dWndPosX, $32($invoke(CenterForm, dWndWidth, $32($invoke(GetSystemMetrics, SM_CXSCREEN))))
    mov dWndPosY, $32($invoke(CenterForm, dWndHeight, $32($invoke(GetSystemMetrics, SM_CYSCREEN))))

    invoke CreateWindowEx, WS_EX_LEFT or WS_EX_APPWINDOW, \
                           offset szSmplSimWoody, offset szAppTitle, WS_OVERLAPPEDWINDOW or WS_CLIPCHILDREN, \
                           dWndPosX, dWndPosY, dWndWidth, dWndHeight, NULL, NULL, hInstance, pSelf

    invoke ShowWindow, [xsi].hWnd, SW_SHOWNORMAL        ;Show the window
    invoke UpdateWindow, [xsi].hWnd                     ;Update if necessary

	OCall [xsi].woodymodel::WoodyCarbon.Init
	
	OCall xsi.SelectGraphDef
    
MethodEnd

Method SmplSimWoody.OnCreate, uses xbx xsi, wParam:WPARAM, lParam:LPARAM
	SetObject xsi

    OCall [xsi].smplplot::SmplPlot.Init, xsi, [xsi].hWnd

	OCall [xsi].chMostrarA::ComboBox.Init, xsi, [xsi].hWnd, offset PropCBMostrarA
		invoke SendMessage, [xsi].chMostrarA.hWnd, CB_ADDSTRING, 0, $OfsCStr("  Density            ")
		invoke SendMessage, [xsi].chMostrarA.hWnd, CB_ADDSTRING, 1, $OfsCStr("  Basal Area         ")
		invoke SendMessage, [xsi].chMostrarA.hWnd, CB_ADDSTRING, 2, $OfsCStr("  Carbon Stock       ")
		invoke SendMessage, [xsi].chMostrarA.hWnd, CB_ADDSTRING, 3, $OfsCStr("  Timber Production  ")
		invoke SendMessage, [xsi].chMostrarA.hWnd, CB_ADDSTRING, 4, $OfsCStr("  Tree Species       ")
		invoke SendMessage, [xsi].chMostrarA.hWnd, CB_ADDSTRING, 5, $OfsCStr("  Shrub Age          ")
		invoke SendMessage, [xsi].chMostrarA.hWnd, CB_ADDSTRING, 6, $OfsCStr("  Stumps             ")
		invoke SendMessage, [xsi].chMostrarA.hWnd, CB_ADDSTRING, 7, $OfsCStr("  Dead Roots Mean Age")
		invoke SendMessage, [xsi].chMostrarA.hWnd, CB_ADDSTRING, 8, $OfsCStr("  Trees Height       ")

		invoke SendMessage, [xsi].chMostrarA.hWnd, CB_SETCURSEL, [xsi].MuestraA, 0

	OCall [xsi].chMostrarB::ComboBox.Init, xsi, [xsi].hWnd, offset PropCBMostrarB
		invoke SendMessage, [xsi].chMostrarB.hWnd, CB_ADDSTRING, 0, $OfsCStr("  Not Apply         ")
		invoke SendMessage, [xsi].chMostrarB.hWnd, CB_ADDSTRING, 1, $OfsCStr("  Undisturbed       ")
		invoke SendMessage, [xsi].chMostrarB.hWnd, CB_ADDSTRING, 2, $OfsCStr("  Full Impact       ")
		invoke SendMessage, [xsi].chMostrarB.hWnd, CB_ADDSTRING, 2, $OfsCStr("  Low Impact        ")
		invoke SendMessage, [xsi].chMostrarB.hWnd, CB_ADDSTRING, 3, $OfsCStr("  False SP          ")

		invoke SendMessage, [xsi].chMostrarB.hWnd, CB_SETCURSEL, [xsi].MuestraB, 0

	OCall [xsi].chMostrarC::ComboBox.Init, xsi, [xsi].hWnd, offset PropCBMostrarC
		invoke SendMessage, [xsi].chMostrarC.hWnd, CB_ADDSTRING, 0, $OfsCStr("  Not Apply         ")
		invoke SendMessage, [xsi].chMostrarC.hWnd, CB_ADDSTRING, 1, $OfsCStr("  Stab. for Undisturbed/MBGI  ")
		invoke SendMessage, [xsi].chMostrarC.hWnd, CB_ADDSTRING, 2, $OfsCStr("  Stab. for False SP          ")
		invoke SendMessage, [xsi].chMostrarC.hWnd, CB_ADDSTRING, 2, $OfsCStr("  Comparison Means            ")

		invoke SendMessage, [xsi].chMostrarC.hWnd, CB_SETCURSEL, [xsi].MuestraC, 0

	OCall [xsi].btRun::ComboBox.Init, xsi, [xsi].hWnd, offset PropBTRun
	
	OCall [xsi].progressbar::Progressbar.Init, xsi, [xsi].hWnd, offset PropProgress
	invoke ShowWindow, [xsi].progressbar.hWnd, SW_HIDE
    m2m hProgressBar, [xsi].progressbar.hWnd, xax
    
    InitRichEdit RichCol, xsi
    invoke SendMessage, [xsi].reRichCol.hWnd, EM_SETTEXTMODE, TM_RICHTEXT, 0
	invoke SendMessage, [xsi].reRichCol.hWnd, EM_SETBKGNDCOLOR, 0, 0
    invoke SendMessage, [xsi].reRichCol.hWnd, EM_EXLIMITTEXT, 0, 100000000
	invoke SendMessage, [xsi].reRichCol.hWnd, WM_SETTEXT, 0, addr tabla

	invoke ShowWindow, [xsi].smplplot.hWnd, SW_HIDE
	invoke ShowWindow, [xsi].reRichCol.hWnd, SW_SHOW

	;mov [xsi].smplplot.moscasS, 0
	invoke RedrawWindow, [xsi].smplplot.hWnd, NULL, NULL, \
                   RDW_ERASE or RDW_INVALIDATE or RDW_UPDATENOW or RDW_ALLCHILDREN
MethodEnd

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:    SmplSimWoody.SelectGraphDef
; Purpose:   Copy selected grphic structure to SmplPlot.
; Arguments: None.
; Return:    None.
Method SmplSimWoody.SelectGraphDef, uses xsi xdi xbx

    SetObject xbx
   
    mov xcx, [xbx].MuestraA
    lea xax, GraphDefs
    mov xsi, [xax+xcx*POINTER]
   
    lea xdi, [xbx].smplplot.definition
    cld
    mov xcx , sizeof SmplGraphDef                 ; Size
    rep movsb
    
MethodEnd

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:    SmplSimWoody.OnCommand
; Purpose:   Event procedure for WM_COMMAND message.
; Arguments: Arg1: First message parameter.
;            Arg2: Second message parameter.
; Return:    eax = Zero if handled.
		IDM_Selected macro char1
		.elseif ax == IDM_chMostrar&char1
     	       	HiWord wParam, xbx
			.if bx == CBN_SELCHANGE
			        invoke SendMessage, [xsi].chMostrar&char1.hWnd, CB_GETCURSEL,0,0
					m2m [xsi].Muestra&char1, xax
					ifidn <char1>,<A> 
	                	OCall xsi.SelectGraphDef
						OCall xsi.Show
					elseifidn <char1>,<B>
					    .if xax != 0 && [xsi].MuestraC != 0
					        mov   [xsi].MuestraC, 0
        			        invoke SendMessage, [xsi].chMostrarC.hWnd, CB_SETCURSEL,0,0
					    .endif
					elseifidn <char1>,<C>
					    .if xax != 0 && [xsi].MuestraB != 0
					        mov   [xsi].MuestraB, 0
        			        invoke SendMessage, [xsi].chMostrarB.hWnd, CB_SETCURSEL,0,0
					    .endif
					endif
	 		.endif
		endm

Method SmplSimWoody.OnCommand, uses xsi xbx, wParam:WPARAM, lParam:LPARAM
    local cBuffer[MAX_PATH]:CHR, Dlg:$Obj(DialogAbout)
    local cText[1024]:CHR

    SetObject xsi
    mov xax, wParam
    
    .if ax == IDM_EXIT
        invoke SendMessage, [xsi].hWnd, WM_SYSCOMMAND, SC_CLOSE, NULL
        xor eax, eax
    .elseif ax == IDM_ABOUT
        New Dlg::DialogAbout
        invoke LoadIcon, hInstance, offset szIcoName
        OCall Dlg::DialogAbout.Init, xsi, [xsi].hWnd, xax, offset szAboutText
        OCall Dlg::DialogAbout.Show
        OCall Dlg::DialogAbout.Done
        xor eax, eax
    .elseif ax == IDM_HELPA
        invoke ExpandEnvironmentStrings, $OfsCStr(".\Help\SmplSimWoody.pdf"), addr cBuffer, lengthof(cBuffer)
        invoke PdfView, [xsi].hWnd, addr cBuffer, $OfsCStr("SmplSimWoody")
        xor eax, eax
    .elseif ax == IDM_HELPD
        invoke ExpandEnvironmentStrings, $OfsCStr("%OBJASM_PATH%\Help\ObjAsm_Reference_Volume-I.pdf"), addr cBuffer, lengthof(cBuffer)
        invoke PdfView, [xsi].hWnd, addr cBuffer, $OfsCStr("Introduction")
        xor eax, eax

   	IDM_Selected A
   	IDM_Selected B
   	IDM_Selected C
    .elseif ax == IDM_btRUN
		invoke CreateThread, NULL, 0, $MethodAddr(SmplSimWoody.Execute), xsi, 0, NULL
	    invoke CloseHandle, xax
    .else
        invoke DefWindowProc, [xsi].hWnd, WM_COMMAND, wParam, lParam
    .endif
MethodEnd

Method SmplSimWoody.OnSize, uses xsi, wParam:WPARAM, lParam:LPARAM
    local dHeight:DWORD, dWidth:DWORD, dTop:DWORD
    local CtRect:RECT
    local porcentaje : dword , ancho1 : dword 
    local muestra : dword
    local ancho2: dword
    
    local rect:RECT
    
    .if wParam != SIZE_MINIMIZED
     
      	SetObject xsi
      	
      	invoke GetClientRect, [xsi].hWnd, addr CtRect
      	
      	mov	eax , CtRect.bottom
      	sub	eax , CtRect.top
      	sub	eax , 30
      	mov dHeight, eax
      	
      	mov	eax , CtRect.right
      	sub	eax , CtRect.left
      	sub	eax , 0
      	mov dWidth, eax
    
		invoke MoveWindow, [xsi].smplplot.hWnd, 0, 0, dWidth, dHeight , TRUE
		invoke MoveWindow, [xsi].reRichCol.hWnd, 0, 0, dWidth, dHeight , TRUE
		add   dHeight, 3
		shr   dWidth, 2
		invoke MoveWindow, [xsi].chMostrarC.hWnd, 0, dHeight, dWidth, 200 , TRUE
		mov   __ecx, dWidth
		invoke MoveWindow, [xsi].chMostrarB.hWnd, __ecx, dHeight, dWidth, 200 , TRUE
		mov   __ecx, dWidth
		add   __ecx, dWidth
		invoke MoveWindow, [xsi].btRun.hWnd, __ecx, dHeight, dWidth, 24 , TRUE
		mov   __ecx, dWidth
		add   __ecx, dWidth
		invoke MoveWindow, [xsi].progressbar.hWnd, __ecx, dHeight, dWidth, 24 , TRUE
		mov   __ecx, dWidth
		add   __ecx, dWidth
		add   __ecx, dWidth
		invoke MoveWindow, [xsi].chMostrarA.hWnd, __ecx, dHeight, dWidth, 200 , TRUE
	
    .endif
MethodEnd
; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:    SmplSimWoody.OnPaint
; Purpose:   Event procedure for WM_PAINT message.
; Arguments: Arg1: First message parameter.
;            Arg2: Second message parameter.
; Return:    eax = Zero if handled.

Method SmplSimWoody.OnPaint, uses xsi, wParam:WPARAM, lParam:LPARAM
    local PS:PAINTSTRUCT, hDC:HDC, Rct:RECT
    local btn_hi   :DWORD
    local btn_lo   :DWORD
    
    SetObject xsi
    mov hDC, $invoke(BeginPaint, [xsi].hWnd, addr PS)

    invoke GetClientRect, [xsi].hWnd, addr Rct
    ;invoke DrawEdge, hDC, addr Rct, EDGE_SUNKEN, BF_RECT
    ;invoke DrawEdge, hDC, addr Rct, EDGE_ETCHED, BF_ADJUST
  if 0
    invoke GetSysColor,COLOR_BTNHIGHLIGHT
    mov btn_hi, eax

    invoke GetSysColor,COLOR_BTNSHADOW
    mov btn_lo, eax

    ; --------------------------------------------------------
    ; The following 2 calls draw the border around the buttons
    ; --------------------------------------------------------
    ;   invoke Frame3D, hDC, btn_lo, btn_hi,340,20,460,125,2
    ;   invoke Frame3D, hDC, btn_hi, btn_lo,337,17,463,128,2
    
    add Rct.left,   1
    add Rct.top,    1
    sub Rct.right,  1
    sub Rct.bottom, 1

    invoke Frame3D,hDC,btn_lo,btn_hi,
                   Rct.left,Rct.top,
                   Rct.right,Rct.bottom,2
  endif
    invoke EndPaint, [xsi].hWnd, addr PS
    mov eax, TRUE
MethodEnd
 ;——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:    SmplSimWoody.Execute
; Purpose:   Execute Model.
; Arguments: None.
; Return:    Nothing.
Method SmplSimWoody.Execute, uses xsi
    local fregTLS()
    
	SetObject xsi
	
    mov [xsi].lock1, 1
	invoke ShowWindow, [xsi].btRun.hWnd, SW_HIDE
	invoke ShowWindow, [xsi].progressbar.hWnd, SW_SHOW

	mov   xax, [xsi].MuestraB
	.if xax > 0
	    add   xax, 3
	.else
	    mov   xax, [xsi].MuestraC
	.endif
	_push xax
	
	.if xax == 3

        mov   cut, 0
      	invoke SendMessage, [xsi].reRichCol.hWnd, WM_SETTEXT, 0, addr tabla

	    .if [xsi].showing == 0
	        mov  [xsi].showing, 1
        	invoke ShowWindow, [xsi].reRichCol.hWnd, SW_SHOW
        	invoke ShowWindow, [xsi].smplplot.hWnd, SW_HIDE
            	
	    .endif
	.else    
	    .if [xsi].showing == 1
	        mov  [xsi].showing, 0
	        mov  [xsi].woodymodel.calculado, 0
        	invoke ShowWindow, [xsi].reRichCol.hWnd, SW_HIDE
        	invoke ShowWindow, [xsi].smplplot.hWnd, SW_SHOW
	    .endif
	.endif
	_peek xax
	OCall [xsi].woodymodel::WoodyCarbon.Run, xax
	.if [xsi].woodymodel.calculado
		mov [xsi].smplplot.calculado, TRUE
	   	OCall xsi.Show
    .endif
	invoke ShowWindow, [xsi].btRun.hWnd, SW_SHOW
	invoke ShowWindow, [xsi].progressbar.hWnd, SW_HIDE
    mov [xsi].lock1, 0
MethodEnd

 ;——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:    SmplSimWoody.Show
; Purpose:   Show results.
; Arguments: None.
; Return:    Nothing.
Method SmplSimWoody.Show, uses xsi xdi xbx
    local fregTLS()
    local i:dword, it:dword
    local value_x:REAL8,value_y:REAL8
    local pf : PARAFORMAT
    local pfext[10]:dword
    
	SetObject xsi

    mov xdi, [xsi].woodymodel.model
	assume xdi: $ObjPtr(FOMSI)
	assume _ecx: ptr $Obj(FOMSIResult)

	OCall [xsi].smplplot::SmplPlot.Clear
	
    .if [xsi].showing == 0
		.if [xsi].woodymodel.calculado
	    	ForLpD i, 0, [xsi].smplplot.definition.nseries, eax
			    mov eax, [xsi+DWORD*xax].smplplot.ColorBas
			    OCall [xsi].smplplot::SmplPlot.InitSerie, i, eax, 1
			    mov xbx, xax
			    SDLL_BeginLoop xdi,,FOMSIResult, _ecx,, _edx
			        mov eax, [xsi].smplplot.definition._X
			        fSlv8 value_x = [_ecx+xax*8].R8_Year
			        mov eax, i
			        mov eax, [xsi+xax*4].smplplot.definition._Y1
			        fSlv8 value_y = [_ecx+xax*8].R8_Year
			        OCall xbx::SerieGraph.CargaPunto, value_x, value_y
			    SDLL_EndLoop _ecx
			    .if [xsi].smplplot.definition._Seried
				    mov eax, i
				    mov xcx, [xsi].smplplot.definition._Seried
				    mov xax, [xcx+xax*POINTER]
				    mov [xbx].$Obj(SerieGraph).pName, xax 
			    .endif
			Next i 
	    .endif
	
		invoke RedrawWindow, [xsi].smplplot.hWnd, NULL, NULL, \
		                   RDW_ERASE or RDW_INVALIDATE or RDW_UPDATENOW or RDW_ALLCHILDREN
	
    .else

       	invoke SendMessage, [xsi].reRichCol.hWnd, WM_SETTEXT, 0, hRes$
        
		invoke RedrawWindow, [xsi].reRichCol.hWnd, NULL, NULL, \
		                   RDW_ERASE or RDW_INVALIDATE or RDW_UPDATENOW or RDW_ALLCHILDREN
        
    .endif
	assume _ecx:nothing
	assume xdi:nothing

MethodEnd
 ;——————————————————————————————————————————————————————————————————————————————————————————————————
; Method:    SmplSimWoody.Done
; Purpose:   Free resources and closes and destroys all associated things.
; Arguments: None.
; Return:    Nothing.
Method SmplSimWoody.Done, uses xsi

	SetObject xsi

	OCall [xsi].woodymodel::WoodyCarbon.Done
	OCall [xsi].smplplot::SmplPlot.Done

MethodEnd
endif
