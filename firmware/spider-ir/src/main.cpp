#include <Arduino.h>
#define TURN_SPEED 80
#define DIAG_K 0.8

#include <GyverMotor2.h>
GMotor2<DRIVER2WIRE_PWM_POWER> motorR(3, 5);
GMotor2<DRIVER2WIRE_PWM_POWER> motorL(6, 9);

#include <NecDecoder.h>
NecDecoder ir;

#define IR_1 0xA2
#define IR_2 0x62
#define IR_3 0xE2
#define IR_4 0x22
#define IR_5 0x02
#define IR_6 0xC2
#define IR_7 0xE0
#define IR_8 0xA8
#define IR_9 0x90
#define IR_STAR 0x68
#define IR_0 0x98
#define IR_HASH 0xB0
#define IR_UP 0x18
#define IR_LEFT 0x10
#define IR_OK 0x38
#define IR_RIGHT 0x5A
#define IR_DOWN 0x4A

int speed = 50;

void setup() {
    Serial.begin(115200);
    attachInterrupt(0, []() { ir.tick(); }, FALLING);
    motorR.setAccel(800);
    motorL.setAccel(800);
}

void loop() {
    motorR.tick();
    motorL.tick();

    if (ir.available(true)) {
        switch (ir.readData()) {
            case IR_1:
                speed = 10;
                break;
            case IR_2:
                speed = 20;
                break;
            case IR_3:
                speed = 30;
                break;
            case IR_4:
                speed = 40;
                break;
            case IR_5:
                speed = 50;
                break;
            case IR_6:
                speed = 60;
                break;
            case IR_7:
                speed = 70;
                break;
            case IR_8:
                speed = 80;
                break;
            case IR_9:
                speed = 90;
                break;
            case IR_0:
                speed = 100;
                break;

            case IR_STAR:
                motorR.setSpeedPerc(speed);
                motorL.setSpeedPerc(speed * DIAG_K);
                break;
            case IR_HASH:
                motorR.setSpeedPerc(speed * DIAG_K);
                motorL.setSpeedPerc(speed);
                break;

            case IR_UP:
                motorR.setSpeedPerc(speed);
                motorL.setSpeedPerc(speed);
                break;
            case IR_DOWN:
                motorR.setSpeedPerc(-speed);
                motorL.setSpeedPerc(-speed);
                break;

            case IR_LEFT:
                motorR.setSpeedPerc(TURN_SPEED);
                motorL.setSpeedPerc(-TURN_SPEED);
                break;
            case IR_RIGHT:
                motorR.setSpeedPerc(-TURN_SPEED);
                motorL.setSpeedPerc(TURN_SPEED);
                break;
        }
    }
}
