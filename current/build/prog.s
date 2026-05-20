section .data

section .text

    global myFunctionFinal:

myFunctionFinal:
    push rbp
    mov rbp, rsp
    sub rsp, 32

    mov edi, 5
    mov [rbp-8], edi

    mov edi, 70
    mov [rbp-12], edi

    mov edi, 200
    mov [rbp-16], edi

    mov edi, 10
    mov [rbp-20], edi

    mov ebx, [rbp-20]
    mov eax, ebx
    leave
    ret
