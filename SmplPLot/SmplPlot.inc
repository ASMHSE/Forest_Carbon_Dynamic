; ==================================================================================================
; Title:   SmplPlot.inc
; Author:  Héctor S. Enrique - January 2026
; Version: 1.0.0
; Purpose: Simple graphic implementation 
; Notes:    Framework version C.2.0, 2026
;               - First release.
; ==================================================================================================

	mcore macro grafico:REQ, base:REQ, args:VARARG  
		cnt = 0
		
		;assume ebx : ACUo
		if Instancias_del_modelo eq 0
			FOR item, <args>
	    			OCall [xsi].&grafico::Resul02.CargaPuntoC, tanda, %cnt,  x, [xbx].R8_<item>
				cnt = cnt + 1
	  			DbgFloat  [xbx].R8_<item>
  			ENDM
  		else
  			FOR item, <args>
    				OCall [xsi].&grafico::Resul02.CargaPuntoC, tanda, vk,  x, [xbx].R8_<item>
				cnt = 1
				DbgFloat  [xbx].R8_<item>
  			ENDM
  		endif
		mov [xsi].&grafico.moscasC, cnt
		mov [xsi].&grafico.arranque, &base
	endm
	graf_mif macro grafico:REQ, categoria:REQ, numero:REQ, base:REQ, args:VARARG
	    	LOCAL cnt
		.if    @CatStr(< >, <( [xsi].>, &categoria, < == >, &numero, < )>) 
			mcore &grafico, &base, <args> 
	endm
	graf_melseif macro grafico:REQ,categoria:REQ, numero:REQ, base:REQ, args:VARARG
	    	LOCAL cnt
  	;		.elseif    @CatStr(< >, <( [xsi].>, &categoria, < == >, &numero, < )>) 
 		.elseif    ( [xsi].&categoria ==  &numero ) 
		mcore &grafico, &base, <args> 
	endm
	mendif macro
		.endif
	endm
	; ——————————————————————————————————————————————————————————————————————————————————————————————————
	
	mtcore macro  grafico:REQ, titulop:REQ, unidadyp:REQ, args:VARARG
		m2m [xsi].&grafico.titulo, $OfsCStr(" &titulop ") 
		m2m [xsi].&grafico.unidady, $OfsCStr(" &unidadyp ")   
		cnt = 0
  		FOR item, <args>
			m2m [xsi+%cnt*4].&grafico.tituloSerie, %@CatStr( <$OfsCStr(!">  item  < !")> )
			cnt = cnt + 1
  		ENDM
	endm
	graf_mtif macro grafico:REQ,categoria:REQ, numero:REQ, titulop:REQ, unidadyp:REQ, args:VARARG  ;	//	Peso Vacío
	    	LOCAL cnt
  		.if @CatStr(< >, <( [xsi].>, &categoria, < == >, &numero, < )>)
  			 mtcore &grafico, &titulop, &unidadyp, <args>
	endm
	graf_mtelseif macro grafico:REQ,categoria:REQ, numero:REQ, titulop:REQ, unidadyp:REQ, args:VARARG  ;	//	Peso Vacío
	    	LOCAL cnt
 		.elseif    ( [xsi].&categoria ==  &numero ) 
			 mtcore &grafico, &titulop, &unidadyp, <args>
	endm
	graf_mtelse macro grafico:REQ, titulop:REQ, unidadyp:REQ, args:VARARG  ;	//	Peso Vacío
	    	LOCAL cnt
 		.else 
			 mtcore &grafico, &titulop, &unidadyp, <args>
	endm
	mmuestra macro  grafico:REQ,base:REQ, args:VARARG  ;	//	Peso Vacío
	    	LOCAL cnt
  		cnt = 0
  		FOR item, <args>
    			OCall [xsi].&grafico::Resul02.CargaPuntoC, tanda, %cnt,  x, [xbx].R8_<item>
			cnt = cnt + 1
  		ENDM
		mov [xsi].&grafico.moscasC, cnt
		mov [xsi].&grafico.arranque, &base
	endm


% include SmplPlot_OOP.inc

if IMPLEMENT

 % include SmplPlot_PROC.inc    

 % include SerieGraph.inc    

endif
