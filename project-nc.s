.intel_syntax noprefix
	.text
	.globl _start
_start:

// ATOI
	mov edi, 0
	mov eax, 0
	mov ecx, [esp+8]
	mov edx, 0
	mov ebx, 0
	
petla:
	cmp byte ptr [ecx+eax], 0
	je out
	jne kolejny_znak
	
kolejny_znak:
	imul edx, 10
	mov bl, [ecx+eax]
	sub bl, '0'
	movzx ebx, bl
	add edx, ebx
	inc eax
	jmp petla
	
out:
	mov edi, edx
// END ATOI

	
// SZUKAJ
	mov ecx, [esp+12]
	mov eax, 0 
	mov ebx, 0
	mov edx, 1
	
sprawdz_znak:
	cmp byte ptr [ecx+eax], 0
	je znaleziono_koniec
	cmp byte ptr [ecx+eax], ' '
	je znaleziono_spacje
	jmp znaleziono_znak
		
znaleziono_spacje:	
	cmp byte ptr [ecx+eax], 0
	je znaleziono_koniec
	cmp byte ptr [ecx+eax], ' '
	je przesun_znak_i_ustaw_flage
	jmp sprawdz_znak
	
przesun_znak_i_ustaw_flage:
	inc eax
	mov edx, 1
	jmp znaleziono_spacje
	
znaleziono_znak:
	cmp byte ptr [ecx+eax], 0
	je znaleziono_koniec
	cmp edx, 1
	je znaleziono_nowe_slowo
	cmp ebx, edi
	je drukuj_znak
	inc eax
	jmp sprawdz_znak
	
znaleziono_nowe_slowo:
	mov edx, 0
	inc ebx
	jmp znaleziono_znak
	
drukuj_znak:
	push eax
	push ebx
	push ecx
	push edx
	push ebp
	
	mov ebp, ecx
	add ebp, eax
	mov eax, 4
	mov ebx, 1
	mov ecx, ebp
	mov edx, 1
	int 0x80
	
	pop ebp
	pop edx
	pop ecx
	pop ebx
	pop eax
	
	inc eax
	jmp sprawdz_znak

znaleziono_koniec:
	mov eax, 1
	mov ebx, 0
	int 0x80
