; ==================================================================================================
; Title:   ModMacs.inc
; Author:  Héctor S. Enrique - March 2018
; Version: 1.0.0
; Purpose: Support macros for models implementation and testing 
; Notes:    Framework version C.1.0, 2018
;               - First release.
; ==================================================================================================
    DefineDD macro nombre , valor:=<0>
        @CatStr(  DD35_ , &nombre , <  dword >) &valor
       % &nombre textequ <[regpointer].DD35_&nombre>
    endm
    DefineXD macro nombre , valor:=<0>
        @CatStr( XD_ , &nombre, <  XWORD > ) &valor
        % &nombre textequ <[regpointer].XD_&nombre>
    endm
    
    DefineR8 macro nombre , valor:=<0.0>
        @CatStr(R8_ , &nombre, < REAL8 > ) &valor
       % &nombre textequ <[regpointer].R8_&nombre>
    endm
    DefinemDD macro nombre , valor:=<0>
        % DefineVariable    @CatStr(  < >,DD35_ , &nombre ) , dword , &valor
        &nombre textequ <[regpointer].DD35_&nombre>
    endm
    DefinemXD macro nombre , valor:=<0>
        %DefineVariable    @CatStr(  < >,XD_ , &nombre ) , XWORD , &valor
        &nombre textequ <@CatStr(<[regpointer].XD_>,&nombre)>
    endm
    
    DefinemR8 macro nombre , valor:=<0.0>
        DefineVariable    @CatStr(  < >,R8_ , &nombre ) , REAL8 , &valor
        &nombre textequ <[regpointer].R8_&nombre>
    endm
    DefinemR8s macro args:vararg
        for arg, <args>
            DefinemR8 <arg>
        endm 
    endm
    DefinemR8a macro nombre:req, args:vararg
        $$vuelta = 0
        for arg, <args>
            if $$vuelta eq 0
                DefinemR8 &nombre, <arg>
            else
                DefinemR8 @CatStr(<&nombre>,%$$vuelta), <arg>
            endif    
            $$vuelta = $$vuelta + 1
        endm 
    endm
    
    estado macro nombre:REQ, iniValue:=<0.0>
        DefinemR8        i&nombre& ,  &iniValue
        DefinemR8        &nombre&  ,  0.0
        DefinemR8        d&nombre& ,  0.0
    endm

    ClassesVar macro name:REQ, args:VARARG
        cuenta = 0
        for arg,<args>
            @CatStr(&name,%cuenta,< equ >,arg)
            cuenta = cuenta+1
        endm
    endm
    ClassVar macro var:REQ,name:REQ
        @CatStr(<Definem>,&var,< >,&name,<,>,&name,%Specie)
    endm
    
    TreesVar macro var:REQ,name:REQ
        if Specie gt 0
            @CatStr(<Definem>,&var,< >,&name,<,>,&name,%Specie)
        endif
    endm
    ClassArray macro var:REQ,name:REQ
        @CatStr(<Definem>,&var,< >,&name,<,>, n&name,%Specie,< + 1 dup (0.0)>)
    endm
    
    DefVar macro variable
		if DefType eq 0
			DefinemR8 &variable
		elseif DefType eq 1
		    fSlv8 [xax].R8_&variable = 0
		elseif DefType eq 2
		    fSlvW [xax].R8_&variable += [_ecx].R8_&variable 
		elseif DefType eq 3
		    fSlvW [xax].R8_&variable /= years
		    DbgWriteF ,,"&variable = ¦F4",[xax].R8_&variable 
		elseif DefType eq 4
		    fSlvW [xax].R8_&variable = 0.3*[_ecx].R8_&variable + 0.6*[_edx].R8_&variable+0.1*[xbx].R8_&variable 
		    DbgWriteF ,,"&variable = ¦F4",[xax].R8_&variable 
		endif
	endm	




