#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/divider.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "pico/util/queue.h"
#include "hardware/adc.h"



#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9
#define D10 10 
#define D11 11
#define D12 12 
#define D13 13 
#define D14 14 
#define D15 15 
#define I0 16 

#define DELAY 10 

int ticks_on[16];
int ticks_off[16];
int phase[16];

void lamps_thread()
{
    uint64_t wanted;
    int tick[16];
    int ton[16];
    int toff[16];
    int x,count;

    for (x=0;x<16;x++)
    {	    
    	tick[x]=0; 
	ton[x]=0;
	toff[x]=0;
    }
    wanted=time_us_64();
    while (1)
    {
   	wanted+=DELAY;
	count=0;
	while (time_us_64()<wanted)
	{
		sleep_us(1);
		//count++;
	}
	//if (count<1){ gpio_put(PICO_DEFAULT_LED_PIN,1);}
	for (x=0;x<16;x+=2)
	{
		if (tick[x]==ton[x]){ 
			gpio_put(D0+x,0);
			toff[x]=ticks_off[x];
			ton[x]=ticks_on[x];
			tick[x]=ton[x];
		}else if (tick[x]==toff[x]){
			gpio_put(D0+x,1);
		}

		int px ;
		px=phase[x]+tick[x];
		if (px>toff[x]){px-=toff[x];}
		if (px==ton[x]){ 
			gpio_put(D0+x+1,0);
		}else if (px==toff[x]){
			gpio_put(D0+x+1,1);
		} 

		tick[x]++;
		if (tick[x]>toff[x]){ tick[x]=0;}
	}
    }
}

int init_pins ()
{

     	adc_init();
     	adc_gpio_init(26);
     	adc_gpio_init(27);
     	adc_gpio_init(28);

    	gpio_init(I0); gpio_set_dir(I0,GPIO_IN); gpio_pull_down(I0); // button 0 

    	gpio_init(D0); gpio_set_dir(D0,GPIO_OUT); //Driver 0 
    	gpio_init(D1); gpio_set_dir(D1,GPIO_OUT); //Driver 1 
    	gpio_init(D2); gpio_set_dir(D2,GPIO_OUT); //Driver 2 
    	gpio_init(D3); gpio_set_dir(D3,GPIO_OUT); //Driver 3 
    	gpio_init(D4); gpio_set_dir(D4,GPIO_OUT); //Driver 4 
    	gpio_init(D5); gpio_set_dir(D5,GPIO_OUT); //Driver 5 
    	gpio_init(D6); gpio_set_dir(D6,GPIO_OUT); //Driver 6 
    	gpio_init(D7); gpio_set_dir(D7,GPIO_OUT); //Driver 7 
    	gpio_init(D8); gpio_set_dir(D8,GPIO_OUT); //Driver 8 
    	gpio_init(D9); gpio_set_dir(D9,GPIO_OUT); //Driver 9 
    	gpio_init(D10); gpio_set_dir(D10,GPIO_OUT); //Driver 10 
    	gpio_init(D11); gpio_set_dir(D11,GPIO_OUT); //Driver 11 
    	gpio_init(D12); gpio_set_dir(D12,GPIO_OUT); //Driver 12 
    	gpio_init(D13); gpio_set_dir(D13,GPIO_OUT); //Driver 13 
    	gpio_init(D14); gpio_set_dir(D14,GPIO_OUT); //Driver 14 
    	gpio_init(D15); gpio_set_dir(D15,GPIO_OUT); //Driver 15 
    	gpio_init(PICO_DEFAULT_LED_PIN); gpio_set_dir(PICO_DEFAULT_LED_PIN,GPIO_OUT); // LED 
	gpio_set_slew_rate(D0, GPIO_SLEW_RATE_SLOW);
	gpio_set_slew_rate(D1, GPIO_SLEW_RATE_SLOW);
	gpio_set_slew_rate(D2, GPIO_SLEW_RATE_SLOW);
	gpio_set_slew_rate(D3, GPIO_SLEW_RATE_SLOW);
	gpio_set_slew_rate(D4, GPIO_SLEW_RATE_SLOW);
	gpio_set_slew_rate(D5, GPIO_SLEW_RATE_SLOW);
	gpio_set_slew_rate(D6, GPIO_SLEW_RATE_SLOW);
	gpio_set_slew_rate(D7, GPIO_SLEW_RATE_SLOW);
	gpio_set_slew_rate(D8, GPIO_SLEW_RATE_SLOW);
	gpio_set_slew_rate(D9, GPIO_SLEW_RATE_SLOW);
	gpio_set_slew_rate(D10, GPIO_SLEW_RATE_SLOW);
	gpio_set_slew_rate(D11, GPIO_SLEW_RATE_SLOW);
	gpio_set_slew_rate(D12, GPIO_SLEW_RATE_SLOW);
	gpio_set_slew_rate(D13, GPIO_SLEW_RATE_SLOW);
	gpio_set_slew_rate(D14, GPIO_SLEW_RATE_SLOW);
	gpio_set_slew_rate(D15, GPIO_SLEW_RATE_SLOW);
}

int main()
{
	// adding stdio causes interrupts and slows everything down. Debug only.
    	stdio_init_all();
	init_pins();

	sleep_ms(100);
	
	int x,attack,press,release;
	float a,cent,fact;
	cent=140;
	fact=0;
	attack=0;
	release=1;

	for (x=0;x<16;x++)
	{
		ticks_on[x]=1000000/(2*cent*DELAY);
		ticks_off[x]=ticks_on[x]*2;
		phase[x]=0;
	}

	a=1;
    	multicore_launch_core1(lamps_thread);
	float ncent;
	float nfact;
	float dec;

	dec=1;

	while (1)
	{
		//gpio_put(PICO_DEFAULT_LED_PIN,0);
		sleep_us(30);

    		adc_select_input(0);
       		ncent = ((float)adc_read()/3)+40;
		cent=((1000*cent)+ncent)/1001;
    		adc_select_input(1);
       		nfact = (float)adc_read()/2048;
		//if (nfact<0.005){nfact=0;}
		fact=((5*fact)+nfact)/6;

		press=gpio_get(I0); 
		if ( press){ 
			if (release){ release=0; dec=1;}
			if (! attack){
			attack=1; 
			a=0.0001;}
			dec*=0.9995;
			if (dec<0.001){dec=0.001;}
		}else{
			if (attack){ release=1;}
		}

		if (attack==1){ a*=1.005; if(a>=1){a=1;if(!press){attack=0;}}}

		for (x=0;x<16;x+=2)
		{
			float f,df;
			df=(((float)(7-x))*fact/50);
			if (df<0){ 
				f=cent-((df*df*cent));
			}else{
				f=cent+((df*df*cent));
			}
			ticks_on[x]=1000000/(2*f*DELAY);
			ticks_off[x]=ticks_on[x]*2;
			phase[x]=((1-a)*(float)ticks_on[x]);
		}
		if(!attack){if (a>0 ){a-=dec/100;}else{a=0;dec=1;}}
	}
}
