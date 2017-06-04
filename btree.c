// struktura wezla B-drzewa i przyklad zapisu i odczytu na plik
// budowanie B-drzewa o zadanej wysokosci i drukowanie


#include <stdio.h>
#define T 3   // stopien B-drzewa

typedef struct{
  short n;        //ilosc kluczy (-1 oznacza wezel usuniety)
  short leaf;     // czy lisc
  int k[2*T-1];   // klucze
  int c[2*T];     // wskazniki do synow (pozycje w pliku: 0,1,2 ...)
  int pozycja;		// pozycja w pliku
} Wezel;

int rozmiarw = sizeof(Wezel); // rozmiar wezla w bajtach
FILE *drzewo;  // plik drzewa (zawierajacy wezly)

int wolnaPozycja = 0; // nastepna wolna pozycja w pliku

void zapisz(int i,Wezel *w){
  // zapisuje *w jako i-ty zapis w pliku drzewa
  fseek(drzewo,(long)i*rozmiarw,SEEK_SET);
  fwrite(w,rozmiarw,1,drzewo);
  //  printf("z%d ",i);
}

void odczytaj(int i,Wezel *w){
  // odczytuje i-ty zapis w pliku drzewa i wpisuje do *w
  fseek(drzewo,(long)i*rozmiarw,SEEK_SET);
  fread(w,rozmiarw,1,drzewo);
  //  printf("o%d ",i);
}

void usun(int i){
  // usuwa i-ty zapis w pliku drzewa
  // w tej wersji nie wykorzystujemy usunietych pozycji,
  // tylko zaznaczamy jako usuniete
  Wezel w;
  odczytaj(i,&w);
  w.n=-1;
  zapisz(i,&w);
}

void drukujB(int r, int p){
  // drukuje B-drzewo o korzeniu r (pozycja w pliku)
  // wydruk przesuniety o p w prawo
  Wezel w;
  int i,j;
  odczytaj(r,&w);
  if (w.leaf){
    for (i=0;i<p;i++) printf(" ");
    for (i=0;i<w.n;i++) printf("%d ",w.k[i]);
    printf("\n");
  } else {
    drukujB(w.c[w.n],p+4);
    for (i=w.n-1;i>=0;i--){
      for (j=0;j<p;j++) printf(" ");
      printf("%d\n",w.k[i]);
      drukujB(w.c[i],p+4);
    }
  }
}

int stworzB()
{
	// tworzymy korzen dla nowego drzewa
	Wezel korzen;
	korzen.n = 0;
	korzen.leaf = 1;
	
	// ustawiamy pozycje w pliku
	korzen.pozycja = wolnaPozycja;
	wolnaPozycja++;
	
	
	// ustaw brak synow
	for (int i = 0; i < 2*T; i++) 
	{
		korzen.c[i] = -1;
	}
	
	
	// zapisz korzen do pliku
	zapisz(korzen.pozycja, &korzen);
	
	// zwroc pozycje korzenia w pliku (bedzie potrzebna potem)
	return korzen.pozycja;
}

// grafika wyjasniajaca
// http://www.geeksforgeeks.org/b-tree-set-1-insert-2/
void bRozbijSyna(Wezel x, int index)
{
	// mamy sytuacje ze jest sobie wezel X
	// wezel X ma syna Y, ktorego chcemy rozbic na dwa wezly (Z i Y)
	// Z bedzie trzymalo lewa czesc Y, natomiast Y bedzie mialo prawa strone Y
	
	// czyli z X(ojciec) - Y(syn)
	// robimy X(ojciec) - Z(lewy syn) + Y(prawy syn)
	
	
	// wezel ktory bedzie trzymac (t-1) kluczy Y (lewa strone)
	Wezel z;
	
	// stworz zmienna dla wezla Y, czyli syna X
	Wezel y;
	// skopiuj do Y syna X
	odczytaj(x.c[index], &y);
	
	// jesli Y byl lisciem, to Z tez powinien byc (logiczne)
	z.leaf = y.leaf;
	// Z ma miec 2 klucze, tak samo jak Y
	z.n = T - 1;
	// przyporzadkuj pozycje w pliku dla Z
	z.pozycja = wolnaPozycja;
	wolnaPozycja++;
	
	// przesuwanie kluczy
	// np Y = [1][2][3][4][5]
	for(int i = 0; i < T - 1; ++i)
	{
		// wpisz do Z lewa czesc kluczy Y => Z=[1][2]
		z.k[i] = y.k[i];
		// przesun prawa czesc kluczy Y w lewa strone => Y=[4][5]
		y.k[i] = y.k[i+T];
		// [3] trafi potem do wezla X, czyli ojca
	}
	
	// przesuwanie synow
	// analogicznie jak wyzej, tylko dla synow mamy T a nie T-1
	for(int i = 0; i < T; ++i)
	{
		// wpisz do Z lewych synow Y
		z.c[i] = y.c[i];
		// przesun prawych synow Y w lewo
		y.c[i] = y.c[i+T];
	}
	
	// teraz Y ma juz tylko 2 klucze
	y.n = T-1;
	

	// przesuwamy synow X w prawo
	// bo w miejscu X[index] wyladuje srodkowy klucz z Y
	// ze swoimi dwoma synami - musimy mu zrobic miejsce
	for(int i = x.n; i >= index; --i)
	{
		// przesuwamy synow w prawo
		x.c[i+1] = x.c[i];
	}
	
	// teraz X[index] ma po lewej wezel Z, a po prawej wezel Y
	x.c[index] = z.pozycja;
	
	// przesuwamy klucze X w prawo
	// aby zrobic miejsce dla srodkowego klucza z wezla Y (klucz powedrowal do gory, do X[index])
	for(int i = x.n; i > index; --i)
	{
		// przesuwamy klucze w prawo
		x.k[i+1] = x.k[i];
	}
	
	// srodkowy klucz z Y wrzucamy do wezla X, w miejscu X[index]
	x.k[index] = y.k[T-1];
	// teraz wezel X ma o 1 wiecej kluczy
	x.n += 1;

	// zapisz w pliku
	zapisz(y.pozycja, &y);
	zapisz(z.pozycja, &z);
	zapisz(x.pozycja, &x);
}

void bInsertNiepelny(Wezel wybrany, int wstawianyKlucz)
{
	// pozycja ostatniego (rightmost) klucza w wezle
	// np. gdy 4 klucze to ostatni jest k[3]
	// wiec i = n-1 (3 = 4-1)
	int i = wybrany.n - 1;
	
	if(wybrany.leaf == 1) // jesli wezel do ktorego wstawiamy jest lisciem
	{
		// znajdz miejsce dla nowego klucza
		// czyli przesun wieksze klucz od wstawianyKlucz w prawo
		while(i >= 0 && wybrany.k[i] > wstawianyKlucz)
		{
			// przesun klucze by zrobic miejsce dla nowego klucza
			wybrany.k[i+1] = wybrany.k[i];
			i--;
		}

		// wstaw nowy klucz do wezla w odpowiednim miejscu
		wybrany.k[i+1] = wstawianyKlucz;
		// zwieksz liczbe kluczy w wezle
		wybrany.n += 1;
		// zapisz wezel do pliku
		zapisz(wybrany.pozycja, &wybrany);
	}
	else // jesli wezel do ktorego wstawiamy nie jest lisciem
	{
		// znajdz wezel-klucz (syna) do ktorego powinien trafic nowy klucz
		while(i >= 0 && wybrany.k[i] > wstawianyKlucz)
		{
			i--;
		}
		
		// wczytaj syna ktory jest pod tym kluczem
		Wezel syn;
		// klucz jest na pozycji i, wiec syn jest i+1
		odczytaj(wybrany.c[i+1], &syn);
		
		
		// teraz musimy sprawdzic, czy mozemy dodac klucz do tego syna
		// jesli niepelny, to wstawiamy do niego
		// jesli pelny, to rozbijamy i wstawiamy do jego lewego lub prawego syna
		
		// jesli syn tez jest pelny (5 kluczy), to musimy rozbic
		if(syn.n == 2*T - 1)
		{
			// rozbij syna wybranego wezla
			bRozbijSyna(wybrany, i+1);
			
			// sprawdz do ktorego syna powinnismy wstawiac klucz
			if(wstawianyKlucz > wybrany.k[i])
			{
				// wstawianyKlucz wiekszy - wczytaj prawego syna
				odczytaj(wybrany.c[i+1], &syn);
			}
			else
			{
				// wstawianyKlucz mniejszy - wczytaj lewego syna
				odczytaj(wybrany.c[i], &syn);
			}
		}	
		
		// sprobuj wstawic klucz do wybranego syna
		bInsertNiepelny(syn, wstawianyKlucz);
	}
}


void bInsert(int* pozycjaWPliku, int wstawianyKlucz)
{	
	// korzen
	Wezel wybrany;
	// wczytaj wybrany wezel (z podanej pozycji w pliku)
	odczytaj(*pozycjaWPliku, &wybrany);
	
	if(wybrany.n == 2*T - 1) // jesli wezel jest pelny (ma 5 kluczy)
	{
		// tworzymy wezel-ojca do ktorego trafi srodkowy z 2*T-1 kluczy
		// np. bylo [1] [3] [5] [7] [9]
		// to [5] stanie sie wezlem-ojcem (czyli nowyOjciec), 
		// a [1][3] (lewa czesc wezla  "wybrany") 
		// oraz [7][9] (prawa czesc wezla "wybrany")
		// stana sie jego synami ([1][3] lewy, [7][9] prawy)
		
		// tworzymy wezel dla nowego ojca
		Wezel nowyOjciec;
		nowyOjciec.leaf = 0;
		// ilosc synow nowego ojca uaktualnimy w trakcie rozbicia syna (wybrany)
		nowyOjciec.n = 0;
		
		// ustawiamy pozycje nowegoOjca w pliku
		nowyOjciec.pozycja = wolnaPozycja;
		wolnaPozycja++;
		
		// synem nowego ojca staje sie nasz wybrany wezel
		nowyOjciec.c[0] = wybrany.pozycja;
		// pozycja nowego korzenia w pliku
		*pozycjaWPliku = nowyOjciec.pozycja;
		
		// rozbij syna wezla nowyOjciec (czyli wezel "wybrany")
		// do nowyOjciec powedruje jeden klucz
		// a wybrany zostanie rozbity na dwa wezly
		// ktore beda synami nowyOjciec
		bRozbijSyna(nowyOjciec, 0);
		
		// jako ze robilismy rozbicie
		// to ojciec (wczesniej wybrany, teraz nowyOjciec)
		// nie jest juz pelny - sprobuj wstawic klucz (do korzenia lub nizej)
		bInsertNiepelny(nowyOjciec, wstawianyKlucz);
	}
	else // jesli wezel nie jest pelny, to mozemy wstawic
	{
		// wstaw klucz do wybranego wezla (lub jego syna)
		bInsertNiepelny(wybrany, wstawianyKlucz);
	}
}


int szukajB(int korzen, int szukanyKlucz)
{
	// wczytaj aktualny wezel
	Wezel wezel;
	int i = 0;
	odczytaj(korzen, &wezel);
	
	// znajdz pierwszy klucz wiekszy badz rowny szukanyKlucz
	while(i < wezel.n && szukanyKlucz > wezel.k[i])
	{
		i++;
	}
	
	// jesli znaleziony klucz jest rowny szukanyKlucz to zwroc pozycje
	if (i < wezel.n && szukanyKlucz == wezel.k[i])
	{
		return korzen;
	}
	
	// jesli nie znaleziono klucza a wezel jest lisciem
	if(wezel.leaf == 1)
	{
		return -1; // brak
	}
	else // jesli nie jest lisciem, to zejdz nizej
	{
		return szukajB(wezel.c[i], szukanyKlucz);
	}
}

void drukujSzukaj(int korzen, int szukanyKlucz)
{
	printf("## SZUKAM KLUCZA %i \n", szukanyKlucz);
	
	int zwroconyWezel = szukajB(korzen, szukanyKlucz);
	if(zwroconyWezel == -1)
	{
		printf("### NIE ZNALEZIONO \n");
	}
	else
	{
		printf("### ZNALEZIONO (wezel nr %i)\n", zwroconyWezel);
		drukujB(zwroconyWezel, 2);
	}
	printf("\n");
}



void main()
{	
    drzewo = fopen("bdrzewo2.txt","w+");
	// STWORZ KORZEN
	int korzen = stworzB(); // pozycja korzenia w pliku

	
	// ####################################################################################	

	
	/*
	
	// TESTOWE DANE #1
	bInsert(&korzen, 1);
	bInsert(&korzen, 2);
	bInsert(&korzen, 3);
	bInsert(&korzen, 4);
	bInsert(&korzen, 5);
	bInsert(&korzen, 6);
	bInsert(&korzen, 7);
	bInsert(&korzen, 8);
	bInsert(&korzen, 9);
	bInsert(&korzen, 10);
	bInsert(&korzen, 11);
	bInsert(&korzen, 12);
	bInsert(&korzen, 13);
	bInsert(&korzen, 14);
	bInsert(&korzen, 15);
	
	// */
	
	// ####################################################################################
	
	
	/*
	
	// TESTOWE DANE #2
	bInsert(&korzen, 9);
	bInsert(&korzen, 7);
	bInsert(&korzen, 5);
	bInsert(&korzen, 3);
	
	// */
	
	// ####################################################################################
	
	
	// /*
	
	// TESTOWE DANE #3
	// od 1 do 30 (bez 15)
	bInsert(&korzen, 2);
	bInsert(&korzen, 1);
	bInsert(&korzen, 3);
	bInsert(&korzen, 4);
	bInsert(&korzen, 5);
	bInsert(&korzen, 11);
	bInsert(&korzen, 12);
	bInsert(&korzen, 7);
	bInsert(&korzen, 9);
	bInsert(&korzen, 8);
	bInsert(&korzen, 10);
	bInsert(&korzen, 6);
	bInsert(&korzen, 14);
	bInsert(&korzen, 13);
	bInsert(&korzen, 17);
	bInsert(&korzen, 16);
	bInsert(&korzen, 18);
	bInsert(&korzen, 21);
	bInsert(&korzen, 20);
	bInsert(&korzen, 19);
	bInsert(&korzen, 25);
	bInsert(&korzen, 24);
	bInsert(&korzen, 23);
	bInsert(&korzen, 22);
	// BEDZIE KONKRETNE ROZBICIE
	//bInsert(&korzen, 26);
	//bInsert(&korzen, 27);
	//bInsert(&korzen, 28);
	//bInsert(&korzen, 29);
	//bInsert(&korzen, 30);
	// MOZEMY DODAC 15 zeby znajdywalo wszystkie 3 szukane
	//bInsert(&korzen, 15);
	
	// */
	
	// ####################################################################################
	
	
	
	
	// drukuj drzewo
	printf("************************************************************\n");
    drukujB(korzen,0);
	printf("************************************************************\n");
	
	// szukaj
	printf("************************************************************\n");
	drukujSzukaj(korzen, 3);
	drukujSzukaj(korzen, 9);
	drukujSzukaj(korzen, 15);
	printf("************************************************************\n");

	
    fclose(drzewo);
}
