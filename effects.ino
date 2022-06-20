#pragma once

void e1() { //not bad
    static uint32_t tmr = 0;
    if (millis() - tmr >= 100) {
        static uint8_t con = 0;
        static uint8_t i = 0;
        tmr = millis();

        buf[i] = con;

        if (++con >= 10)i++;
        if (i >= 4)i = 0;
    }
}

void e2() {
    static uint32_t tmr = 0;
    if (millis() - tmr >= 100) {
        static uint8_t i = 0;
        static uint8_t con = 0;
        tmr = millis();

        buf[i] = con;
        if (++con >= 10) { buf[i] = 10;con = 0;i++; }
        if (i >= 4)i = 0;
    }
}

void e3() { //full identical
    static uint32_t tmr = 0;
    if (millis() - tmr >= 100) {
        static uint8_t con = 0;
        tmr = millis();
        buf[0] = con;
        buf[1] = con;
        buf[2] = con;
        buf[3] = con;
        if (++con >= 10)con = 0;
    }
}

void e4_0() { //rand num
    static uint32_t tmr = 0;
    if (millis() - tmr >= 70) {
        static uint8_t i = 0;
        static uint8_t last;
        tmr = millis();

        last = random(0, 9);
        buf[i] = (last == buf[i] ? random(0, 9) : last);
        if (++i >= 4)i = 0;
    }
}

void e4_1() { //rand num(v2)
    static uint32_t tmr = 0;
    if (millis() - tmr >= 150) {
        static uint8_t arr[4];
        tmr = millis();

        for (uint8_t i = 0;i < 4;i++)arr[i] = random(0, 9);
        for (uint8_t i = 0;i < 4;i++)buf[i] = (arr[i] == buf[i] ? random(0, 9) : arr[i]);
    }
}

bool e5(uint8_t num) {
    static uint32_t tmr = 0;
    static uint8_t i = 0;
    if (millis() - tmr >= 100 && i <= 10) {
        tmr = millis();
        buf[num] = i;
        if (++i >= 10) { i = 0;return true; }
        return false;
    }
}

/*
15:5[5]
0
1
2
3
4
5
6
7
8
9
15:5[6]
0
1
2
3
4
5
6
7
8
9
15:5[7]
*/