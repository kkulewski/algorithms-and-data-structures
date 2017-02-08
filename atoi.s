
.intel_syntax noprefix
	.text
	.globl _start

	//asci 0 = 48
_start:

	//ustaw 0 jako pozycje aktualnego znaku
	mov eax, 0
	//wstaw liczbe-string do ecx
	mov ecx, [esp+8]
	//ustaw 0 jako liczbe z linii polecen
	mov edx, 0
	//ustaw 0 jako aktualna cyfre
	mov ebx, 0
	
petla:
	//sprawdz czy koniec liczby-stringa
	cmp byte ptr [ecx+eax], 0
	je out
	jne kolejny_znak
	
kolejny_znak:
	//pomnoz liczbe razy 10
	imul edx, 10
	//wstaw kolejna ascii-cyfre do ebx
	mov ebx, [ecx+eax]
	//odejmij 48 od ascii-cyfry -> cyfra staje sie int
	sub ebx, '0'
	//dodaj kolejna cyfre do liczby
	add edx, ebx
	//zwieksz pozycje znaku
	inc eax
	//skocz do petla
	jmp petla
	
	
	
out:
	//wstaw liczbe do edi
	mov edi, edx
	
	//zakoncz program
	mov eax, 1
	mov ebx, edi
	int 0x80
	

