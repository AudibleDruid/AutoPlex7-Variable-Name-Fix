#include "AutoPlex7.h"

void Display::blankSegments() {
    digitalWrite(7A, OFF); digitalWrite(7B, OFF); digitalWrite(7C, OFF);
    digitalWrite(7D, OFF); digitalWrite(7E, OFF); digitalWrite(7F, OFF);
    digitalWrite(7G, OFF); digitalWrite(7DP, OFF);
}

void Display::activateDigit(int digitIndex){
    digitalWrite(7D1, digitIndex==0?OFF:ON);
    digitalWrite(7D2, digitIndex==1?OFF:ON);
    digitalWrite(7D3, digitIndex==2?OFF:ON);
    digitalWrite(7D4, digitIndex==3?OFF:ON);
}

void Display::showDigitSegments(int val){
    switch(val){
        case 0: digitalWrite(7A,ON); digitalWrite(7B,ON); digitalWrite(7C,ON); digitalWrite(7D,ON); digitalWrite(7E,ON); digitalWrite(7F,ON); digitalWrite(7G,OFF); break;
        case 1: digitalWrite(7A,OFF); digitalWrite(7B,ON); digitalWrite(7C,ON); digitalWrite(7D,OFF); digitalWrite(7E,OFF); digitalWrite(7F,OFF); digitalWrite(7G,OFF); break;
        case 2: digitalWrite(7A,ON); digitalWrite(7B,ON); digitalWrite(7C,OFF); digitalWrite(7D,ON); digitalWrite(7E,ON); digitalWrite(7F,OFF); digitalWrite(7G,ON); break;
        case 3: digitalWrite(7A,ON); digitalWrite(7B,ON); digitalWrite(7C,ON); digitalWrite(7D,ON); digitalWrite(7E,OFF); digitalWrite(7F,OFF); digitalWrite(7G,ON); break;
        case 4: digitalWrite(7A,OFF); digitalWrite(7B,ON); digitalWrite(7C,ON); digitalWrite(7D,OFF); digitalWrite(7E,OFF); digitalWrite(7F,ON); digitalWrite(7G,ON); break;
        case 5: digitalWrite(7A,ON); digitalWrite(7B,OFF); digitalWrite(7C,ON); digitalWrite(7D,ON); digitalWrite(7E,OFF); digitalWrite(7F,ON); digitalWrite(7G,ON); break;
        case 6: digitalWrite(7A,ON); digitalWrite(7B,OFF); digitalWrite(7C,ON); digitalWrite(7D,ON); digitalWrite(7E,ON); digitalWrite(7F,ON); digitalWrite(7G,ON); break;
        case 7: digitalWrite(7A,ON); digitalWrite(7B,ON); digitalWrite(7C,ON); digitalWrite(7D,OFF); digitalWrite(7E,OFF); digitalWrite(7F,OFF); digitalWrite(7G,OFF); break;
        case 8: digitalWrite(7A,ON); digitalWrite(7B,ON); digitalWrite(7C,ON); digitalWrite(7D,ON); digitalWrite(7E,ON); digitalWrite(7F,ON); digitalWrite(7G,ON); break;
        case 9: digitalWrite(7A,ON); digitalWrite(7B,ON); digitalWrite(7C,ON); digitalWrite(7D,ON); digitalWrite(7E,OFF); digitalWrite(7F,ON); digitalWrite(7G,ON); break;
    }
}

void Display::begin(){
    int pins[] = {7D1,7D2,7D3,7D4,7A,7B,7C,7D,7E,7F,7G,7DP};
    for(int i=0;i<12;i++) pinMode(pins[i], OUTPUT);
    clearDisplay();

    // Set up Timer2 for ~1kHz multiplexing
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2  = 0;
    OCR2A = 249;               // 16MHz/(64*1000Hz)-1
    TCCR2A |= (1 << WGM21);
    TCCR2B |= (1 << CS22);     // prescaler 64
    TIMSK2 |= (1 << OCIE2A);   // enable compare match interrupt
    sei();                      // enable global interrupts
}

void Display::blankDigit(){ blankSegments(); }

void Display::clearDisplay(){
    for(int i=0;i<4;i++){ digitsToDisplay[i]=-1; decimalFlags[i]=false; }
    blankSegments();
    digitalWrite(7D1,OFF); digitalWrite(7D2,OFF); digitalWrite(7D3,OFF); digitalWrite(7D4,OFF);
}

void Display::showDecimal(){ decimalFlags[currentDigit]=true; }
void Display::clearDecimal(){ decimalFlags[currentDigit]=false; }

void Display::testDisplay(){
    for(int i=0;i<4;i++){ digitsToDisplay[i]=8; decimalFlags[i]=true; }
}

void Display::showNumber(int num){
    int tmp[4];
    tmp[0] = (num/1000)%10; tmp[1] = (num/100)%10;
    tmp[2] = (num/10)%10; tmp[3] = num%10;

    bool nonZero=false;
    for(int i=0;i<4;i++){
        if(tmp[i]!=0 || nonZero || i==3){ digitsToDisplay[i]=tmp[i]; nonZero=true; }
        else digitsToDisplay[i]=-1;
    }
}

void Display::setDigit(int number){ currentDigit = number>3?0:number; }

void Display::multiplexStep(){
    blankSegments();
    activateDigit(currentDigit);

    int val = digitsToDisplay[currentDigit];
    if(val != -1) showDigitSegments(val);
    digitalWrite(DP, decimalFlags[currentDigit]?ON:OFF);

    currentDigit++;
    if(currentDigit>3) currentDigit=0;
}

Display display;

ISR(TIMER2_COMPA_vect){
    display.multiplexStep();
}
