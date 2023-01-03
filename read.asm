
EXTERN gacbase :QWORD
EXTERN origcall :QWORD
EXTERN origqword :QWORD

.CODE

__readr14 PROC
mov rax,r14
ret
__readr14 ENDP

__readr13 PROC
mov rax,r13
ret
__readr13 ENDP

__readrdi PROC
mov rax,rdi
ret
__readrdi ENDP

__readrsi PROC
mov rax,rsi
ret
__readrsi ENDP

__readrbx PROC
mov rax,rbx
ret
__readrbx ENDP

__origcall PROC

sub rsp,028h
mov rax,origqword
mov rax,[rax]
mov rdx,rcx

jmp origcall


__origcall ENDP

END