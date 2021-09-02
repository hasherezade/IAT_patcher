[bits 64]
start:
LABEL_newEP:
	mov rax, 11111111h ; new EP
LABEL_dataRVA:
	mov rbx, 22222222h ; data RVA
	call init
after_init:
LABEL_oldEP:
    jmp 12345678h ; oep
init:
	add rax, (after_init - start)
	mov r15, [rsp]
	sub r15, rax ;r15 -> ImageBase
	add rbx, r15
load_lib:
	call get_saved_rva
	TEST EAX, EAX
	JNZ SHORT load_next_lib
	RET

load_next_lib:
	mov rcx, rax
	mov r14,rbx
	mov rbx,[rsp]
LABEL_LoadLibrary:
	CALL QWORD NEAR [RIP+0X5A1ED] ; call LoadLibraryA
	test rax, rax
	mov rbx,r14
	jnz load_function
skip_functions:	; if DLL not found, skip mapped <name><thunk>
	call get_saved_value
	test eax,eax
	jne skip_functions
	jmp load_lib
	
load_function:
	mov rcx, rax
	call get_saved_rva
	TEST EAX, EAX
	JZ SHORT load_lib

	mov rdx, rax
	CALL getProc
	mov rsi, rax ; RSI <- Handle
	call get_saved_rva ; thunk to fill or to skip
	TEST EAX, EAX	; is thunk empty?
	jne overwrite_thunk
	ret ; malformed data, just finish...

overwrite_thunk:
	TEST rsi, rsi ; is Handle Empty?
	je _end_load_function
	MOV [rax], rsi
_end_load_function:
	JMP SHORT load_function
	
get_saved_rva:
	call get_saved_value
	test eax,eax
	jz _end_get_saved_rva
	ADD rax, r15 ; ImgBase
_end_get_saved_rva:
	ret
	
get_saved_value:
	xor rax, rax
	mov eax, dword [rbx]
	ADD rbx, 0X4
	ret
;>------	
getProc:
	PUSH RBX
	SUB RSP, 0X20
LABEL_GetProcAddress:
	CALL QWORD NEAR [RIP+0XFB4]	;[KERNEL32.dll].GetProcAddress
	ADD RSP, 0X20
	POP RBX
	RET
;<-------
;--------
;Data:
dd (LABEL_newEP - start)
dd (LABEL_dataRVA - start)
dd (LABEL_oldEP - start)
dd (LABEL_LoadLibrary - start)
dd (LABEL_GetProcAddress - start)