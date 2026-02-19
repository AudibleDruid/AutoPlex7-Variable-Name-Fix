#ifndef AUTOPLEX7_H
#define AUTOPLEX7_H

#include <Arduino.h>

#define displayType ON
#define COMMON_CATHODE HIGH
#define COMMON_ANODE LOW
#define OFF !ON
#define ALL 0

extern int displayType;
extern int D17, D27, D37, D47;
extern int A7, B7, C7, D7, E7, F7, G7, DP7;

class Display {
private:
    int digitsToDisplay[4] = {-1,-1,-1,-1};
    bool decimalFlags[4] = {false,false,false,false};
    int currentDigit = 0;

    void blankSegments();
    void activateDigit(int digitIndex);
    void showDigitSegments(int val);

public:
    void begin();
    void blankDigit();
    void clearDisplay();
    void showDecimal();
    void clearDecimal();
    void testDisplay();
    void showNumber(int num);
    void setDigit(int number);
    void multiplexStep();
};

extern Display display;

#endif
