


const int basePosition = 0;
const int positionClawRelease = 0;
const int positionClawTake = -160;
const int maxDiscrapency = 10;
const float basicFeedbackCoefficient = 1.0;

int curentPosition = basePosition;
int curentClawPosition = positionClawRelease;
float currentFeedbackCoefficient = basicFeedbackCoefficient;

bool motorClawReady = 0;

const float basicPositionCoefficient = 10.0;
char str[10];

int min(int x, int y){
	if(x > y){
		return y;
	}
	return x;
}


#define mainMotorId motorA
#define clawMotorId motorB

void stabilise(int motorId, int position, float feedbackCoefficient){
	int discrapency = nMotorEncoder[motorId] - position;
	discrapency = discrapency / abs(discrapency) * min(abs(discrapency), maxDiscrapency);
	motor[motorId] = (int)( -feedbackCoefficient * discrapency );
}

void goToBasePoint(){
	for(int i = 0; i < 1000; i++){
		stabilise(mainMotorId, basePosition, basicFeedbackCoefficient);
		stabilise(clawMotorId, positionClawRelease, basicFeedbackCoefficient);
		wait1Msec(1);
	}
}

task stabilisation(){
	while(1){
		stabilise(mainMotorId, curentPosition, currentFeedbackCoefficient);
		stabilise(clawMotorId, curentClawPosition, currentFeedbackCoefficient);

		wait1Msec(1);
	}
}

void clawTake(){
	curentClawPosition = positionClawTake;
}

void clawRelease(){
	curentClawPosition = positionClawRelease;
}

task main(){
	//nxtDisplayCenteredTextLine(0,"Hello");
	nMotorEncoder[mainMotorId] = 0;
	nMotorEncoder[clawMotorId] = 0;

	StartTask(stabilisation);

	ClearTimer(T1);
	while(1){
		nxtDisplayCenteredTextLine(0, "Claw take");
		sprintf(str, "%d", curentClawPosition);
		nxtDisplayCenteredTextLine(1,str);
		clawTake();
		wait1Msec(1000);
		nxtDisplayCenteredTextLine(0, "Claw release");
		sprintf(str, "%d", curentClawPosition);
		nxtDisplayCenteredTextLine(1,str);
		clawRelease();
		wait1Msec(1000);
		//int alpha	= time100[T1] / 10;// * basicPositionCoefficient;

		//curentPosition = alpha * basicPositionCoefficient;
		//wait1Msec(1);
	}
}
