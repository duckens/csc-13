#pragma config(Motor,  motorA,          LEFT_MOTOR,    tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  motorB,          RIGHT_MOTOR,   tmotorNormal, PIDControl, encoder)

#define motorL	motorA
#define motorR motorB
#define GyroSensor S4

#define WAIT_TIME 8.0

const float gOffsetOfMySensor = 602.0;
const int OFFSET_SAMPLES = 100;
const float EMAOFFSET = 0.0005;

const float tInterval = WAIT_TIME * 0.001;

float gOffset;

float propCoeff = 1.15;
float integroCoeff = 7.5;
float motorSpeedCoeff = 0.2;
float motorCoordCoeff = 0.14;

float gAngleSpeed = 0.0;
float gAngleGlobal = 0.0;
float motorSpeed = 0.0;
float motorAver = 0.0;
float motorAverOld = 0.0;

float motorAverExpected = 0.0;
float motorSpeedExpected = 0.0;
float motorSpeedToAverCoeff = 10.0;
float motorsDifferenceCoeff = 1.0;
float motorsDifference = 0.0;
float speedToGyroCoeff = 0.2;

int time = 0;
const int timeMax = 1000000;


const int SCHEDULE_SIZE = 4;
int schedule[SCHEDULE_SIZE] = {300, 2000, 2500, 4000};
float scheduleSpeeds[SCHEDULE_SIZE] = {40.0, 0.0, 40.0, 0.0};


void getGyroData();
void getGyroOffsetHonest();
void getGyroOffsetLazy();

void getMotorSpeed();
float getMotorsAverage();
float getMotorsDifference();
void resetEncoders();
void setOutputOnMotors(int l, int r);
void timeTick();

void ballance();
void scheduler();

float propRegulator(float coeff, float arg);

void debugger();
void recountBindedExpectedThings();


void recountBindedExpectedThings(){
	motorAverExpected += motorSpeed * tInterval * motorSpeedToAverCoeff;
}

void resetEncoders(){
	nMotorEncoder[motorL] = 0;
	nMotorEncoder[motorR] = 0;

	float motorAverNew = motorAver - motorAverExpected;
	float motorAverOldNew = motorAverOld - motorAverExpected;
	motorAverExpected = 0.0;
	motorAverNew = 0.0;
	motorAverOldNew = 0.0;

}

void setOutputOnMotors(int l, int r){
	motor[motorL] = ( (abs(l) < 100) ? l : sgn(l) * 100 ) - motorsDifference;
	motor[motorR] = ( (abs(r) < 100) ? l : sgn(r) * 100 ) + motorsDifference;
}

float getMotorsAverage(){
	return (nMotorEncoder[motorL] + nMotorEncoder[motorR]) / 2.0;
}

float getMotorsDifference(){
	return (nMotorEncoder[motorL] - nMotorEncoder[motorR]) * motorsDifferenceCoeff;
}

void getMotorSpeed(){
	motorAverOld = motorAver;
	motorAver = getMotorsAverage();
	motorsDifference = getMotorsDifference();

	motorSpeed = (motorAver - motorAverOld) / tInterval;
}

void getGyroData(){
	int gRAW = SensorValue(GyroSensor);

	gOffset = EMAOFFSET * gRAW + (1 - EMAOFFSET) * gOffset;
	gAngleSpeed = gRAW - gOffset;
	gAngleGlobal += gAngleSpeed * tInterval;
}

float propRegulator(float coeff, float arg){
	return coeff * arg;
}

void debugger(){
	nxtDisplayTextLine(1, "V ang = %f", gAngleSpeed);
	nxtDisplayTextLine(2, "Angle = %f", gAngleGlobal);
	nxtDisplayTextLine(3, "V = %f", motorSpeed - motorSpeedExpected);
	nxtDisplayTextLine(4, "X = %f", motorAver - motorAverExpected);
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
	nxtDisplayTextLine(2, "gyroData = %d", gAngleSpeed);

	float res = propRegulator(propCoeff, gAngleSpeed)
		+ propRegulator(integroCoeff, gAngleGlobal)
		+ propRegulator(motorSpeedCoeff, motorSpeed)
		+ propRegulator(motorCoordCoeff, motorAver);

	setOutputOnMotors(res, res);

}

void getGyroOffsetHonest(){
	getGyroOffset();

	nxtDisplayTextLine(4,"Balance in 3 second");
	PlayTone(440,100);
  wait1Msec(3000);
}

void timeTick(){
	if(++time > timeMax){
		time = 0;
		resetEncoders();
	}
}

void changeSpeed(float newSpeed){
	//smotorSpeedExpected = newSpeed;
	gOffset = gOffsetOfMySensor - newSpeed * speedToGyroCoeff;
}

void scheduler(){

	for(int i = 0; i < SCHEDULE_SIZE; i++){
		if(time == schedule[i]){
			PlaySound(soundShortBlip);
			changeSpeed(scheduleSpeeds[i]);
			return;
		}
	}



	//changeSpeed(50.0 * sin(time * 2 * PI / 1000.0));

}

task main()
{
	getGyroOffsetLazy();
	//getGyroOffsetHonest();

	resetEncoders();
  PlayTone(440,100);
  eraseDisplay();
	nxtDisplayTextLine(0, "balancing gOffset = %f", gOffset);




	while(1){

		timeTick();
		scheduler();

		ballance();
		debugger();
		wait1Msec(WAIT_TIME);
		recountBindedExpectedThings();

		nxtDisplayTextLine(5, "K = %f", motorSpeed / gAngleSpeed);
	}

}
