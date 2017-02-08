
//////////////////////////////////////////////////////////////////////////////////////////
// DZIALANIE:																			//
//  1. Ustawiamy flage nowego slowa jako 1, czyli kolejny znak to bedzie nowe slowo.	//
//  2. Sprawdzamy dany znak																//
//	 a) jesli znak=0, to zakoncz dzialanie bo koniec ciagu								//
//	 b) jesli znak to spacja, to ustaw flage ze bedzie nowe slowo i pomin znak			//
//   c) jesli znak!=spacja, to sprawdz czy zaczelo sie nowe slowo						//
//		1) jesli nowe (edx=1) to zgas flage edx i zwieksz aktualne numer slowa			//
//	 d) sprawdz czy aktualny nr slowa = szukany nr slowa								//
//		1) jesli tak, to drukuj															//
//	 e) wroc do sprawdzania nastepnego znaku											//
//  3. Koniec																			//
//																						//
// ARGUMENTY:																			//
//  esp+8 = szukany nr slowa (arg1)														//
//  esp+12 = wejsciowy ciag (arg2)														//
//																						//
// UZYTE REJESTRY:																		//
//  eax - aktualny znak																	//
//  ebx - nr aktualnego slowa															//
//  ecx - ciag wejsciowy																//
//  edx - flaga nowego slowa															//
//  edi - nr szukanego slowa															//
//////////////////////////////////////////////////////////////////////////////////////////

.intel_syntax noprefix
	.text
	.globl _start
_start:



//////////////////////////////////////////////////////////////////////////////////////////
// ATOI
	//ustaw 0 jako liczbe wynikowa
	mov edi, 0
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
	//wstaw kolejna ascii-cyfre do ebx. wstawiamy 1 bajt (8 bitow) do rejestru 32-bitowego, wiec tylko BL
	mov bl, [ecx+eax]
	//odejmij 48 od ascii-cyfry -> cyfra staje sie int, dzialamy dalej na rejestrze 8-bitowym
	sub bl, '0'
	//dodaj kolejna cyfre do liczby. uwaga: dodaj 8 bitow z tej liczby z dosunieciem zer, ebx jest juz prawidlowa wartoscia 32-bitowa
	movzx ebx, bl
	add edx, ebx
	//zwieksz pozycje znaku
	inc eax
	//skocz do petla
	jmp petla
	
	
	
out:
	//wstaw liczbe do edi
	mov edi, edx


/////////////////////////////////////////////////////////////////////////////////////////


//wstaw ciag wejsciowy do rejestru (ecx)
	mov ecx, [esp+12]
//ustaw aktualna pozycje znaku (eax) jako 0
	mov eax, 0 
//ustaw aktualny nr slowa (ebx) jako 0
	mov ebx, 0
//ustaw flage nowego slowa (edx) jako 1 == pojawi sie slowo
	mov edx, 1
	
//SPRAWDZ AKTUALNY ZNAK
sprawdz_znak:

	//jesli koniec -> skocz do koniec
	cmp byte ptr [ecx+eax], 0
	je znaleziono_koniec
	
	//jesli spacja -> skocz do znaleziono_spacje 
	cmp byte ptr [ecx+eax], ' '
	je znaleziono_spacje
	
	//jesli znak -> skocz do znaleziono_znak
	jmp znaleziono_znak
	
	
	
//ZNALEZIONO SPACJE	
znaleziono_spacje:	
	//jesli koniec -> skocz do koniec
	cmp byte ptr [ecx+eax], 0
	je znaleziono_koniec
	
	//jesli spacja, to przesun pozycje o jeden i sprawdz kolejny znak
	cmp byte ptr [ecx+eax], ' '
	je przesun_znak_i_ustaw_flage

	//kolejny znak!=spacja, wiec nowe slowo -> zwieksz nr slowa i do glownej petli
	jmp sprawdz_znak
	
//PRZESUN AKTUALNY ZNAK I USTAW FLAGE, ZE POJAWI SIE NOWE SLOWO
przesun_znak_i_ustaw_flage:
	inc eax
	mov edx, 1
	jmp znaleziono_spacje
	

//ZNALEZIONO ZNAK (ROZNY OD SPACJI I KONCA)	
znaleziono_znak:
	//jesli koniec -> skocz do koniec
	cmp byte ptr [ecx+eax], 0
	je znaleziono_koniec
	
	//sprawdz czy trafiono na nowe slowo (flaga edx == 1)
	cmp edx, 1
	je znaleziono_nowe_slowo
	
	//sprawdz, czy numer aktualnego slowa == numer szukanego slowa
	cmp ebx, edi
	
	//aktualne slowo jest szukanym -> drukuj znak
	je drukuj_znak

	//aktualne slowo nie jest szukanym -> przesun pozycje o jeden i sprawdz kolejny znak
	inc eax
	jmp sprawdz_znak
	
	
//ZGAS FLAGE NOWEGO SLOWA, ZWIEKSZ NR AKTUALNEGO SLOWA
znaleziono_nowe_slowo:
	mov edx, 0
	inc ebx
	jmp znaleziono_znak
	
	
	
	
//DRUKUJ AKTUALNY ZNAK
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


//WYCZYSC STOS I ZAKONCZ	
znaleziono_koniec:
	
	mov eax, 1
	mov ebx, 0
	int 0x80

