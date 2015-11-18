#include <ADUC812.H>

sbit K1 = P1^3;
sbit K2 = P1^6;

int i = 0;
unsigned char deng = 0;

int overflow_count = 0;
int one_sec_counter = 1;	//how many one second should pass
int second_divider = 15;	//one second divider, 2^16 * 15 ~~ 1s

void main()
{
	TMOD = 0x01;	//timer0 mode: 16bit timer
	EA = 1;			//enable interrupt
	P0 = 0;			//initialize LED
	P1 = 0;			//initialize button
	ET0 = 1;		//enable timer0 interrupt
	TR0 = 0;		//initialize timer0 to close
	
		
		while(1) {
			if( K1 == 1) TR0 = 1;	//when button K1 is pressed, switch on the timer0
			if (K2 == 1){			//when K2 is pressed, switch off the timer0 and reset all related variable
				one_sec_counter = 1;
				overflow_count = 0;
				TR0 = 0;
				deng = 0;
			}
			P0 = deng;				//loop and keep the target led on;
		}
	
}

void timer0() interrupt 1 {
	
	overflow_count++ ;												//count to multiple of 15
	if(overflow_count%(second_divider * one_sec_counter) == 0) {	//check whether the overflow count is divisible by 15*n
		one_sec_counter++;											//gradually increase light switching time
		overflow_count = 0;											//reset overflow count
		
		if(deng == 2) deng = 4;										//switch light
		else if(deng == 4) deng = 8;
		else if(deng == 8) deng =16;
		else if(deng == 16) deng = 2;
		else (deng = 2);
	}
	
	
	
}
