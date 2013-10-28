


const int basePosition = 0;
const int stayPosition = 45;
const float basicFeedbackCoefficient = 1.0;

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

char str[10];

task main(){

	//nxtDisplayCenteredTextLine(0,"Hello");

	nMotorEncoder[mainMotorId] = 0;


	while(1){
		for(float feedbackCoefficient = 0.1; feedbackCoefficient < 200; feedbackCoefficient *= 2){
			sprintf(str, "%f", feedbackCoefficient);
			nxtDisplayCenteredTextLine(0,"goToBasePoint");
			goToBasePoint();
			nxtDisplayCenteredTextLine(0,"done");
			timeOut = 0;
			nxtDisplayCenteredTextLine(0,str);
			StartTask(dalayer);
			while(!timeOut){
				stabilise(stayPosition, feedbackCoefficient);
				wait1Msec(1);
			}

		}
	}
}
