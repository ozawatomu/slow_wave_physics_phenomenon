#include <Arduino.h>
#include <Servo.h>

class BrushlessMotor{
  private:
  Servo motor;
  bool highSet = false;
  double highSetTime;
  bool lowSet = false;
  double lowSetTime;
  bool hasInit = false;

  int mapValues(double value, double fromLow, double fromHigh, double toLow, double toHigh){
    return round((((value - fromLow)/(fromHigh - fromLow))*(toHigh - toLow)) + toLow);
  }

  public:
  BrushlessMotor(short pinNumber){
    motor.attach(pinNumber);
  }

  boolean hasInit(){
    return hasInit;
  }

  void init(){
    if(!hasInit){
      if(highSet == false){
        motor.writeMicroseconds(2000);
        highSet = true;
        highSetTime = millis()/1000.0;
        hasInit = false;
      }else if(lowSet == false){
        double timeSinceHighSet = millis()/1000.0 - highSetTime;
        if(timeSinceHighSet > 11){
          motor.writeMicroseconds(1000);
          lowSet = true;
          lowSetTime = millis()/1000.0;
        }
        hasInit = false;
      }else{
        double timeSinceLowSet = millis()/1000.0 - lowSetTime;
        if(timeSinceLowSet > 8){
          hasInit = true;
        }
      }
    }
  }

  void setThrust(short thrustPercent){
    motor.writeMicroseconds(mapValues(thrustPercent, 0, 100, 1000, 2000));
  }
};

double mapValues(double value, double fromLow, double fromHigh, double toLow, double toHigh){
  return round((((value - fromLow)/(fromHigh - fromLow))*(toHigh - toLow)) + toLow);
}

const short MOTOR_PIN = 3;
const short POT_PIN = A1;
BrushlessMotor motor{MOTOR_PIN};


void setup() {
  while(!motor.hasInit()){
    motor.init();
  }
}

void loop() {
  motor.setThrust(mapValues(analogRead(POT_PIN), 0, 1023, 0, 100));
}