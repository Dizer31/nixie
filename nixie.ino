//-----setting-----//
#define mosfet 3
#define period 3    //не изменять

#define debugMode 0
//-----setting-----//

//-----lib & define & init-----//
#if debugMode == 1
#define debug(x) Serial.println(x)
#else
#define debug(x)
#endif

#define pwm(x) analogWrite(mosfet, x)
#define digitalWrite(pin, x) bitWrite((pin < 8 ? PORTD : PORTB), (pin < 8 ? pin : (pin - 8)), x)

#include <DS3231.h>
DS3231 rtc;
Time t;
//-----lib & define & init-----//

//-----special variables-----//
uint8_t optPin[] = { 4,5,6,7 };
uint8_t idPin[] = { 9,10,11,12 };
uint8_t buf[4];

uint16_t x = 50; //or 185
//-----special variables-----//

//-----func-----//
void dec(bool x1, bool x2, bool x3, bool x4, uint8_t* arr) {
    digitalWrite(arr[0], x4);
    digitalWrite(arr[1], x3);
    digitalWrite(arr[2], x2);
    digitalWrite(arr[3], x1);
}

void setDigit(uint8_t num) {
    /*
    switch (num) {
    case 0:  dec(0, 0, 0, 0); break;
    case 1:  dec(0, 0, 0, 1); break;
    case 2:  dec(0, 0, 1, 0); break;
    case 3:  dec(0, 0, 1, 1); break;
    case 4:  dec(0, 1, 0, 0); break;
    case 5:  dec(0, 1, 0, 1); break;
    case 6:  dec(0, 1, 1, 0); break;
    case 7:  dec(0, 1, 1, 1); break;
    case 8:  dec(1, 0, 0, 0); break;
    case 9:  dec(1, 0, 0, 1); break;
    case 10: dec(1, 1, 1, 1); break;
    }
    */
    if (num == 10) { dec(1, 1, 1, 1, idPin);return; }
    dec(bitRead(num, 3), bitRead(num, 2), bitRead(num, 1), bitRead(num, 0), idPin);
}

void setOpt(uint8_t num) {
    /*
    switch (num) {
    case 0: dec(0, 0, 0, 1, optPin); break;
    case 1: dec(0, 0, 1, 0, optPin); break;
    case 2: dec(0, 1, 0, 0, optPin); break;
    case 3: dec(1, 0, 0, 0, optPin); break;
    }
    */
    num = 0x01 << num;
    dec(bitRead(num, 3), bitRead(num, 2), bitRead(num, 1), bitRead(num, 0), optPin);
}

void updData(uint16_t num) {
    if (num < 0 || 9999 < num)return;

    for (int8_t i = 3;i >= 0;i--) {
        if (num > 0) {
            buf[i] = num % 10;
            num /= 10;
        } else buf[i] = 0;
    }
}

void display() {
    static uint8_t con = 0;
    static uint32_t tmr = 0;

    if (millis() - tmr >= period) {
        tmr = millis();
        setOpt(con);
        delayMicroseconds(x);
        setDigit(buf[con]);

        if (++con >= 4)con = 0;
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

    TCCR2B = 0b00000001;
    TCCR2A = 0b00000001;

    pwm(190);
}

void loop() {
#if debugMode == 1
    if (Serial.available()) { x = Serial.parseInt();debug(x); }
#endif

    static bool flag = false;
    if (millis() % 100 == 0) {
        if (!flag) {
            flag = true;
            t = rtc.getTime();
            updData(t.hour * 100 + t.min);
        }
    } else flag = false;

    display();
}
