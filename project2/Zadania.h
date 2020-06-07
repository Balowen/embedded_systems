/////////////////////////////////////////////////////////////////////////////////////
// Bart³omiej Nawój -			Program testowy symuluj¹cy produkcje napoju		   //
/////////////////////////////////////////////////////////////////////////////////////
bool auto_mode=1;
int tm1 = 0, tm2a=0, tm2b=0, tm3=0, tm4=0,  wybrany_zawor = 0;
char stan1=1, stan2=1, stan3=0, stan_manual=0, stan_mieszadelko=1;

// Program testowy symuluj¹cy produkcje napoju
void test_A(void)
{
	if(aK4&&!pK4){
			if(auto_mode==1) auto_mode = 0;	// wlacz tryb manual
			else {
				// wlacz tryb auto
				auto_mode = 1;
				}
		}

	if(auto_mode){
		//auto mode
		switch(stan1){
			/*ZBIORNIK 1 / LEWY GÓRNY*/
			case 1: Z1 = 1;
					if(X2) {tm1=10; stan1=2;}
					break;
			case 2: Z1 = 1;
					if(!X2) stan1=1;
					else if(!tm1) {tm1=4; stan1=3;}
					else if(X1) stan1=4;
					break;
			case 3: Z1 = 0;
					if(!tm1) {tm1=10; stan1=2;}
					else if(!X2) stan1=1;
					break;
			case 4: Z1 = 0;
					if(!X1) {tm1=10; stan1=2;}
					break;
			}
		if(tm1) --tm1;


		switch(stan2){
			/*ZBIORNIK 2 / PRAWY GÓRNY*/
			case 1: Z2 = 1;
					if(X4) {tm2a=6; tm2b=40; stan2=2;}
					break;
			case 2: Z2 = 1;
					if(!X4) stan2=1;
					else if(!tm2a) {tm2a=4; stan2=3;}
					else if(!tm2b || X3) stan2=4;
					break;
			case 3: Z2 = 0;
					if(!tm2a) {tm2a=6; stan2=2;}
					else if(!X4) stan2=1;
					break;
			case 4: Z2 = 0;
					if(!X4) stan2=1;
					break;
			}
		if(tm2a) --tm2a;
		if(tm2b) --tm2b;

	
		switch(stan3){
			// ZBIORNIK 3 / DOLNY (bez mieszadelka)
			case 0:	// czekanie a¿ górne zbiorniki siê nape³ni¹
					Z3=0; Z4=0; Z5=0; G=0;
					if(stan1==4 && stan2 != 1) stan3=1;
					break;
			case 1: Z3=1; Z4=0; Z5=0; G=0;
					if(X7) {tm3=12; stan3=2;}
					break;
			case 2: Z3=1; Z4=0; Z5=0; G=0;
					if(!tm3) {tm3=4; stan3=3;}
					else if(X6) {tm3=4; stan3=4;}
					break;
			case 3: Z3=0; Z4=1; Z5=0; G=0;
					if(!tm3) {tm3=12; stan3=2;}
					break;
			case 4: Z3=1; Z4=0; Z5=0; G=0;
					if(!tm3) {tm3=4; stan3=5;}
					else if(X5) stan3=6;
					break;
			case 5: Z3=0; Z4=1; Z5=0; G=0;
					if(!tm3) {tm3=4; stan3=4;}
					else if(X5) stan3=6;
					break;
			case 6: Z3=0; Z4=0; Z5=0; G=1;
					if(T) {tm3=4; stan3=7;}
					break;
			case 7: Z3=0; Z4=0; Z5=1; G=0;
					if(!tm3) {tm3=2; stan3=8;}
					else if(!X7) stan3=1;
					break;
			case 8: Z3=0; Z4=0; Z5=0; G=0;
					if(!tm3) {tm3=4; stan3=7;}
					break;
			}	
		if(tm3) --tm3;

		switch(stan_mieszadelko){
			// switch dla ukladu sterujacego mieszadelkiem w trybie auto
			case 1: M=0;
				if(stan3==4) {tm4 = 4; stan_mieszadelko=2;}
				break;
			case 2: M=1;
				if(!tm4) {tm4 = 4; stan_mieszadelko=3;}
				else if(stan3==6) stan_mieszadelko=4;
				break;
			case 3: M=0;
				if(!tm4) {tm4 = 4; stan_mieszadelko=2;}
				else if(stan3==6) stan_mieszadelko=4;
				break;
			case 4: M=1;
				if(stan3==7) {tm4=8; stan_mieszadelko=5;}
				break;
			case 5: M=1;
				if(!tm4) {tm4=6; stan_mieszadelko = 6;}
				else if(stan3==1) stan_mieszadelko=1;
				break;
			case 6: M=0;
				if(!tm4) {tm4=8; stan_mieszadelko = 5;}
				else if(stan3==1) stan_mieszadelko = 1;
				break;
		}
		if(tm4) --tm4;

		sprintf(buf,"K1=%dK2=%dK3=%dK4=%d ",(int)aK1,(int)aK2,(int)aK3,(int)aK4);
		LCD_xy(1,1); LCD_puts(buf);
		sprintf(buf,"L1=%dL2=%dL3=%dL4=%d ",(int)L1,(int)L2,(int)L3,(int)L4);
		LCD_xy(1,2); LCD_puts(buf);
	}
	else {
		// tryb manualny		
		//automode = false

		switch(stan_manual){
			case 0: // oczekiwanie i zliczanie wyboru zaworu (aK1)
				Z1=0; Z2=0; Z3=0; Z4=0; Z5=0; G=0; M=0;
				if(aK1&&!pK1) {
					if(wybrany_zawor>=6) wybrany_zawor = 0;
					else ++wybrany_zawor;
				}
				if(aK2) stan_manual=1;	// trzymanie aK2 w³¹cza wybrany zawór (G i M równie¿)
				break;
			case 1: wybrany_zawor == 0 ? Z1=1 : 0;	// Otwórz Z1 jeœli wybrany_zawor == 0
					wybrany_zawor == 1 ? Z2=1 : 0;	// Otwórz Z2 jeœli wybrany_zawor == 1
					wybrany_zawor == 2 ? Z3=1 : 0;	// Otwórz Z3 jeœli wybrany_zawor == 2
					wybrany_zawor == 3 ? Z4=1 : 0;	// Otwórz Z4 jeœli wybrany_zawor == 3
					wybrany_zawor == 4 ? Z5=1 : 0;	// Otwórz Z5 jeœli wybrany_zawor == 4
					wybrany_zawor == 5 ? M=1 : 0;	// W³¹cz M jeœli wybrany_zawor == 5
					wybrany_zawor == 6 ? G=1 : 0;	// W³¹cz G jeœli wybrany_zawor == 6

					if(!aK2) stan_manual=0;			// wróæ do stanu 0 jeœli puszczono aK2
					break;
			}
		
		// LCD, wyœwietl wybrany_zawor+1 dla czytelnoœci / Z1 to wtedy zawor 1, a nie 0
		sprintf(buf,"wybrany zawor=%d ",((int)wybrany_zawor+1));	
		LCD_xy(1,1); LCD_puts(buf);
		sprintf(buf,"L1=%dL2=%dL3=%dL4=%d ",(int)L1,(int)L2,(int)L3,(int)L4);
		LCD_xy(1,2); LCD_puts(buf);
	}
	


	
}
