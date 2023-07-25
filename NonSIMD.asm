;Charlyne Arajoy Carabeo CSC612M
section .text
bits 64
default rel ;[]
global stencilVectorx86

stencilVectorx86:
   stencilVectorx86:
    ; rcx - ARRAY_SIZE, rdx - ptr to out r8 - ptr to in, r9 - not used
    movq xmm1, qword [r8-12] ; xmm1 = X[i-3]
    movq xmm2, qword [r8-8]  ; xmm2 = X[i-2]
    vpaddd xmm3, xmm1, xmm2 ; xmm3 = X[i-3] + X[i-2]


    movq xmm1, qword [r8-4] ; xmm1 = X[i-1]
    vpaddd xmm3, xmm1       ; xmm3 = X[i-3] + X[i-2] + X[i-1]

    movq xmm1, qword [r8] ; xmm1 = X[i]
    vpaddd xmm3, xmm1       ; xmm3 = X[i-3] + X[i-2] + X[i-1] + X[i]

    movq xmm1, qword [r8+4] ; xmm1 = X[i+1]
    vpaddd xmm3, xmm1       ; xmm3 = X[i-3] + X[i-2] + X[i-1] + X[i] + X[i+1]

    movq xmm1, qword [r8+8] ; xmm1 = X[i+2]
    vpaddd xmm3, xmm1       ; xmm3 = X[i-3] + X[i-2] + X[i-1] + X[i] + X[i+1] + X[i+2]

    movq xmm1, qword [r8+12] ; xmm1 = X[i+3]
    vpaddd xmm3, xmm1       ; xmm3 = X[i-3] + X[i-2] + X[i-1] + X[i] + X[i+1] + X[i+2] + X[i+3]

    movq qword[rdx], xmm3	;output = xmm3	
    add r8, 8			;next iteration
    add rdx, 8			;next iteration 
    loop stencilVectorx86

ret