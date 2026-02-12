; ==================================================================================================
; Title:      SDLLH.inc
; Author:     Héctor S. Enrique
; Version:    1.0.0
; Purpose:    Extended ObjAsm support of "Sentinel Doubly Linked List" macros.
; Notes:      SDLL.inc not need to be loaded if only this macros are used.
;             Version 1.0.0, February 2016
;               - First release.
; ==================================================================================================

PSDLL_ITEM typedef ptr SDLL_ITEM
SDLL_ITEM struc
  pNextItem     PSDLL_ITEM   NULL                          ;-> Next item
  pPrevItem     PSDLL_ITEM   NULL                          ;-> Previous item
SDLL_ITEM ends
SDLL_SENTINEL struc
  pFirstItem    PSDLL_ITEM   NULL                          ;-> First item
  pLastItem     PSDLL_ITEM   NULL                          ;-> Last item
SDLL_SENTINEL ends
; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Macro:      SENTINEL_P
; Purpose:    Insert a new Sentinel structure.
; Arguments:  Arg1: Name -> Reference to sentinel structure.

; Example:
;             Object PrentObject,, Primer
;                 SENTINEL_P Items     
;                 DefineR8_ observado
;                 DefineR8_ calculado
;             ObjectEnd
;
;    in parent     
;        -------------------------------------------------
;          DefineVariable    Items, SDLL_SENTINEL,  {}
;        ------------------------------------------------
; ==================================================================================================

    SENTINEL_P macro name:=<items>
        DefineVariable    &name, SDLL_SENTINEL,  {}
    endm

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Macro:      SENTINEL_I
; Purpose:    Insert a new Item structure.
; Arguments:  Arg1: Name -> Reference to item structure. 

; Example
;
;        Object ChildObject,, Primer
;            SENTINEL_I  cadena   
;            DefineR8_ x
;            DefineR8_ y
;        ObjectEnd
;
;     in child
;        -------------------------------------------------------------     
;          DefineVariable    cadena, SDLL_ITEM , {NULL, NULL}        ;Item linkage
;        ------------------------------------------------------------

    SENTINEL_I macro name:=<chain>
        DefineVariable    &name, SDLL_ITEM , {NULL, NULL}
        DefineVariable    &name&_crtl, XWORD, 0  
    endm

; --------------------------------------------------------------------------------------------------
; Macro:      SDLL_AppendH
; Purpose:    Append a member to the linked list.
; Arguments:  Arg1: Register -> Reference Parent object
;             Arg2: Name -> Sentinel structure.
;             Arg3: Name -> Child object.
;             Arg4: Register -> Child object
;             Arg5: Name -> item structure
;             Arg6: Auxiliar register.

; Example:    SDLL_RemoveH esi, Items, Woody0, ebx, cadena, eax

SDLL_AppendH macro ParentReg:req, Sentinel:=<items>, ChildObj:req, ChildReg:req, Chain:=<chain>, AuxReg:req

    SetObject AuxReg, ChildObj, [ParentReg].&Sentinel.SDLL_SENTINEL.pLastItem
    mov   [AuxReg].&Chain.SDLL_ITEM.pNextItem , ChildReg
    
    assume ChildReg: $ObjPtr(ChildObj)
    mov   [ChildReg].&Chain.SDLL_ITEM.pPrevItem, AuxReg
    mov   [ParentReg].&Sentinel.SDLL_SENTINEL.pLastItem, ChildReg
    assume ChildReg:nothing
endm

; --------------------------------------------------------------------------------------------------
; Macro:      SDLL_SDLL_InitH
; Purpose:    Sentinel creation like first member in the linked list.
; Arguments:  Arg1: Register -> Reference Parent object
;             Arg2: Name -> Sentinel structure.
;             Arg3: Name -> Child object.

SDLL_InitH macro ParentReg:req, Sentinel:=<items>, ChildObj:req, Chain:=<chain>

    New   &ChildObj
    mov   [ParentReg].&Sentinel.SDLL_SENTINEL.pFirstItem, xax
    mov   [ParentReg].&Sentinel.SDLL_SENTINEL.pLastItem, xax
    assume xax: $ObjPtr(&ChildObj)
    mov   [xax].&Chain.SDLL_ITEM.pPrevItem, 0
    mov   [xax].&Chain.SDLL_ITEM.pNextItem, 0
    assume xax:nothing
   

endm

; --------------------------------------------------------------------------------------------------
; Macro:      SDLL_BeginLoop
; Purpose:    Remove a member from the linked list.
; Arguments:  Arg1: Register -> Reference Parent object
;             Arg2: Name -> Sentinel structure.
;             Arg3: Name -> Child object.
;             Arg4: Register -> Child object
;             Arg5: Name -> item structure

SDLL_BeginLoop macro ParentReg:req, Sentinel:=<items>, ChildObj:req, ChildReg:req, Chain:=<chain>,AuxReg:=<r10>
    mov   ChildReg, [ParentReg].&Sentinel.SDLL_SENTINEL.pFirstItem
    ;.if ChildReg == NULL
    ;    DbgWarning "Where the sentinel is???"
    ;.else 
    ;    DbgHex ChildReg," sentinel"        
    ;.endif
    mov   ChildReg, [ChildReg].&Chain.SDLL_ITEM.pNextItem
    .while ChildReg != NULL
        ;assume ChildReg: ptr $Obj(&ChildObj)
        _pushv [ChildReg].&Chain.SDLL_ITEM.pNextItem, @if64bits(&AuxReg)
        ;assume ChildReg: nothing
endm

; --------------------------------------------------------------------------------------------------
; Macro:      SDLL_EndLoop
; Purpose:    End loop througth the linked list.
; Arguments:  No arguments

SDLL_EndLoop macro ChildReg:req
        _pop ChildReg
    .endw
    ;DbgText "close loop"
endm

; --------------------------------------------------------------------------------------------------
; Macro:      SDLL_RemoveH
; Purpose:    Remove a member from the linked list.
; Arguments:  Arg1: Register -> Reference Parent object
;             Arg2: Name -> Sentinel structure.
;             Arg3: Name -> Child object.
;             Arg4: Register -> Child object
;             Arg5: Name -> item structure
;             Arg6: Auxiliar register 1.
;             Arg7: Auxiliar register 2. If not specified, RefReg is used.

; Example:    SDLL_RemoveH esi, Items, Woody0, ebx, cadena, eax, ecx

SDLL_RemoveH macro  ParentReg:req, Sentinel:=<items>, ChildObj:req, RefReg:req ,Chain:=<chain>, AuxReg1:req, AuxReg2:=<>, clean

    assume AuxReg1: $ObjPtr(&ChildObj)
    ifb <AuxReg2>
        mov AuxReg1, [RefReg].&Chain.SDLL_ITEM.pNextItem
        mov RefReg, [RefReg].&Chain.SDLL_ITEM.pPrevItem
        .if AuxReg1 != NULL
            mov [AuxReg1].&Chain.SDLL_ITEM.pPrevItem, RefReg
        .else
            mov [ParentReg].&Sentinel.SDLL_SENTINEL.pLastItem, RefReg
        .endif
        mov [RefReg].&Chain.SDLL_ITEM.pNextItem, AuxReg1
    else
        assume AuxReg2: $ObjPtr(&ChildObj)
        mov AuxReg1, [RefReg].&Chain.SDLL_ITEM.pNextItem
        mov AuxReg2, [RefReg].&Chain.SDLL_ITEM.pPrevItem
        .if AuxReg1 != NULL
            mov [AuxReg1].&Chain.SDLL_ITEM.pPrevItem, AuxReg2
        .else
            mov [ParentReg].&Sentinel.SDLL_SENTINEL.pLastItem, AuxReg2
        .endif
        mov [AuxReg2].&Chain.SDLL_ITEM.pNextItem, AuxReg1
        ifnb <clean>
	        ; Free relations in case is preserved
	        mov [RefReg].&Chain.SDLL_ITEM.pNextItem, 0
	        mov [RefReg].&Chain.SDLL_ITEM.pPrevItem, 0
        endif
        assume AuxReg2: nothing
    endif
    assume AuxReg1: nothing
endm

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Macro:      SDLL_CleanH
; Purpose:    Release the Linked List from Parent.
; Arguments:  Arg1: Register -> Parent pointer.
;             Arg2: Name -> Sentinel structure.
;             Arg3: ChilObject -> Linked Object Name
;             Arg4: Register -> willbe used as pointer
;             Arg5: Name - > Linked structure     
;
; Example:    SDLL_Clean esi, Items, punto, eax, cadena

SDLL_CleanH macro ParentReg:req, Sentinel:=<items>, ChildObj:req, ChildReg:req, Chain:=<chain>

    assume ChildReg: ptr $Obj(&ChildObj)
    SDLL_DestroyH &ParentReg, &Sentinel, &ChildObj, &ChildReg, &Chain

    mov ChildReg, [ParentReg].&Sentinel.SDLL_SENTINEL.pFirstItem
    mov [ChildReg].&Chain.SDLL_ITEM.pPrevItem, 0
    mov [ChildReg].&Chain.SDLL_ITEM.pNextItem, 0
    mov [ParentReg].&Sentinel.SDLL_SENTINEL.pLastItem, ChildReg

    assume ChildReg: nothing
endm

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Macro:      SDLL_DoneH
; Purpose:    Remove all members, calling Done before.
; Arguments:  Arg1: Register -> Parent pointer.
;             Arg2: Name -> Sentinel structure (in parent).
;             Arg3: ChilObject -> Linked Object Name
;             Arg4: Register -> willbe used as pointer
;             Arg5: Name - > Linked structure (in child),    
;
; Example:    SDLL_Done esi, Items, punto, eax, cadena
;
SDLL_DoneH macro ParentReg:req, Sentinel:=<items>, ChildObj:req, ChildReg:req, Chain:=<chain>
    mov ChildReg, [ParentReg].&Sentinel.SDLL_SENTINEL.pFirstItem
    .while ChildReg != NULL
          assume ChildReg: $ObjPtr(&ChildObj)
         _pushv [ChildReg].&Chain.SDLL_ITEM.pNextItem    ;Preserve address of next node
          Destroy ChildReg                                ;Free the linked node
          _pop   ChildReg                                    ;Restore address of next node
    .endw
endm

; ——————————————————————————————————————————————————————————————————————————————————————————————————
; Macro:      SDLL_DestroyH
; Purpose:    Remove all members, calling Done before, keep alive Sentinel.
; Arguments:  Arg1: Register -> Parent pointer.
;             Arg2: Name -> Sentinel structure (in parent).
;             Arg3: ChilObject -> Linked Object Name
;             Arg4: Register -> willbe used as pointer
;             Arg5: Name - > Linked structure (in child),    
;
; Example:    SDLL_Done esi, Items, punto, eax, cadena
;
SDLL_DestroyH macro ParentReg:req, Sentinel:=<items>, ChildObj:req, ChildReg:req, Chain:=<chain>
 
    mov ChildReg, [ParentReg].&Sentinel.SDLL_SENTINEL.pFirstItem
    mov ChildReg, [ChildReg].&Chain.SDLL_ITEM.pNextItem
    .while ChildReg != NULL
         _pushv [ChildReg].&Chain.SDLL_ITEM.pNextItem    ;Preserve address of next node
          Destroy ChildReg                                ;Free the linked node
          _pop   ChildReg                                    ;Restore address of next node
    .endw
endm
;---------------------------------------------------------------------------
