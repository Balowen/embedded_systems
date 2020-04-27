/*************************************************************************/
// symulator windy w trzypiętrowym budynku                                                  
/*************************************************************************/
#include "spsym.h"         	// Pomocnicze funkcje i deklaracje
#include <stdio.h>         	// Standardowe I/O

// --- Zmienne użytkownika ---
char stan = 0;
char stan_zawolania = 0;	// zmienne do switcha  wyswietlajacego gdzie przywołano winde
char stan_winda = 0;
char stan_poruszanie = 0;

int gdzieZawolano; p_gdzieZawolano;		// Liczba impulsów
int tim;					// Czas impulsu
int licz;					// Licznik impulsów

int liczPor = 0; timP;	// zmienne do switcha wyswietlającego poruszanie windy
float val;

int polozenieWindy = 1; winda_dostepna; timW; liczW;
int czas;

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
        gdzieZawolano = p_gdzieZawolano;
        stan_poruszanie = 0;
        winda_dostepna = 1;
        czas = 150;
        stan = 1;
        break;
    case 1: 							
        L1 = 0;
        if(winda_dostepna){
		    if (aK1 && !pK1) {gdzieZawolano = 1; stan =2;}             // 1 piętro
		    else if (aK2 && !pK2) {gdzieZawolano = 2;stan =2;}         // 2 piętro
		    else if (aK3 && !pK3) {gdzieZawolano = 3;stan =2;}        // 3 piętro
		    else {gdzieZawolano = 0;}
	    }
        break;
    case 2:
    	--czas;
    	if(!czas) {
    		polozenieWindy = gdzieZawolano;
    		liczPor = 0; winda_dostepna =1;
    		L3 = 0; stan_poruszanie = 0; stan = 0;
    		}
    	else if (polozenieWindy != gdzieZawolano) {winda_dostepna = 0; liczPor=4;}
    	else stan = 0;
    	break;
    }

    // --- Generowanie serii impulsów na L1 - gdzie przywołano windę---
    switch (stan_zawolania)
    {
    case 0: 
                // jesli nie zawolano tam gdzie znajduje sie winda
        if ( gdzieZawolano && (gdzieZawolano != polozenieWindy)) 
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
        else if (!tim && !licz ) { L1 = 0; winda_dostepna = 1; tim = 0; stan_zawolania = 0; } // =>koniec
        break;
    }
    
    
    
    
    switch(stan_winda)				// na którym piętrze znajduje się winda	
    {
    
       case 0: // Stan jałowy (oczekiwanie), L1=0;
            if(polozenieWindy && stan != 2) {timW=10; liczW=polozenieWindy-1; L2=1; stan_winda=1;}
            break;
       case 1: // Impuls, L1=1;
            --timW;
            if(!timW && liczW) {timW=10; L2=0; stan_winda=2;}          // =>krótka przerwa
            else if(!timW && !liczW) {timW=30; L2=0; stan_winda=2;}   // =>długa przerwa
            break;
       case 2: // Przerwa, L1=0;
            --timW;
            if(!timW && liczW) {timW=10; --liczW; L2=1; stan_winda=1;}  // =>kolejny impuls
            else if(!timW && !liczW) {L2=0; stan_winda=0;} // =>koniec
            break;
    }
    
    
        // --- zagęszczane impulsy --- poruszanie się windy
    switch(stan_poruszanie)
    {
        case 0: // Oczekiwanie
            L3=0;           // OFF
            if(liczPor) {timP=2; val=liczPor; stan_poruszanie=1;}        // => impuls
            break;
        case 1: // Impuls
            L3=1;           // ON
            if(timP) --timP;
            if(!timP) {timP=2*(val+1); stan_poruszanie=2;}              // => przerwa
            break;
        case 2: // Przerwa
            L3=0;           // OFF
            if(timP) --timP;
            if(!timP && val>0.0) {timP=2; val-=0.5; stan_poruszanie=1;} // => impuls
            else if(!timP && val<=0.0) {timP=2; stan_poruszanie=1;}     // => impuls
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
    bTab[12] = czas;
    bTab[13] = gdzieZawolano;
    bTab[14] = tim;
    bTab[15] = polozenieWindy;
}
#endif
