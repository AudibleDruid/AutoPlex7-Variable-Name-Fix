#include "AutoPlex7.h"

void Display::blankSegments() {
    digitalWrite(A7, OFF); digitalWrite(B7, OFF); digitalWrite(C7, OFF);
    digitalWrite(D7, OFF); digitalWrite(E7, OFF); digitalWrite(F7, OFF);
    digitalWrite(G7, OFF); digitalWrite(DP, OFF);
}

void Display::activateDigit(int digitIndex){
    digitalWrite(D17, digitIndex==0?OFF:ON);
    digitalWrite(D27, digitIndex==1?OFF:ON);
    digitalWrite(D37, digitIndex==2?OFF:ON);
    digitalWrite(D47, digitIndex==3?OFF:ON);
}

void Display::showDigitSegments(int val){
    switch(val){
        case 0: digitalWrite(A7,ON); digitalWrite(B7,ON); digitalWrite(C7,ON); digitalWrite(D7,ON); digitalWrite(E7,ON); digitalWrite(F7,ON); digitalWrite(G7,OFF); break;
        case 1: digitalWrite(A7,OFF); digitalWrite(B7,ON); digitalWrite(C7,ON); digitalWrite(D7,OFF); digitalWrite(E7,OFF); digitalWrite(F7,OFF); digitalWrite(G7,OFF); break;
        case 2: digitalWrite(A7,ON); digitalWrite(B7,ON); digitalWrite(C7,OFF); digitalWrite(D7,ON); digitalWrite(E7,ON); digitalWrite(F7,OFF); digitalWrite(G7,ON); break;
        case 3: digitalWrite(A7,ON); digitalWrite(B7,ON); digitalWrite(C7,ON); digitalWrite(D7,ON); digitalWrite(E7,OFF); digitalWrite(F7,OFF); digitalWrite(G7,ON); break;
        case 4: digitalWrite(A7,OFF); digitalWrite(B7,ON); digitalWrite(C7,ON); digitalWrite(D7,OFF); digitalWrite(E7,OFF); digitalWrite(F7,ON); digitalWrite(G7,ON); break;
        case 5: digitalWrite(A7,ON); digitalWrite(B7,OFF); digitalWrite(C7,ON); digitalWrite(D7,ON); digitalWrite(E7,OFF); digitalWrite(F7,ON); digitalWrite(G7,ON); break;
        case 6: digitalWrite(A7,ON); digitalWrite(B7,OFF); digitalWrite(C7,ON); digitalWrite(D7,ON); digitalWrite(E7,ON); digitalWrite(F7,ON); digitalWrite(G7,ON); break;
        case 7: digitalWrite(A7,ON); digitalWrite(B7,ON); digitalWrite(C7,ON); digitalWrite(D7,OFF); digitalWrite(E7,OFF); digitalWrite(F7,OFF); digitalWrite(G7,OFF); break;
        case 8: digitalWrite(A7,ON); digitalWrite(B7,ON); digitalWrite(C7,ON); digitalWrite(D7,ON); digitalWrite(E7,ON); digitalWrite(F7,ON); digitalWrite(G7,ON); break;
        case 9: digitalWrite(A7,ON); digitalWrite(B7,ON); digitalWrite(C7,ON); digitalWrite(D7,ON); digitalWrite(E7,OFF); digitalWrite(F7,ON); digitalWrite(G7,ON); break;
    }
}

void Display::begin(){
    int pins[] = {D17,D27,D37,D47,A7,B7,C7,D7,E7,F7,G7,DP7};
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
    digitalWrite(D17,OFF); digitalWrite(D27,OFF); digitalWrite(D37,OFF); digitalWrite(D47,OFF);
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
    digitalWrite(DP7, decimalFlags[currentDigit]?ON:OFF);

    currentDigit++;
    if(currentDigit>3) currentDigit=0;
}

Display display;

ISR(TIMER2_COMPA_vect){
    display.multiplexStep();
}
