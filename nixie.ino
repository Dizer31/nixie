//-----setting-----//
#define mosfet 9    //generator
#define debugMode 1
//-----setting-----//

//-----lib & define & init-----//
#if debugMode == 1
#define debug(x) Serial.println(x)
#else
#define debug(x)
#endif

#define pwm(x) analogWrite(mosfet, x)
//#define digitalWrite(pin, x) bitWrite((pin < 8 ? PORTD : PORTB), (pin < 8 ? pin : (pin - 8)), x)
//#define digitalWrite(pin, x) bitWrite((pin < 8 ? PORTD : (pin < 14 ? PORTB : (pin < 20 ? PORTC))), (pin < 8 ? pin : (pin < 14 ? pin - 8 : (pin < 20 ? pin - 14 : 0))), x)

#include <DS3231.h>
DS3231 rtc;
Time t;
//-----lib & define & init-----//

//-----special variables-----//
uint8_t optPin[] = { 4,5,6,7 };
uint8_t idPin[] = { A0,A1,A2,A3 };
uint8_t buf[4] = { 10,10,10,10 };
//-----special variables-----//

//-----func-----//
void setPin(uint8_t pin, uint8_t x) {
    if (pin < 8) bitWrite(PORTD, pin, x);
    else if (pin < 14) bitWrite(PORTB, (pin - 8), x);
    else if (pin < 20) bitWrite(PORTC, (pin - 14), x);
    else return;
}

void dec(bool x1, bool x2, bool x3, bool x4, uint8_t* arr) {
    setPin(arr[0], x4);
    setPin(arr[1], x3);
    setPin(arr[2], x2);
    setPin(arr[3], x1);
}

void setDigit(uint8_t num) {
    if (num == 10) { dec(true, true, true, true, idPin);return; }
    dec(bitRead(num, 3), bitRead(num, 2), bitRead(num, 1), bitRead(num, 0), idPin);
}

void setOpt(uint8_t num) {
    num = 0x01 << num;
    dec(bitRead(num, 3), bitRead(num, 2), bitRead(num, 1), bitRead(num, 0), optPin);
}

void split(uint16_t num) {
    if (num < 0 || 9999 < num)return;

    for (int8_t i = 3;i >= 0;i--) {
        if (num > 0) {
            buf[i] = num % 10;
            num /= 10;
        } else buf[i] = 0;
    }
}

ISR(TIMER2_COMPA_vect) {
    static uint8_t counter = 0;
    static uint8_t con = 0;
    if (counter == 19)setPin(optPin[con], 0);

    if (++counter > 22) {
        counter = 0;
        if (++con >= 4)con = 0;
        setPin(optPin[con], 1);

        setPin(idPin[0], bitRead(buf[con], 0));
        setPin(idPin[1], bitRead(buf[con], 1));
        setPin(idPin[2], bitRead(buf[con], 2));
        setPin(idPin[3], bitRead(buf[con], 3));
    }
}
//-----func-----//

void setup() {
#if debugMode == 1
    Serial.begin(9600);
#endif
    rtc.begin();

    pinMode(mosfet, OUTPUT);
    for (uint8_t i = 0;i < 4;i++) {
        pinMode(idPin[i], OUTPUT);
        pinMode(optPin[i], OUTPUT);
    }
    TCCR1B = TCCR1B & 0b11111000 | 1;   //pwm

    pwm(180);

    TCCR2B = (TCCR2B & B11111000) | 2;  //timer2
    TCCR2A |= (1 << WGM21);
    TIMSK2 |= (1 << OCIE2A);
}
/*
void loop() {
#if debugMode == 1
    if (Serial.available()) { x = Serial.parseInt();debug(x); }
#endif


    static bool flag = false;
    if (millis() % 100 != 0) flag = false; else {
        if (!flag) {
            flag = true;
            t = rtc.getTime();
            //updData(t.hour * 100 + t.min);
            buf[0] = t.hour / 10;
            buf[1] = t.hour % 10;
            buf[2] = t.min / 10;
            buf[3] = t.min % 10;
        }
    }

    static uint32_t tmr = 0;
    if (millis() - tmr >= period) {
        tmr = millis();
        t = rtc.getTime();
        //updData(t.hour * 100 + t.min);
        buf[0] = (t.hour / 10 == 0 ? 10 : t.hour / 10);
        buf[1] = t.hour % 10;
        buf[2] = t.min / 10;
        buf[3] = t.min % 10;
    }

    display();
}
*/

void loop() {
    /*
    static uint32_t tmr = 0;
    if (millis() - tmr >= 1000) {
        static uint8_t i = 0;
        tmr = millis();
        buf[0] = i;
        buf[1] = i;
        buf[2] = i;
        buf[3] = i;
        if (++i >= 10)i = 0;
    }
    static uint32_t tmr = 0;
    if (millis() - tmr >= 100) {
        static uint8_t i = 0;
        static uint8_t con = 0;
        tmr = millis();

        buf[i] = con;
        if (++con >= 10) { buf[i] = 10;con = 0;i++; }
        if (i >= 4)i = 0;
    }
    */
    static uint32_t tmr = 0;
    if (millis() - tmr >= 100) {
        tmr = millis();
        t = rtc.getTime();
        buf[0] = (t.hour / 10 == 0 ? 10 : t.hour / 10);
        buf[1] = t.hour % 10;
        buf[2] = t.min / 10;
        buf[3] = t.min % 10;
    }
}
/*
    static uint8_t con = 0;
    static uint32_t tmr = 0;
    if (millis() - tmr >= 100) {
        tmr = millis();
        static uint8_t i = 0;

        buf[i] = con;

        if (++con >= 10)i++;
        if (i >= 4)i = 0;
    }
*/