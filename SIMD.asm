;Charlyne Arajoy Carabeo CSC612M
section .text
bits 64
default rel ;[]
global SIMDstencilVector

SIMDstencilVector:
; rcx - ARRAY_SIZE, rdx - ptr to out r8 - ptr to in, r9 - not used
shr rcx, 3 ; divide by 8
; shr divided by power of 2 raised by constant 2^3 = 8


stencilVectorSIMD:
  vmovdqu ymm1,[r8]  ;ymm1 = X[i-3] 
  vmovdqu ymm2,[r8+4]   ;ymm2 = X[i-2]
  vpaddd ymm3,ymm1,ymm2 ;ymm3 = X[i-3] + X[i-2]

  vmovdqu ymm1, [r8+8]  ;ymm1 = X[i-1]
  vpaddd ymm3,ymm1      ;ymm3 = X[i-3] + X[i-2] + X[i-1]
 
  vmovdqu ymm1, [r8+12]  ;ymm1 = X[i]
  vpaddd ymm3,ymm1      ;ymm3 = X[i-3] + X[i-2] + X[i-1] + X[i]

  vmovdqu ymm1, [r8+16]  ;ymm1 = X[i+1]
  vpaddd ymm3,ymm1      ;ymm3 = X[i-3] + X[i-2] + X[i-1] + X[i] + X[i+1]

  vmovdqu ymm1, [r8+20]  ;ymm1 = X[i+2]
  vpaddd ymm3,ymm1      ;ymm3 = X[i-3] + X[i-2] + X[i-1] + X[i] + X[i+1] + X[i+2]

 vmovdqu ymm1, [r8+24]  ;ymm1 = X[i+3]
 vpaddd ymm3,ymm1      ;ymm3 = X[i-3] + X[i-2] + X[i-1] + X[i] + X[i+1] + X[i+2] + X[i+3]

  vmovdqu [rdx], ymm3	;output = ymm3	
  add r8, 32			;next iteration
  add rdx, 32			;next iteration 
loop stencilVectorSIMD
exit:
    ret