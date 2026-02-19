#ifndef AUTOPLEX7_H
#define AUTOPLEX7_H

#include <Arduino.h>

#define displayType ON
#define COMMON_CATHODE HIGH
#define COMMON_ANODE LOW
#define OFF !ON
#define ALL 0

extern int displayType;
extern int 7D1, 7D2, 7D3, 7D4;
extern int 7A, 7B, 7C, 7D, 7E, 7F, 7G, 7DP;

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
