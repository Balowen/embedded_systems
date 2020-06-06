/////////////////////////////////////////////////////////////////////////////////////
// Bart³omiej Nawój -			Program testowy symuluj¹cy produkcje napoju		   //
/////////////////////////////////////////////////////////////////////////////////////
int tm1 = 0, tm2a=0, tm2b=0;
char stan1=1, stan2=1;

// Program testowy symuluj¹cy produkcje napoju
void test_A(void)
{
	// manual mode
	Z1 = aK1 ? 1 : 0;				// Otwórz Z1 jeœli naciœniêty K1
	Z2 = aK2 ? 1 : 0;				// Otwórz Z2 jeœli naciœniêty K2
	Z3 = aK3 ? 1 : 0;				// Otwórz Z3 jeœli naciœniêty K3
	Z4 = aK4 ? 1 : 0;				// Otwórz Z4 jeœli naciœniêty K4
	Z5 = aK5 ? 1 : 0;				// Otwórz Z5
	//M  = 0x1 ? 1 : 0;				// W³¹cz  M (mieszanie)
	G  = aK6 ? 1 : 0;				// Wy³¹cz G (grzanie)

	//if(!tm1) {M = !M; tm1=2*T1s;}	// Mieszanie cykliczne


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
	
	
	sprintf(buf,"K1=%dK2=%dK3=%dK4=%d ",(int)aK1,(int)aK2,(int)aK3,(int)aK4);
	LCD_xy(1,1); LCD_puts(buf);
	sprintf(buf,"L1=%dL2=%dL3=%dL4=%d ",(int)L1,(int)L2,(int)L3,(int)L4);
	LCD_xy(1,2); LCD_puts(buf);

	
}
