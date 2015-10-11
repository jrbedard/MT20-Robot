/* ServoRobot.ino
 by J.R. Bedard - @jrbedard - http://jrbedard.com
 Arduino code to control the 5 servos on MT-20 the 3D printed robot.
*/

#include <Servo.h>
#include "ServoEaser.h" // https://github.com/todbot/ServoEaser


// Servo pins on the arduino (you can change them)
enum {
  HEAD_PIN = 1,
  RIGHT_ARM_PIN = 2,
  LEFT_ARM_PIN = 3,
  RIGHT_LEG_PIN = 4,
  LEFT_LEG_PIN = 5,
};


// Robot servos enumeration
enum {
  HEAD = 0,
  RIGHT_ARM = 1,
  LEFT_ARM = 2,
  RIGHT_LEG = 3,
  LEFT_LEG = 4,
};

// Robot animations enumeration
enum {
  RESET,
  IDLES,
  TEST_ALL,
  HEAD_SWEEP,
  RIGHT_ARM_LIFT,
  LEFT_ARM_LIFT,
  BOTH_ARM_LIFT,
  BOTH_ARM_WALK,
  WALK_UPPER_BODY,
  RIGHT_LEG_WALK,
  LEFT_LEG_WALK,
  WALK_LOWER_BODY,
  WALK,
  STRUT,
  RUN,
  WAVE,
  SALUTE,
};

int playAnimation = RESET; // Play this animation!
int playRepeat = 5; // replay x amount of time, -1 for infinite


int SERVO_FRAME = 20; // minimum time between servo updates (milliseconds)


// Robot Servo class
class RobotServo {
  int type; // which servo from enum
  Servo servo; // the Arduino servo
  ServoEaser servoEaser; // servo move easer
  bool started; // play() was called
  
public:
  RobotServo(int _type) {
    type = _type;
    started = false;
  }
  
  void attach(int pin) {
    servo.attach(pin);
  }
  void detach() {
    servo.detach();
  }
  void reset() {
    servoEaser.reset();
  }

  // Play animation on a Servo using ServoEaser
  void play(ServoMove* servoMoves, int servoMoveCount) {
    servoEaser.begin(servo, SERVO_FRAME);
    servoEaser.setMinMaxMicroseconds(1000,2000);
    
    servoEaser.play(servoMoves, servoMoveCount);
    started = true;
  }
  
  void update() {
    if(started) {
      servoEaser.update();
    }
  }
};

// 5 Robot Servos
RobotServo head(HEAD);
RobotServo rightArm(RIGHT_ARM);
RobotServo leftArm(LEFT_ARM);
RobotServo rightLeg(RIGHT_LEG);
RobotServo leftLeg(LEFT_LEG);


// Servo Animation of an array of ServoMoves
class ServoAnimation {
  bool active;
  ServoMove* servoMoves;
  int moveCount;
  
public:
  ServoAnimation() {
    active=false;
  }
  void create(ServoMove* moves, int count) { // Create the list of moves
    servoMoves = moves;
    moveCount = count;
    active=true;
  }
  void play(RobotServo& servo) { // Play this animation
    if(active) {
      servo.play(servoMoves, moveCount);
    }
  }
};


// SERVO MOVES : Arrays of {Angle, Duration} in degrees, milliseconds
ServoMove reset1[] = {
{ 90, 2000},
};//1
ServoMove resetRightLeg1[] = {
{ 110, 2000},
};//1
ServoMove resetLeftLeg1[] = {
{ 70, 2000},
};//1


ServoMove slow180[] = {
{ 90, 3000},
{170, 3000},
{ 90, 3000},
{ 10, 3000},
};//4


// HEAD

ServoMove headSweep4[] = {
{ 90, 2000},
{ 10, 2000}, // look right
{ 90, 2000},
{ 170, 2000}, // look left
};//4

ServoMove headLift4[] = {
{ 140, 2000}, // look left
{ 60, 2000}, // look right
{ 140, 2000},
{ 60, 2000},
};//4

ServoMove headWalk4[] = {
{ 140, 2000}, // look left
{ 60, 2000}, // look right
{ 140, 2000},
{ 60, 2000},
};//4

ServoMove headRun4[] = {
{ 140, 500}, // look left
{ 60, 500}, // look right
{ 140, 500},
{ 60, 500},
};//4


// RIGHT ARM

ServoMove rightArmLift4[] = {
{ 90, 2000}, // right arm down
{ 170, 2000}, // right arm up
{ 90, 2000},
{ 170, 2000},
};//4

ServoMove rightArmWalk4[] = {
{ 90, 2000},
{ 130, 2000},
{ 90, 2000},
{ 130, 2000},
};//4

ServoMove rightArmRun4[] = {
{ 90, 500},
{ 130, 500},
{ 90, 500},
{ 130, 500},
};//4



// LEFT ARM

ServoMove leftArmLift4[] = {
{ 10, 2000}, // left arm up
{ 90, 2000}, // left arm down
{ 10, 2000},
{ 90, 2000},
};//4

ServoMove leftArmWalk4[] = {
{ 50, 2000},
{ 90, 2000},
{ 50, 2000},
{ 90, 2000},
};//4

ServoMove leftArmRun4[] = {
{ 50, 500},
{ 90, 500},
{ 50, 500},
{ 90, 500},
};//4



// RIGHT LEG

ServoMove rightLegWalk4[] = {
{ 70, 2000}, // right leg down
{ 110, 2000}, // right leg up
{ 70, 2000},
{ 110, 2000},
};//4

ServoMove rightLegRun4[] = {
{ 70, 500}, // right leg down
{ 110, 500}, // right leg up
{ 70, 500},
{ 110, 500},
};//4



// LEFT LEG

ServoMove leftLegWalk4[] = {
{ 60, 2000}, // left leg up
{ 110, 2000}, // left leg down
{ 60, 2000},
{ 110, 2000},
};//4

ServoMove leftLegRun4[] = {
{ 60, 500}, // left leg up
{ 110, 500}, // left leg down
{ 60, 500},
{ 110, 500},
};//4




// Robot Animation Class
class RobotAnimation {
  int anim;
  int reps; // repetition amount, -1 for infinite
  int repCount; // counter
  ServoAnimation servoAnims[5]; // 5 servos

public:
  RobotAnimation(int _anim, int _reps) {
    anim = _anim;
    reps = _reps;
  }

  void createAnimations() { // Create animation moves
    switch(anim) {
      case RESET:
        servoAnims[HEAD].create(reset1, 1);
        servoAnims[RIGHT_ARM].create(reset1, 1);
        servoAnims[LEFT_ARM].create(reset1, 1);
        servoAnims[RIGHT_LEG].create(resetRightLeg1, 1);
        servoAnims[LEFT_LEG].create(resetLeftLeg1, 1);
        break;
      case IDLES:
        break;
      case TEST_ALL:
        servoAnims[HEAD].create(slow180, 4);
        servoAnims[RIGHT_ARM].create(slow180, 4);
        servoAnims[LEFT_ARM].create(slow180, 4);
        servoAnims[RIGHT_LEG].create(slow180, 4);
        servoAnims[LEFT_LEG].create(slow180, 4);
        break;
      case HEAD_SWEEP:
        servoAnims[HEAD].create(headSweep4, 4);
        break;
      case RIGHT_ARM_LIFT:
        servoAnims[RIGHT_ARM].create(rightArmLift4, 4);
        break;
      case LEFT_ARM_LIFT:
        servoAnims[LEFT_ARM].create(leftArmLift4, 4);
        break;
      case BOTH_ARM_LIFT:
        servoAnims[HEAD].create(headLift4, 4);
        servoAnims[RIGHT_ARM].create(rightArmLift4, 4);
        servoAnims[LEFT_ARM].create(leftArmLift4, 4);
        break;
      case BOTH_ARM_WALK:
        servoAnims[RIGHT_ARM].create(rightArmWalk4, 4);
        servoAnims[LEFT_ARM].create(leftArmWalk4, 4);
        break;
      case WALK_UPPER_BODY:
        servoAnims[HEAD].create(headWalk4, 4);
        servoAnims[RIGHT_ARM].create(rightArmWalk4, 4);
        servoAnims[LEFT_ARM].create(leftArmWalk4, 4);
        break;
      case RIGHT_LEG_WALK:
        servoAnims[RIGHT_LEG].create(rightLegWalk4, 4);
        break;
      case LEFT_LEG_WALK:
        servoAnims[LEFT_LEG].create(leftLegWalk4, 4);
        break;
      case WALK_LOWER_BODY:
        servoAnims[RIGHT_LEG].create(rightLegWalk4, 4);
        servoAnims[LEFT_LEG].create(leftLegWalk4, 4);
        break;
      case WALK:
        servoAnims[HEAD].create(headWalk4, 4);
        servoAnims[RIGHT_ARM].create(rightArmWalk4, 4);
        servoAnims[LEFT_ARM].create(leftArmWalk4, 4);
        servoAnims[RIGHT_LEG].create(rightLegWalk4, 4);
        servoAnims[LEFT_LEG].create(leftLegWalk4, 4);
        break;
      case RUN:
        servoAnims[HEAD].create(headRun4, 4);
        servoAnims[RIGHT_ARM].create(rightArmRun4, 4);
        servoAnims[LEFT_ARM].create(leftArmRun4, 4);
        servoAnims[RIGHT_LEG].create(rightLegRun4, 4);
        servoAnims[LEFT_LEG].create(leftLegRun4, 4);
        break;
    }
  }

  void playAnimations() {
    servoAnims[HEAD].play(head);
    servoAnims[RIGHT_ARM].play(rightArm);
    servoAnims[LEFT_ARM].play(leftArm);
    servoAnims[RIGHT_LEG].play(rightLeg);
    servoAnims[LEFT_LEG].play(leftLeg);
  }
};


// Instanciate current robot animation
RobotAnimation robotAnim(playAnimation, playRepeat);


// Setup
void setup() {
  head.attach(HEAD_PIN); // attaches the head servo to pin
  rightArm.attach(RIGHT_ARM_PIN); // right arm pin
  leftArm.attach(LEFT_ARM_PIN); // left arm pin
  rightLeg.attach(RIGHT_LEG_PIN); // right leg pin
  leftLeg.attach(LEFT_LEG_PIN); // left leg pin
  
  robotAnim.createAnimations(); // generate animations

  robotAnim.playAnimations(); // start playing animations
}


// Arduino Loop
void loop() {
  head.update();
  rightArm.update();
  leftArm.update();
  rightLeg.update();
  leftLeg.update();
}

