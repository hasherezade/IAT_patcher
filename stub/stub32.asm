[bits 32]
start:
LABEL_newEP:
	mov EAX, 11111111h ; new EP
LABEL_dataRVA:
	mov EBX, 22222222h ; data RVA
	call init
after_init:
LABEL_oldEP:
    jmp 12345678h ; oep
init:
	add EAX, (after_init - start)
	mov EDI, [ESP]
	sub EDI, EAX ; ImageBase
	add EBX, EDI
load_lib:
	call get_saved_rva
	TEST EAX, EAX
	JNZ SHORT load_next_lib
	RET

load_next_lib:
	mov ECX, EAX
	push ECX
LABEL_LoadLibrary:
	MOV ESI, 22222222h
	ADD ESI, EDI
	CALL [ESI] ; call LoadLibraryA
	test EAX, EAX
	jnz load_function
	
skip_functions:	; if DLL not found, skip mapped <name><thunk>
	call get_saved_value
	test eax,eax
	jne skip_functions
	jmp load_lib
	
load_function:
	mov ECX, EAX
	call get_saved_rva
	TEST EAX, EAX
	JZ SHORT load_lib
	
	push EAX
	push ECX
LABEL_GetProcAddress:
	MOV ESI, 22222222h
	ADD ESI, EDI
	CALL DWORD NEAR [ESI] ; call GetProcAddress
	mov ESI, EAX ; ESI <- Handle
	call get_saved_rva ; thunk to fill or to skip
	TEST EAX, EAX	; is thunk empty?
	jne overwrite_thunk
	ret ; malformed data, just finish...

overwrite_thunk:
	TEST ESI, ESI ; is Handle Empty?
	je _end_load_function
	MOV [EAX], ESI
_end_load_function:
	JMP SHORT load_function
	
get_saved_rva:
	call get_saved_value
	test eax,eax
	jz _end_get_saved_rva
	ADD EAX, EDI ; ImgBase
_end_get_saved_rva:
	ret
	
get_saved_value:
	mov eax, dword [EBX]
	ADD EBX, 0X4
	ret
;--------
;Data:
dd (LABEL_newEP - start)
dd (LABEL_dataRVA - start)
dd (LABEL_oldEP - start)
dd (LABEL_LoadLibrary - start)
dd (LABEL_GetProcAddress - start)
