#pragma config(Motor,  motorA,          LEFT_MOTOR,    tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorB,          RIGHT_MOTOR,   tmotorNormal, PIDControl, encoder)

#define motorL	motorA
#define motorR motorB
const tSensors GyroSensor = (tSensors) S4;


float gOffset;
#define WAIT_TIME 8.0

const float gOffsetOfMySensor = 607.425;
const int OFFSET_SAMPLES = 100;
const float EMAOFFSET = 0.0005;
//const float EMAANGLE = 0.008;
const float tInterval = WAIT_TIME * 0.001;
float gCurrent;
float gOld;

float propCoeff = 1.15;
//float propCoeff = 0.0;

float cubeCoeff = 0.0;
float diffCoeff = 0.0;


float motorSpeedCoeff = 0.2;
//float motorSpeedCoeff = 0.0;

float motorCoordCoeff = 0.14;
//float motorCoordCoeff = 0.0;


float integroCoeff = 7.5;
float gAngleGlobal = 0;
int motorEncoderOld_L;
int motorEncoderOld_R;
float motorSpeed = 0;
float motorCoord = 0;
float motorAver = 0;
float motorAverOld = 0;

void getGyroData();
void getGyroOffset();
void ballance();
void setOutputOnMotors(int l, int r);
void getGyroOffsetLazy();
float propRegulator(float coeff, float arg);
float cubeRegulator(float arg);
float diffRegulator(float argNew, float argOld);
void getMotorSpeed();
void resetEncoders();


void resetEncoders(){
	nMotorEncoder[motorL] = 0;
	nMotorEncoder[motorR] = 0;
}

void setOutputOnMotors(int l, int r){
	motor[motorL] = (abs(l) < 100) ? l : sgn(l) * 100;
	motor[motorR] = (abs(r) < 100) ? l : sgn(r) * 100;
}

void getMotorSpeed(){
	int motorEncoderCur_L = nMotorEncoder[motorL];
	int motorEncoderCur_R = nMotorEncoder[motorR];
	motorAverOld = motorAver;
	motorAver = (motorEncoderCur_L + motorEncoderCur_R) / 2.0;
	motorSpeed = (motorAver - motorAver) / tInterval;

	//motorCoord += (motorEncoderCur_L + motorEncoderCur_R) / 2.0;
	//motorCoord = (motorEncoderCur_L + motorEncoderCur_R) / 2.0;
}

void getGyroData(){
	//gOld = gCurrent;
	int gRAW = SensorValue(GyroSensor);
	gOffset = EMAOFFSET * gRAW + (1 - EMAOFFSET) * gOffset;
	gCurrent = gRAW - gOffset;
	gAngleGlobal += gCurrent * tInterval;
	//gAngleGlobal -= sgn(gAngleGlobal) * EMAANGLE;
}

float propRegulator(float coeff, float arg){
	return coeff * arg;
}

float cubeRegulator(float arg){
	return cubeCoeff * arg * arg * arg;
}

float diffRegulator(float argNew, float argOld){
	return diffCoeff * (argNew - argOld);
}

void getGyroOffset(){
	wait1Msec(50);

	float gSum;
	int gMin;
	int gMax;

	do{
		gSum = 0.0;
		gMin = 1000;
		gMax = -1000;

		for(int i = 0; i < OFFSET_SAMPLES; i++){
			int g = SensorValue(GyroSensor);
			if(g > gMax) gMax = g;
			if(g < gMin) gMin = g;
			gSum += g;
			wait1Msec(5);
		}
		nxtDisplayTextLine(1, "calibrating: %d", gMax - gMin);
	} while( abs(gMax - gMin) > 1 );
	gOffset = gSum / OFFSET_SAMPLES;
}

void getGyroOffsetLazy(){
	gOffset = gOffsetOfMySensor;
}

void ballance(){

	getGyroData();
	getMotorSpeed();
	nxtDisplayTextLine(0, "gOffset = %f", gOffset);
	nxtDisplayTextLine(2, "gyroData = %d", gCurrent);

	float res = propRegulator(propCoeff, gCurrent)
		+ propRegulator(integroCoeff, gAngleGlobal)
		+ propRegulator(motorSpeedCoeff, motorSpeed)
		+ propRegulator(motorCoordCoeff, motorAver);

	setOutputOnMotors(res, res);

}

task main()
{
	//getGyroOffsetLazy();
	resetEncoders();
	getGyroOffset();

	nxtDisplayTextLine(4,"Balance in 3 second");
	PlayTone(440,100);
  wait1Msec(3000);
  PlayTone(440,100);
  eraseDisplay();
	nxtDisplayTextLine(0, "gOffset = %f", gOffset);
	nxtDisplayTextLine(1, "balancing");

	while(1){
		ballance();
		wait1Msec(WAIT_TIME);
	}

}
