#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int alphaA=0;
int alphaB=0;
int alphaC=0;
int k=2;

bool flag = true;
int masA[1000];
int masB[1000];
int masC[1000];

task pregA(){
	while (true){
		int e = alphaA-nMotorEncoder[motorA];
		motor[motorA] = e*k;
		wait1Msec(1);
	}
}


task pregB(){
	while (true){
		int e = alphaB-nMotorEncoder[motorB];
		motor[motorB] = e*k;
		wait1Msec(1);
	}
}

task pregC(){
	while (true){
		int e = alphaC-nMotorEncoder[motorC];
		motor[motorC] = e*k;
		wait1Msec(1);
	}
}

task main(){
	int i = 0;
	while (nNxtButtonPressed !=-1);

	motor[motorA] = 0;
	motor[motorB] = 0;
	motor[motorC] = 0;

	nMotorEncoder[motorA]=0;
	nMotorEncoder[motorB]=0;
	nMotorEncoder[motorC]=0;

	bFloatDuringInactiveMotorPWM = true;


	while(nNxtButtonPressed!=3){
		masA[i] = nMotorEncoder[motorA];
		masB[i] = nMotorEncoder[motorB];
	//	masC[i] = nMotorEncoder[motorC];
		i++;
		wait1Msec(100);
	}


	PlaySound(soundBeepBeep);
	wait1Msec(1000);
	bFloatDuringInactiveMotorPWM = false;

	StartTask(pregA);
	StartTask(pregB);
	//StartTask(pregC);

	for(int j = i-1; j>=0;j--){
		alphaA = masA[j];
		alphaB = masB[j];
		wait1Msec(100);

	}

	flag=false;
	wait10Msec(100);
}