
/* idea:  
 * use Timer2, because timer0 is used for delay/millis, leaving Timer1 for other stuff (because it has 16 bits)
 * this means we can use pins D3(PB3) and D12(PD3) from the arduino(avr).
 * we use comparator OCR2A to make the counter overflow at 210 resulting at a frequency of 38kHz 
 * we use comparator OCR2B to toggle pin3(OC2B) at 105 (50% dutycycle) (it automatically flips back at 0)
 * the prescaler can be set to 1 for maximum accuracy, but then we do require phase-correct pwm, not for phase correctness but because it halves the frequency.
 
useful link:
https://et06.dunweber.com/atmega_timers/
*/



#define TIMER_ENABLE_PWM    (TCCR2A |= _BV(COM2B1))
#define TIMER_DISABLE_PWM   (TCCR2A &= ~(_BV(COM2B1)))



// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);

  pinMode(3,OUTPUT); // otherwise no pwm output
  pinMode(13,OUTPUT); // for led

  // set the overflow threshold
  // experimentally determined
  OCR2A = 210; // reset counter  
  OCR2B = 105; // flip output halfway

  // set output compare mode: output OC2 high when above OCR2B
  bitSet(TCCR2A , COM2B1);
  bitClear(TCCR2A,COM2B0);
  
  // WGM (configured for phase-correct pwm because then the frequency is halved such that we can use prescaler 1 and we get better frequency accuracy)
  bitSet(TCCR2A,WGM20);
  bitClear(TCCR2A,WGM21);
  bitSet(TCCR2B,WGM22);

  // prescaler 1x
  bitSet(TCCR2B,CS20);
  bitClear(TCCR2B,CS21);
  bitClear(TCCR2B,CS22);

}

#define TOP 5000
#define DECR(x,dx) if(x<(dx)){x=x+TOP-(dx);}{x=x-(dx);}

uint16_t counter = TOP;
bool status = false;
void loop() {
	
	uint16_t oldcounter = counter;
	DECR(counter,3); // normal time progression

	// get in sync:
	if(!digitalRead(8)) { // if IR receiver is getting something
		Serial.println("+");
		if(counter < TOP/2) {
			DECR(counter,1); // catch up
		}else{
			counter++; // slow down
		}
	}else{
		Serial.println("-");
	}
			
	// check if we should turn off
	if(status && counter < TOP/4*3){
		// all off
		// digitalWrite(13,LOW);
		TIMER_DISABLE_PWM;
		status = false;
	}
	if(counter > oldcounter){ // wrapped
		// all on
		// digitalWrite(13,HIGH);
		status = true;
		TIMER_ENABLE_PWM;
	}
	digitalWrite(13,counter < 500 || (counter > 1000 && counter < 1500));
	delay(1);


}
