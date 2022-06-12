#pragma once

class Nixie {
private:
    uint8_t* optPin;
    uint8_t* idPin;
    uint8_t buf[4];

    void dec(uint8_t x1, uint8_t x2, uint8_t x3, uint8_t x4, uint8_t* arr = idPin) {
        digitalWrite(arr[0], x4);
        digitalWrite(arr[1], x3);
        digitalWrite(arr[2], x2);
        digitalWrite(arr[3], x1);
    }

    void setDigit(uint8_t num) {
        switch (num) {
        case 0: dec(0, 0, 0, 0); break;
        case 1: dec(0, 0, 0, 1);break;
        case 2: dec(0, 0, 1, 0);break;
        case 3: dec(0, 0, 1, 1);break;
        case 4: dec(0, 1, 0, 0);break;
        case 5: dec(0, 1, 0, 1);break;
        case 6: dec(0, 1, 1, 0);break;
        case 7: dec(0, 1, 1, 1);break;
        case 8: dec(1, 0, 0, 0);break;
        case 9: dec(1, 0, 0, 1);break;
        case 10: dec(1, 1, 1, 1);break;
        }
    }

    void setOpt(uint8_t num) {
        switch (num) {
        case 0:dec(0, 0, 0, 1, optPin); break;
        case 1:dec(0, 0, 1, 0, optPin); break;
        case 2:dec(0, 1, 0, 0, optPin); break;
        case 3:dec(1, 0, 0, 0, optPin); break;
        }
    }
public:
    Nixie() {}
    void begin(uint8_t* idP, uint8_t* optP) {
        idPin = idP;
        optPin = optP;

        for (uint8_t i = 0;i < 4;i++)pinMode(idPin[i], OUTPUT);
        for (uint8_t i = 0;i < 4;i++)pinMode(optPin[i], OUTPUT);
    }

    void setNumber(uint8_t num) {
        if (num < 0 || 9999 < num)return;
        uint8_t arr[4];

        uint8_t con = 3;
        while (con != 0) {
            arr[con--] = num % 10;
            num /= 10;
        }
    }
};
