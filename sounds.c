#include <dos.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#include "sounds.h"


//
// ZOUNDS!!!!!!!!!!!!!
//

void sound_xplo(int d){
	int c;
	
		//d *= d;

	    for(c = 1;c <= 5; c++) {
			sound(c * 290);
			delay(22);
		}
                
		nosound();
        delay(10);

        for(c = 5;c >= 2;c--) {
			sound((sin(c + d * 200) * 200) * d);
			delay(25);
		}
		
	

		
		
		nosound();
}



void sound_nested(int m, int n)
{
	int index, subindex;
	
	for (index = -10; index < 10; index++)
	{
		for (subindex = 20; subindex > -5; subindex--)
		{
			sound((int )(tan(subindex) * index) % n + 100);
			delay(m);
		}
	}

	nosound();

	
	return;
}


void sound_explosion(int modulate){
	register int index;
	register int subIndex;
	
	for (index = 1; index < 30; index++){
		sound((tan(index + modulate) * index) + 100);
		delay(2);
	}
	
	for (index = 1; index < 25; index++){
		sound((tan(index) * index) + 300);
		delay(index / 2);
	}
	
	
	sound(100);
	delay(100);
	
	
	nosound();  
	return;
}


void sound_fart(int modulate){
	register int index;
	
	for (index = 1; index < 50; index++){
		
		sound(pow(index, 2) + 100);
		
		if (index % modulate)
			delay(modulate);
		
	}
	
	nosound();
	
	return;
}


void sound_shortHigh( int modulate )
{
}

void sound_badSound( )
{
	register int c;

		for(c = 5;c >= 0;c--){
       	sound(c * 250);
      	delay(18);
    }
    nosound();
	delay(10);	
	for (c = 0 ; c <= 5 ; c++){
		sound(c * 180);
		delay(18);
	}
                
	nosound();

}

void sound_marioSound( )
{
	int sounder;
	
		for (sounder = 2; sounder < 1500; sounder += sounder)
	{
		sound( (sounder * 3) );
	delay(10);
		nosound();
	}
	
	
}

// void sound_marioSound( )
// {
// 	int sounder;
// 	int randBase;
// 	
// 	randBase = (rand() % 150);
// 	
// 	
// 		for (sounder = 25; sounder < 2200; sounder += sounder)
// 		{
// 		sound(((sounder) + randBase) - 50);
// 		delay(10);
// 		nosound();
// 	}
// 	
// 	
// }