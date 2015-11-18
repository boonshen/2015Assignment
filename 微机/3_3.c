#include <ADUC812.H>

unsigned char ADCL = 0, ADCH = 0;					//ADC Low Byte & ADC High Byte
int ADC_val = 0;									//adc result before normalize
int norm_val = 100;									// normalize volatge : 0~5V to numeric 0~99
unsigned char deng = 2;

void delayMillisecond(int millisecond) {	
	int i = 0;
	for(i=0;i< millisecond *1000 ; i++){}			//1 tick ~ 1 us, 1 ms ~ 1000 tick
}

void ledSegmentDisplay(int display_value) {
	int j = 0;
	for(j = 0;j<1000;j++) {
		P2 = 96 + display_value%10;								//display first digit
		P2 = 80 + (display_value - display_value%10)/10;		//display second digit
		P2 = 48 + (display_value - display_value%100)/100;		//display third digit
	}
}


void main(){
	P0 = 2;					//initialize input for ADC0
	
	ADCCON1 = 0x78;	//01111000 , set adc clocks to 4 adc clocks ,Mclk divider to 8
	ADCCON2 = 0x00;	//configure adc register to select channel 0
	
	EA = 1;					//enable all interrupt
	EADC = 1;				//enable adc interrupt
	
	SCONV = 1;				//kickstart adc working in single conversion mode, 
							//note: sconv will be reset to zero once adc conversion is complete
							//set sconv to 1 every operation cycle, see below
	
	while(1) {
		if(deng == 2) deng = 4;
		else if(deng == 4) deng = 8;
		else if(deng == 8) deng = 16;
		else if(deng == 16) deng = 2;
		else deng = 2;
		P0 = deng;
		
		
		
		ADCL = ADCDATAL;	//fetch adc data in adc data registers
		ADCH = ADCDATAH;
		
		ADC_val = ADCH&0x0F;		//get first four bit from adc high byte , note: ADC is 4+8bit = 12 bit resolution
		ADC_val = ADC_val<<8;		//ADC_val left shift 8 bit, another note: the front 4bit of ADCH is channel id
		ADC_val = ADC_val + ADCL; 	//get last 8 bit  from adc low byte
		
		
		norm_val =  ADC_val / 4096.0 * 100  ;		//2^12 = 4096, normalize ADC value from 4096 to 100
		ledSegmentDisplay(norm_val);			//display normalize adc value	
		
		delayMillisecond(1 + norm_val*10);		//delay 1ms to 0.99s depend on the adc input
		
		SCONV = 1;								//start adc in single conversion mode again
	}
}