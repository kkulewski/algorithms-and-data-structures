.intel_syntax noprefix
	.text
	.globl main
	
	//eax = arg
	//edx = ret
	//ecx = temp
	
	//f(n) =
		//m for n=0
		//0 for n=1
		//3 for n=2
		//f(n-1)-f(n-2)+2*f(n-3) for n>2
	
main:
	mov eax,1; //m
	push eax
	mov eax,8; //n
	call f
	add esp,4

	push edx
	lea eax,msg
	push eax
	call printf
	add esp,8

	mov eax,0
	ret

f:
	mov ebp,esp
	push ebp

	push eax
	push ecx
	
	mov edx,0
	mov ecx,0

f0:
	cmp eax,0
	jne f1
	add ecx,[ebp+4]
	jmp out
  
f1:
	cmp eax,1
	jne f2
	add ecx,0
	jmp out
  
f2:  
	cmp eax,2
	jne fn
	add ecx,3
	jmp out

fn:
	dec eax
	push [ebp+4]
	call f
	add esp,4
	add ecx,edx

	dec eax
	push [ebp+4]
	call f
	add esp,4
	sub ecx,edx

	dec eax
	push [ebp+4]
	call f
	add esp,4
	add edx,edx
	add ecx,edx

out:
	mov edx,ecx;
	pop ecx
	pop eax
	pop ebp

	ret

.data
msg: .asciz "%i\n"
