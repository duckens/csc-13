#pragma config(Sensor, S1,     ,               sensorLightActive)
#pragma config(Sensor, S4,     ,               sensorSONAR)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop, encoder)


//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

const int motorLeftIndex = motorB;
const int motorRightIndex = motorA;

const int sensorLightIndex = S1;
const int sensorDistIndex = S4;

bool doRemember = false;
int alphaA=0;
int alphaB=0;
int k=2;
int i = 0;

bool flag = true;
int masA[1000];
int masB[1000];

task remember(){

	while (true){
		if(doRemember){
			masA[i] = nMotorEncoder[motorA];
			masB[i++]= nMotorEncoder[motorB];
			wait1Msec(100);
		}
	}
}

task replay(){

	while(i > 0){
		alphaA = masA[i];
		alphaB = masB[i--];
		wait1Msec(100);
	}
}

task goBack(){
	while (true){
		int eA = alphaA-nMotorEncoder[motorA];
		int eB = alphaB-nMotorEncoder[motorB];
		motor[motorA] = eA*k;
		motor[motorB] = eB*k;
		wait1Msec(1);
	}
}


int speed;
int rudder;

int motorLeft;
int motorRight;

void setOutputOnMotors(int l, int r);
void movenment_init();
void go(int desiredSpeed, int desiredTurn);

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
	motorLeft = 0;
	motorRight = 0;
	speed = 0;
	rudder = 0;
	motor[motorA] = 0;
	motor[motorB] = 0;
}



int rudderOfTurn(int desiredSpeed, int desiredTurn){
	int res = (int)( (float)desiredTurn / 5);
	return res;
}

void go(int desiredSpeed, int desiredTurn){
	rudder = rudderOfTurn(desiredSpeed, desiredTurn);
	speed = desiredSpeed;
	motorLeft = speed + rudder;
	motorRight = speed - rudder;

	setOutputOnMotors(motorLeft, motorRight);
}



float baseCoeff = 40;
float coeff = baseCoeff;

float baseDiffCoeff = 300;
float diffCoeff = baseDiffCoeff;

float baseCubeCoeff = 0.3;
float cubeCoeff = baseCubeCoeff;

int grey = 30;

int baseSpeed = 100;
int maxSpeed = 100;

float curSpeed = baseSpeed;

int curSensor;
int oldSensor;

int leftSide = 1;

const int baseDist = 30;
const int maxDist = 100;




int propRegulator(){
	return (int)( coeff * (curSensor - grey) );
}

int diffRegulator(){
	return diffCoeff * (curSensor - oldSensor);
}

int cubeRegulator(){
	int diff = (curSensor - grey);
	return cubeCoeff * diff * diff * diff;
}


void calibrate(){
	int maxim = SensorValue[S1];
	int minim = SensorValue[S1];
	for(int i = 0; i < 4000; i++){

	}
}





task main(){

	movenment_init();

	StartTask(remember);
	doRemember = true;

	for(int t = 0; t < 10000; t++){

		oldSensor = curSensor;
		curSensor = SensorValue[S1];

		int discrapencyProportional = propRegulator();
		int discrapencyDifferential = diffRegulator();
		int discrapencyCube = cubeRegulator();

		int discrapency = discrapencyProportional + discrapencyDifferential + discrapencyCube;
		discrapency *= leftSide;

		curSpeed = baseSpeed;

		go(curSpeed, discrapency);

		wait1Msec(1);
	}
	doRemember = false;
	motor[motorA] = 0;
	motor[motorB] = 0;
	wait1Msec(100);
	StartTask(goBack);
	wait1Msec(100);
	StartTask(replay);
	wait1Msec(100);
	while(1);

}
