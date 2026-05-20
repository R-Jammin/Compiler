section .data

section .text

    global myFunctionFinal:

myFunctionFinal:
    push rbp
    mov rbp, rsp
    sub rsp, 80

    mov edi, 5
    mov [rbp-8], edi

    mov edi, 70
    mov [rbp-12], edi

    mov edi, 200
    mov [rbp-16], edi

    mov edi, 10
    mov [rbp-20], edi

    mov edi, 1
    mov [rbp-24], edi

    mov ebx, [rbp-28]
    mov ecx, [rbp-8]
    add ebx, ecx
    mov [rbp-28], ebx
    mov edi, ebx

    mov ecx, ebx

    mov [rbp-32], edi

    mov [rbp-36], edi

    mov edi, 1
    mov [rbp-40], edi

    mov ebx, [rbp-36]
    mov ecx, [rbp-12]
    add ebx, ecx
    mov [rbp-44], ebx
    mov edi, ebx

    mov ecx, ebx

    mov [rbp-48], edi

    mov [rbp-52], edi

    mov edi, 100
    mov [rbp-56], edi

    mov edi, 1
    mov [rbp-60], edi

    mov ebx, [rbp-44]
    mov ecx, [rbp-8]
    add ebx, ecx
    mov [rbp-64], ebx
    mov edi, ebx

    mov ecx, ebx

    mov [rbp-68], edi

    mov [rbp-72], edi

    mov edi, 1
    mov [rbp-76], edi

    mov ebx, [rbp-52]
    mov ecx, [rbp-12]
    add ebx, ecx
    mov [rbp-80], ebx
    mov edi, ebx

    mov ecx, ebx

    mov [rbp-84], edi

    mov [rbp-88], edi

    mov edi, 1
    mov [rbp-92], edi

    mov ebx, [rbp-60]
    mov ecx, [rbp-16]
    add ebx, ecx
    mov [rbp-96], ebx
    mov edi, ebx

    mov ecx, ebx

    mov [rbp-100], edi

    mov [rbp-104], edi

    mov edi, 1
    mov [rbp-108], edi

    mov ebx, [rbp-68]
    mov ecx, [rbp-8]
    add ebx, ecx
    mov [rbp-112], ebx
    mov edi, ebx

    mov ecx, ebx

    mov [rbp-116], edi

    mov [rbp-120], edi

    mov ebx, [rbp-72]
    mov eax, ebx
    leave
    ret
