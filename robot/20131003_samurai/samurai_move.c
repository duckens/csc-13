


const int basePosition = 0;
const int stayPosition = 45;
const float basicFeedbackCoefficient = 1.0;
const float basicSpeedCoefficient = 36;
const float basicPositionCoefficient = 10.0;
char str[10];
int curentPosition = stayPosition;
float currentFeedbackCoefficient = basicFeedbackCoefficient;

#define mainMotorId motorA

void stabilise(int position, float feedbackCoefficient){
	int discrapency = nMotorEncoder[motorA] - position;
	motor[mainMotorId] = (int)( -feedbackCoefficient * discrapency );
}

void goToBasePoint(){
	for(int i = 0; i < 1000; i++){
		stabilise(basePosition, basicFeedbackCoefficient);
		wait1Msec(1);
	}
}

int timeOut = 0;

task dalayer(){
	wait1Msec(10000);
	timeOut = 1;
}

task stabilisation(){
	while(1){
		stabilise(curentPosition, currentFeedbackCoefficient);
		wait1Msec(1);
	}
}

task main(){
	//nxtDisplayCenteredTextLine(0,"Hello");
	nMotorEncoder[mainMotorId] = 0;

	StartTask(stabilisation);

	ClearTimer(T1);
	while(1){
		int alpha	= time100[T1] / 10;// * basicPositionCoefficient;
		sprintf(str, "%d", alpha);
		nxtDisplayCenteredTextLine(0,str);
		curentPosition = alpha * basicPositionCoefficient;
		wait1Msec(1);
	}
}
