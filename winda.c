/*************************************************************************/
// symulator windy w trzypiętrowym budynku                                                  
/*************************************************************************/
#include "spsym.h"         	// Pomocnicze funkcje i deklaracje
#include <stdio.h>         	// Standardowe I/O

// --- Zmienne użytkownika ---
char stan = 0;
char stan_zawolania = 0;	// zmienne do switcha  wyswietlajacego stan przywołania windy
int gdzieZawolano; p_gdzieZawolano;		// Liczba impulsów
int tim;					// Czas impulsu
int licz;					// Licznik impulsów
int polozenieWindy; winda_dostepna;
void prolog(void)			// Inicjowanie programu (jednorazowo przy starcie)
{
    L1 = L2 = L3 = L4 = 0;         	// Zgaszenie LED-ów
    L5 = L6 = L7 = L8 = 0;         	// Zgaszenie LED-ów

   //pK1=aK1; pK2=aK2; pK3=aK3;
}

void oblicz(void)            // Kod użytkownika wykonywany cyklicznie
{
    // --- Zadawanie liczby impulsów - gdzie przywołano windę ---


    switch (stan)
    {
    case 0:											// Stan startowy
        polozenieWindy = 1;
        gdzieZawolano = 0;
        winda_dostepna = 1;
        stan = 1;
        break;
    case 1: 							
        L1 = 0;
	    if (aK1 && !pK1) gdzieZawolano = 1;             // 1 piętro
	    else if (aK2 && !pK2) gdzieZawolano = 2;         // 2 piętro
	    else if (aK3 && !pK3) gdzieZawolano = 3;        // 3 piętro
	    else {gdzieZawolano = 0;}
        break;
    }

    // --- Generowanie serii impulsów na L1 - gdzie przywołano windę---
    switch (stan_zawolania)
    {
    case 0: 
                // jesli nie zawolano tam gdzie znajduje sie winda
        if (gdzieZawolano && (gdzieZawolano != polozenieWindy)) 
        {
        	winda_dostepna = 0;
            tim = 2;							// Impuls, L1=1;
            licz = gdzieZawolano - 1; L1 = 1; stan_zawolania = 1;
        }
        break;
    case 1: 
        --tim;
        if (!tim && licz) { tim = 4; L1 = 0; stan_zawolania = 2; }          // =>krótka przerwa
        else if (!tim && !licz) { tim = 20; L1 = 0; stan_zawolania = 2; }   // =>długa przerwa
        break;
    case 2: // Przerwa, L1=0;
        --tim;
        if (!tim && licz) { tim = 2; --licz; L1 = 1; stan_zawolania = 1; }  // =>kolejny impuls
        else if (!tim && !licz && (polozenieWindy == gdzieZawolano)) { L1 = 0; winda_dostepna = 1; stan_zawolania = 0; } // =>koniec
        break;
    }
    
    

    
    
    

    pK1 = aK1; pK2 = aK2; pK3 = aK3;
    p_gdzieZawolano = gdzieZawolano;
    
            	    // jesli winda jeszcze nie dojechała do miejsca zawolana
}

void przerwanie(void)      	// Obsługs przerwania od układu czasowo-licznikowego
{}

void komunikacja(void)		// Obsługa komunikacji szeregowej
{}

#ifdef Symulator_PC
void wykres(void)			// Dane do tabeli i wykresu (dot. symulacji obiektu)
{
    aTab[0] = (int)(AI[0] / 10.0);
    aTab[1] = (int)(AI[1] / 5.0);
    aTab[2] = AI[7];
    aTab[3] = 0.0;
    aTab[4] = AO[0];
    aTab[5] = AO[7];
    aTab[6] = -120;
    aTab[7] = 120;

    bTab[0] = (int)aK1;
    bTab[1] = (int)aK2;
    bTab[2] = (int)aK3;
    bTab[3] = (int)aK4;
    bTab[4] = (int)aK5;
    bTab[5] = (int)aK6;
    bTab[6] = (int)L1;
    bTab[7] = (int)L2;
    bTab[8] = (int)L3;
    bTab[9] = (int)L4;
    bTab[10] = (int)L5;
    bTab[11] = (int)L6;
    bTab[12] = stan;
    bTab[13] = gdzieZawolano;
    bTab[14] = tim;
    bTab[15] = licz;
}
#endif
