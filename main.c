#define F_CPU 1000000

#include <util/delay.h>       
#include <avr/io.h>
#include <avr/interrupt.h>


#define SPALTE1		PB6	
#define SPALTE2		PB7
#define SPALTE3		PD5

#define ZEILE		PORTD

#define ZEILE1		PD6
#define ZEILE2		PD7
#define	ZEILE3		PB0
#define number_of_frames	3


int16_t frame = 0;
int8_t zeil;
int8_t spalt;
int8_t spalten_cnt = 0;
int8_t modus;
int16_t about_ten_ms = 0;
int8_t cnter = 0;
int8_t matrix [3][3] = {	{1,1,1},
				{1,1,1},
				{1,1,1}	 
						};

// Schoenheitsfehler wurden behoben

int8_t matrix_movie [number_of_frames][3][3] =	{{	{0,0,0},
						 	{0,1,0},
							{0,0,0}	},
									
						{	{0,1,0},
							{1,0,1},
							{0,1,0}	},
									
						{	{1,1,1},
							{1,0,1},
							{1,1,1}	}	 
								};	


/*int8_t matrix_movie [number_of_frames][3][3] = {{	{0,1,1},
						 							{0,0,1},
													{1,1,1}	},
									
												{	{1,0,1},
						 							{1,0,1},
													{1,1,1}	},
									
												{	{1,0,0},
						 							{1,0,0},
													{1,1,1}	},
													
												{	{0,1,0},
						 							{0,1,0},
													{0,1,0}	},
												
												{	{0,1,0},
						 							{1,1,1},
													{1,0,1}	}
												};*/

			
							
ISR(INT0_vect)							//Interruptvektor
{
	_delay_us(200);
	modus++;
	if(modus>2)
	{
		modus = 1;
	}

}							

displayMatrix()				//Matrix wird auf der LED Matrix angezeigt
{

	switch(spalten_cnt)
	{
		case 0:		//ERSTE SPALTE
	
		if(matrix[0][0]==1)
			{PORTD |= (1 << ZEILE1);
			PORTB |= (1 << SPALTE1);}
					
		if(matrix[1][0]==1)
			{PORTD |= (1 << ZEILE2);
			PORTB |= (1 << SPALTE1);}
					
		if(matrix[2][0]==1)
			{PORTB |= (1 << ZEILE3);
			PORTB |= (1 << SPALTE1);}
		break;
		
		
		case 1:		//ZWEITE SPALTE
		if(matrix[0][1]==1)
			{PORTD |= (1 << ZEILE1);
			PORTB |= (1 << SPALTE2);}
		
		if(matrix[1][1]==1)
			{PORTD |= (1 << ZEILE2);
			PORTB |= (1 << SPALTE2);}
					
		if(matrix[2][1]==1)
			{PORTB |= (1 << ZEILE3);
			PORTB |= (1 << SPALTE2);}
		break;
		
		
		case 2:		//DRITTE SPALTE
	
		if(matrix[0][2]==1)
			{PORTD |= (1 << ZEILE1);
			PORTD |= (1 << SPALTE3);}			
	
		if(matrix[1][2]==1)
			{PORTD |= (1 << ZEILE2);
			PORTD |= (1 << SPALTE3);}
					
		if(matrix[2][2]==1)
		{PORTB |= (1 << ZEILE3);
		PORTD |= (1 << SPALTE3);}
		break;
	}
	_delay_ms(1);
	
	//Spalten aus
	PORTB &= ~(1 << SPALTE1);
	PORTB &= ~(1 << SPALTE2);
	PORTD &= ~(1 << SPALTE3);
	
	
	//Zeilen aus
	PORTD &= ~(1 << ZEILE1);
	PORTD &= ~(1 << ZEILE2);
	PORTB &= ~(1 << ZEILE3);
	
	spalten_cnt++;
	if(cnter<10)
	{
		cnter++;
	}else{
	about_ten_ms++;
	cnter = 0;
	}
	if(spalten_cnt>2)
	{
		spalten_cnt = 0;
	}
}

int main (void)
{

	
	
	MCUCR |= (1<<ISC01) | (1<<ISC00);           //Steigende Flanke von INT0 als auslöser
	GICR  |= (1<<INT0);					//Global Interrupt Flag für INT0
	sei();								//Interrupt aktivieren
	
	DDRD  &= ~(1<<PD2);  /* Pin PD2 als Eingang */
	PORTD |= (1<<PORTD2);    /* internen Pull-Up an PD2 aktivieren */
	
	
	
	DDRD |= (1 << PD5);	//Auf Ausgang schalten
	DDRD |= (1 << PD6);
	DDRD |= (1 << PD7);
	
	DDRB |= (1 << PB0);
	DDRB |= (1 << PB6);
	DDRB |= (1 << PB7);
	
	spalten_cnt = 0;
	
	//Spalten ein
	PORTB |= (1 << SPALTE1);
	PORTB |= (1 << SPALTE2);
	PORTD |= (1 << SPALTE3);
	
	
	//Zeilen ein
	PORTD |= (1 << ZEILE1);
	PORTD |= (1 << ZEILE2);
	PORTB |= (1 << ZEILE3);
	
	_delay_ms(2000);
	
	//Spalten aus
	PORTB &= ~(1 << SPALTE1);
	PORTB &= ~(1 << SPALTE2);
	PORTD &= ~(1 << SPALTE3);
	
	
	//Zeilen aus
	PORTD &= ~(1 << ZEILE1);
	PORTD &= ~(1 << ZEILE2);
	PORTB &= ~(1 << ZEILE3);
	
	_delay_ms(1000);
	
	while(1)
	{
		if(about_ten_ms>50)
		{
			for(zeil=0;zeil<3;zeil++)
			{
				for(spalt=0;spalt<3;spalt++)
				{
				matrix[spalt][zeil]=matrix_movie[frame][spalt][zeil];
				}
			}

			frame++;
			if(frame>(number_of_frames-1))
			{
				frame=0;
			}			
			
			about_ten_ms=0;
		}
	
	
	displayMatrix();
	}

	return 0;
}
