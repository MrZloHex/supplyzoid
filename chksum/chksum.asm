		global    _start

		section   .text
usage:		mov		rax, help_msg
		call		print
		ret

prcss_str:	push		rax
		mov		rbx, rax
		call		length
		cmp	  BYTE	[rbx+rax-1], 94		; '^' XOR checksum
		je		_prc_xor
		cmp  	  BYTE	[rbx+rax-1], 42		; '*' ADD checksum
		je		_prc_add
		mov		rax, prc_err_t
		call		print
		jmp		exit_err
_prc_add:	mov		rdx, 4
		jmp		_prc_count
_prc_xor:	mov		rdx, 8
_prc_count:	pop		rax
		mov		bl, 0			; For ADD checksum
		mov		cl, 0			; For XOR checksum
_prc_loop:	cmp	   BYTE [rax], 94
		je		_prc_exit
		cmp	   BYTE	[rax], 42
		je		_prc_exit
		add		bl, [rax]
		xor		cl, [rax]
		inc		rax
		jmp		_prc_loop
_prc_exit:	mov		rax, 0
		cmp		rdx, 4
		je		_prc_ex_ad
		cmp		rdx, 8
		je		_prc_ex_xr
_prc_ex_ad:	mov		al, bl
		ret
_prc_ex_xr:	mov		al, cl
		ret

u4_2_hxch:	push		rdx
		mov		rdx, 0
		mov		dl, al
		cmp		al, 10
		jl		_hxch_ciph
		cmp		al, 16
		jl		_hxch_lett
		jmp		exit_err
_hxch_ciph:	add		dl, 48
		jmp		_hxch_exit
_hxch_lett:	add		dl, 55
_hxch_exit:	mov		rax, rdx
		pop		rdx
		ret

print_chsm: 	push		rbx			; VALUE OF CHEKSUM
		push 		rax
		call 		length
		mov		rcx, rax
		pop		rax
		mov		rbx, buf
		call		memcpy
		pop		rax
		push		rax
		clc
		shr		al, 4
		call		u4_2_hxch
		mov		[buf+rcx], al
		inc		rcx
		pop		rax
		clc
		shl		al, 4
		clc
		shr		al, 4
		call		u4_2_hxch
		mov		[buf+rcx], al
		inc		rcx
		mov	   BYTE	[buf+rcx], 0
		mov		rax, buf
		call		print
		mov		rax, new_line
		call		print
		ret

_start:		pop		rax
		cmp		rax, 2
		jge		_get_args
		call		usage
		jmp		exit_err
_get_args:	pop		rdx
		mov		r8, rax
		mov		r9, 1
_arg_loop:	cmp		r8, r9
		je		_arg_lp_ex
		pop		rax
		push		rax
		call		prcss_str
		mov		rbx, rax
		pop		rax
		call		print_chsm
		inc		r9
		jmp		_arg_loop
_arg_lp_ex:	jmp		exit_ok



exit_ok:	mov		rax, 60			; Syscall number
		mov		rdi, 0			; Exit code
		syscall

exit_err:	mov		rax, 60
		mov		rdi, 1
		syscall

print:		mov		rsi, rax		; Address of string
		call		length
		mov		rdx, rax		; Length of string
		mov		rax, 1			; Syscall number
		mov		rdi, 1			; File dscrp - STDOUT
		syscall
		ret

length:		mov		rbx, rax
_len_loop:	cmp	  	BYTE 	[rax], 0
		je		_len_exit
		inc		rax
		jmp		_len_loop
_len_exit:	sub		rax, rbx
		ret

memcpy:		push		r8
		mov		rdx, 0
_mmcp_loop:	cmp		rdx, rcx
		je		_mmcp_exit
		mov		r8, [rax+rdx]
		mov		[rbx+rdx], r8
		inc		rdx
		jmp		_mmcp_loop
_mmcp_exit:
		pop		r8
		ret

		section   .bss
buf:		resb 		32

		section   .data
help_msg:  	db		"ERROR: not provided arguments", 10, 0
prc_err_t:	db		"ERROR: unknown type of cheksum", 10, 0
new_line:	db		10,0
