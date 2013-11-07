#pragma config(Sensor, S4,     ,               sensorLightActive)
#pragma config(Sensor, S3,     ,               sensorLightActive)
#pragma config(Sensor, S1,     ,               sensorSONAR)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//



const int motorLeftIndex = motorB;
const int motorRightIndex = motorC;
const int motorManipulatorIndex = motorA;

const int sensorLightIndex_right = S4;
const int sensorLightIndex_left = S3;
const int sensorSonnarIndex = S1;

const int distanceToTakeThing = 20;

const int TASK_M_GRAB = 0;
const int TASK_M_RELEASE = 1;
const int TASK_M_WAIT = 2;

const int TASK_M_STATE_OPEN = 0;
const int TASK_M_STATE_KEEP = 1;
const int TASK_M_STATE_OPPENING= 2;
const int TASK_M_STATE_GRABBING = 3;


bool shouldRotate = false;
bool goingWithObject = false;

int taskMShouldBe = TASK_M_STATE_OPEN;

int taskM = TASK_M_WAIT;
int taskMState = TASK_M_STATE_OPEN;

bool crossDetected = 0;

int motorLeft;
int motorRight;

int grey_left;
int grey_right;

int initialError;

float coeff = 40;
float diffCoeff = 30;
float cubeCoeff = 0.03;

float coeffSpeed = 1.0;

int baseSpeed = 80;
int maxSpeed = 100;

float curSpeed = baseSpeed;

int curSensor;
int oldSensor;

int curDistance;

int leftSide = 1;



void fullRotate();
void killJar();
int recalculateSpeedOfDistance(int dist);
int getSensor();
void makeActionCross();
void setOutputOnMotors(int l, int r);
void movenment_init();
void go(int desiredSpeed, int desiredTurn);
int getDistance();

void grab();
void relese();
task taskGrab();



int max(int x, int y){
	if(x > y)	return x;
	return y;
}


int min(int x, int y){
	if(x < y)	return x;
	return y;
}


void setOutputOnMotors(int l, int r){
	if(abs(l) > 100){
		l = 100 * l / abs(l);
	}
	if(abs(r) > 100){
		r = 100 * r / abs(r);
	}
	motor[motorLeftIndex] = l;
	motor[motorRightIndex] = r;
}


void movenment_init(){
	grey_left = SensorValue[sensorLightIndex_left] - 10;
	grey_right = SensorValue[sensorLightIndex_right] - 10;
	initialError = getSensor();
	setOutputOnMotors(0, 0)	;
}


int rudderOfTurn(int desiredSpeed, int desiredTurn){
	int res = (int)( (float)desiredTurn / 5);
	return res;
}


void go(int desiredSpeed, int desiredTurn){
	int rudder = rudderOfTurn(desiredSpeed, desiredTurn);
	int speed = desiredSpeed;
	motorLeft = speed + rudder;
	motorRight = speed - rudder;

	setOutputOnMotors(motorLeft, motorRight);
}


int getDistance(){
	return SensorValue[sensorSonnarIndex];
}

int propSpeedRegulator(int dist){
	return (int)( coeffSpeed * (dist) );
}

int propRegulator(){
	return (int)( coeff * (curSensor) );
}

int diffRegulator(){
	return diffCoeff * (curSensor - oldSensor);
}

int cubeRegulator(){
	int diff = (curSensor);
	return cubeCoeff * diff * diff * diff;
}

int currentEncoder;
int currentEncoderDelta;
const int currentEncoderDeltaBorder = 100;
const int encoderBorder = 400;
bool goCountingEncoders = 0;
//int summingCorrection;
//const int summingCorrectionBorder = 200;


void recountEncoder(){
	currentEncoder = (nMotorEncoder[motorRightIndex] + nMotorEncoder[motorLeftIndex]);
	currentEncoderDelta = (nMotorEncoder[motorRightIndex] - nMotorEncoder[motorLeftIndex]);
}

void treatCrossDetected(){
	nMotorEncoder[motorRightIndex] = 0;
	nMotorEncoder[motorLeftIndex] = 0;
	goCountingEncoders = true;
	currentEncoderDelta = 0;
	currentEncoder = 0;
}

bool distancePassed(){
	if(currentEncoder > encoderBorder){
		return true;
	}
	return false;
}

int getSensor(){
	int res = SensorValue[sensorLightIndex_left] - SensorValue[sensorLightIndex_right];
	return res;
}

int getSensorCorrectingError(){
	return getSensor() - initialError;
}

void makeActionCorner(){
	PlaySound(soundBeepBeep);
}

void makeAction(){
	PlaySound(soundDownwardTones);
}


task countCrosses(){
	ClearTimer(T1);

	int count = 0;


	while(1){
		if(
			(time1[T1] > 200) &&
			(SensorValue[sensorLightIndex_left] < grey_left) &&
			(SensorValue[sensorLightIndex_right] < grey_right)
		){
			count++;
			ClearTimer(T1);
			crossDetected = true;
			//PlaySound(soundBeepBeep);

		}
		wait1Msec(1);
	}
}

void stabilize(){

		for(int i = 0; i < 100; i++){
			oldSensor = curSensor;
			curSensor = getSensorCorrectingError();

			go(0, leftSide * propRegulator());

			wait1Msec(1);
		}
}

/*
void makeActionCross(){
	stabilize();
	killJar();
}
*/

void makeActionCross(){

	PlaySound(soundBeepBeep);

	if(goingWithObject){
		stabilize();
		taskM = TASK_M_RELEASE;
		taskMShouldBe = TASK_M_STATE_OPEN;
		shouldRotate = true;
		goingWithObject = false;
	}

}

int recalculateSpeedOfDistance(int dist){
	int res = propSpeedRegulator(dist);
	if(res > 100){
		res = 100;
	}
	return res;
}

void fullRotate(){

	int rotateSpeed = 30;

	nMotorEncoder[motorLeftIndex] = 0;
	motor[motorLeftIndex] = -rotateSpeed;
	motor[motorRightIndex] = -rotateSpeed;
  while (nMotorEncoder[motorLeftIndex]>-400){
  		wait1Msec(10);
  }

  nMotorEncoder[motorLeftIndex] = 0;
	motor[motorLeftIndex] = +rotateSpeed;
	motor[motorRightIndex] = -rotateSpeed;
  while (nMotorEncoder[motorLeftIndex]<450){
  	wait1Msec(10);
  }

}

void distanceStuff(){
	if( taskMState != taskMShouldBe ){

		curSpeed = 0;

	} else if(goingWithObject){

		curSpeed = baseSpeed;

	} else {

		curSpeed = recalculateSpeedOfDistance(curDistance);

		if(curDistance < distanceToTakeThing){
			// stop;
			curSpeed = 0;
			stabilize();

			// take it!
			taskM = TASK_M_GRAB;
			taskMShouldBe = TASK_M_STATE_KEEP;
			shouldRotate = true;

			goingWithObject = true;
			PlaySound(soundFastUpwardTones);
		}
	}
}

task main(){

	nMotorEncoder[motorRight] = 0;

	movenment_init();

	StartTask(countCrosses);
	StartTask(taskGrab);

	while(1){

		oldSensor = curSensor;
		curSensor = getSensorCorrectingError();

		curDistance = getDistance();

		int discrapencyProportional = propRegulator();
		int discrapencyDifferential = diffRegulator();
		int discrapencyCube = cubeRegulator();

		int discrapency = discrapencyProportional + discrapencyDifferential + discrapencyCube;
		discrapency *= leftSide;


		distanceStuff();

		//summingCorrection += discrapency;

		go(curSpeed, discrapency);
		nxtDisplayString(0, "go");

		recountEncoder();

		if(crossDetected){
			treatCrossDetected();
			crossDetected = false;
		}

		nxtDisplayString(1, "counting = %d", currentEncoder);
		nxtDisplayString(2, "discrap = %d", currentEncoderDelta);
		if(goCountingEncoders){
			if(distancePassed()){
				if(abs(currentEncoderDelta) > currentEncoderDeltaBorder){
					makeActionCorner();
				} else {
					makeActionCross();
				}
				goCountingEncoders = false;
			}
		}

		if(shouldRotate && (curSpeed != 0) ){
			fullRotate();
			shouldRotate = false;
		}

		wait1Msec(1);

	}
}



void killJar(){
	nMotorEncoder[motorLeftIndex] = 0;
	int killSpead = 60;
	motor[motorLeftIndex] = -killSpead;
	motor[motorRightIndex] = killSpead;

  while (nMotorEncoder[motorLeftIndex]>-260){
  	wait1Msec(10);
  }

  nMotorEncoder[motorLeftIndex] = 0;
  motor[motorLeftIndex] = killSpead;
	motor[motorRightIndex] = killSpead;
  while (nMotorEncoder[motorLeftIndex]<250){
  		wait1Msec(10);
  }
   motor[motorLeftIndex] = -killSpead;
	motor[motorRightIndex] = -killSpead;
	while (nMotorEncoder[motorLeftIndex]>=0){
  		wait1Msec(10);
  }

  nMotorEncoder[motorLeftIndex] = 0;

  motor[motorLeftIndex] = killSpead;
	motor[motorRightIndex] = -killSpead;
  while (nMotorEncoder[motorLeftIndex]<260){
  	wait1Msec(10);
  }
  motor[motorLeftIndex] = 0;
	motor[motorRightIndex] = 0;
}

int grabSpeed = -25;
int keepSpeed = -5;

void relese(){
    	motor[ motorManipulatorIndex ] = -grabSpeed;
			taskMState =TASK_M_STATE_GRABBING;
			int oldEncoderValue =   nMotorEncoder[motorManipulatorIndex];
			wait1Msec(100);
			while( abs( oldEncoderValue -nMotorEncoder[motorManipulatorIndex] )>1){
				oldEncoderValue = nMotorEncoder[motorManipulatorIndex];
				wait1Msec(50);
			}
			motor[ motorManipulatorIndex ] =0;
			taskMState=	TASK_M_STATE_OPEN;
			taskM  =	TASK_M_WAIT;
}

void grab(){
		motor[ motorManipulatorIndex ] = grabSpeed;
			taskMState =TASK_M_STATE_GRABBING;
			int oldEncoderValue =   nMotorEncoder[motorManipulatorIndex];
			wait1Msec(100);
			while( abs( oldEncoderValue -nMotorEncoder[motorManipulatorIndex] )>1){
				oldEncoderValue = nMotorEncoder[motorManipulatorIndex];
				wait1Msec(50);
			}
			motor[ motorManipulatorIndex ] =keepSpeed;
			taskMState=	TASK_M_STATE_KEEP;
			taskM  =	TASK_M_WAIT;
}

task taskGrab(){

  relese();
	nMotorEncoder[motorManipulatorIndex] = 0;

	while (true){
		if (taskM == TASK_M_GRAB){
				grab();
			} if (taskM  == TASK_M_RELEASE){
		   	relese();
   		}
	}


}
