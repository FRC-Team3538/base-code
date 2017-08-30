//Default Code

#include <iostream>
#include <memory>
#include <string>
#include "WPILib.h"

#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>

class Robot: public frc::IterativeRobot {
private:
	frc::LiveWindow* lw = LiveWindow::GetInstance();

	RobotDrive Adrive;
	Joystick DriveStick;
	Joystick OperatorStick;

	//Drive
	VictorSP DriveLeft0;
	VictorSP DriveLeft1;
	VictorSP DriveLeft2;
	VictorSP DriveRight0;
	VictorSP DriveRight1;
	VictorSP DriveRight2;
	Solenoid *DriveSolenoid = new Solenoid(0);
	float OutputX;
	float OutputY;
	float X1;

	//Operator
	VictorSP PWM6;
	VictorSP PWM7;
	VictorSP PWM8;
	VictorSP PWM9;
	Solenoid *PCM1 = new Solenoid(1);
	Solenoid *PCM2 = new Solenoid(2);
	Solenoid *PCM3 = new Solenoid(3);
	Solenoid *PCM4 = new Solenoid(4);
	DigitalInput LimSwUp_PWM7, LimSwDwn_PWM7, LimSwUp_PWM8, LimSwDwn_PWM8;

	PowerDistributionPanel *pdp = new PowerDistributionPanel();  // create pdp variable



	//Added By Ricky
	//VictorSP PWM0;
	//VictorSP PWM1;
	//VictorSP PWM2;
	//VictorSP PWM3;
	//VictorSP PWM4;
	//VictorSP PWM5;

	bool PWM8Enabled;

public:
	Robot() :
		Adrive(DriveLeft0, DriveRight0), DriveStick(0), OperatorStick(1), DriveLeft0(
				0), DriveLeft1(1), DriveLeft2(2), DriveRight0(3), DriveRight1(
				4), DriveRight2(5), PWM6(6), PWM7(7), PWM8(8), PWM9(9), LimSwUp_PWM7(
				0), LimSwDwn_PWM7(1), LimSwUp_PWM8(2), LimSwDwn_PWM8(3) {
		}

	//Ricky's code
	/*Robot() :
						DriveStick(0), OperatorStick(1), PWM0(
						0), PWM1(1), PWM2(2), PWM3(3), PWM4(
						4), PWM5(5), PWM6(6), PWM7(7), PWM8(8), PWM9(9), LimSwUp_PWM7(
						0), LimSwDwn_PWM7(1), LimSwUp_PWM8(2), LimSwDwn_PWM8(3) {
*/

	void RobotInit() {
		//turn off shifter solenoids
		DriveSolenoid->Set(false);

		//disable drive watchdogs
		Adrive.SetSafetyEnabled(false);

		//drive command averaging filter
		OutputX = 0, OutputY = 0;

		//automatically disabling pwm8
		PWM8Enabled = false;
	}

	void RobotPeriodic() {
		//links multiple motors together
		DriveLeft1.Set(DriveLeft0.Get());
		DriveLeft2.Set(DriveLeft0.Get());
		DriveRight1.Set(DriveRight0.Get());
		DriveRight2.Set(DriveRight0.Get());

		//displays sensor and motor info to smartDashboard
		try {
			SmartDashboardUpdate();
		} catch (std::exception ex) {
			std::string err_string = "Smart Dash Errorz! :  ";
			err_string += ex.what();
			DriverStation::ReportError(err_string.c_str());
		}

	}

	void AutonomousInit() override {
	}

	void AutonomousPeriodic() {
	}

	void TeleopInit() {
		OutputX = 0, OutputY = 0;
	}

	void TeleopPeriodic() {

		double Deadband = 0.11;

		//**** DRIVER CONTROLS****

		//high gear & low gear controls
		if (DriveStick.GetRawButton(6))
			DriveSolenoid->Set(true);			// High gear press LH bumper
		if (DriveStick.GetRawButton(5))
			DriveSolenoid->Set(false);			// Low gear press RH bumper

		//drive controls
		double SpeedLinear = DriveStick.GetRawAxis(1) * 1; // get Yaxis value (forward)
		double SpeedRotate = DriveStick.GetRawAxis(4) * -1; // get Xaxis value (turn)

		// Set dead band for X and Y axis
		if (fabs(SpeedLinear) < Deadband)
			SpeedLinear = 0.0;
		if (fabs(SpeedRotate) < Deadband)
			SpeedRotate = 0.0;

		//slow down direction changes from 1 cycle to 5
		OutputY = (0.8 * OutputY) + (0.2 * SpeedLinear);
		OutputX = (0.8 * OutputX) + (0.2 * SpeedRotate);

		//OutputX = SpeedRotate;
		//OutputY = SpeedLinear;

		//drive
		Adrive.ArcadeDrive(OutputY, OutputX, true);
/*
		//********START RICKY'S MECANUM********

		float ANOTHERFREAKINGAXIS = DriveStick.GetRawAxis(0);

		if (fabs(ANOTHERFREAKINGAXIS) < Deadband)
					ANOTHERFREAKINGAXIS = 0.0;

		//X1 = (0.8 * X1) + (0.2 * ANOTHERFREAKINGAXIS);
		X1 = -ANOTHERFREAKINGAXIS;

		if(OutputX < 0.1 && OutputX > -0.1){
			OutputX = 0;
		}

		if(OutputY < 0.1 && OutputY > -0.1){
			OutputY = 0;
		}

		float X2 = OutputX;
		float Y1 = OutputY;

		if(X1 < 0.1 && X1 > -0.1){
			X1 = 0;
		}

		//front-right mecanum
		DriveLeft1.Set(-(Y1 - X2 + X1));

		//back-right mecanum
		DriveLeft0.Set(-(Y1 - X2 - X1));

		//back-left mecanum
		DriveRight1.Set(Y1 + X2 - X1);

		//front-left mecanum
		DriveRight0.Set(Y1 + X2 + X1);

		DriverStation::ReportWarning("Zaeem likes long bananas");
*/
		//********END RICKY'S MECANUM********

		//**** OPERATOR CONTROLS****

		//PWM6 motor control
		if ((OperatorStick.GetRawAxis(2) > Deadband)
				&& (OperatorStick.GetRawAxis(3) < Deadband))
			PWM6.Set(OperatorStick.GetRawAxis(2));
		else if ((OperatorStick.GetRawAxis(3) > Deadband)
				&& (OperatorStick.GetRawAxis(2) < Deadband))
			PWM6.Set(-OperatorStick.GetRawAxis(3));
		else
			PWM6.Set(0.0);

		//PWM7 motor control
		if (OperatorStick.GetRawAxis(1) < -Deadband) {
			PWM7.Set(-1 * OperatorStick.GetRawAxis(1));
			if (!(LimSwUp_PWM7.Get()))
				PWM7.Set(0.0);
		} else if (OperatorStick.GetRawAxis(1) > Deadband) {
			PWM7.Set(-1 * OperatorStick.GetRawAxis(1));
			if (!(LimSwDwn_PWM7.Get()))
				PWM7.Set(0.0);
		} else
			PWM7.Set(0.0);

		//PWM8 motor control
		//top POV button
		//Default PWM value: 0.8
		if ((OperatorStick.GetPOV(0) == 0)) {
			PWM8.Set(0.5);
			if (!(LimSwUp_PWM8.Get()))
				PWM8.Set(0.0);
		}
		//bottom POV button
		//Default PWM value: 0.8
		else if (OperatorStick.GetPOV(0) == 180) {
			PWM8.Set(-0.5);
			if (!(LimSwDwn_PWM8.Get()))
				PWM8.Set(0.0);
		} else
			PWM8.Set(0.0);

		if(OperatorStick.GetPOV(0) == 270){
			PWM8Enabled = true;
		}

		if(OperatorStick.GetPOV(0) == 90){
			PWM8Enabled = false;
		}

		if(PWM8Enabled == true){
			PWM8.Set(0.5);
		} else {
			PWM8.Set(0.0);
		}

		//PWM9 motor control
		if (OperatorStick.GetRawAxis(5) > Deadband
				|| OperatorStick.GetRawAxis(5) < -Deadband)
			PWM9.Set(-1 * OperatorStick.GetRawAxis(5));
		else
			PWM9.Set(0.0);

		//PCM1 solenoid control
		if (OperatorStick.GetRawButton(5))
			PCM1->Set(true);				//press LH bumper
		if (OperatorStick.GetRawButton(6))
			PCM1->Set(false);				//press RH bumper

		//PCM2 solenoid control - B button
		if (OperatorStick.GetRawButton(2))
			PCM2->Set(true);
		else
			PCM2->Set(false);

		//PCM3 solenoid control - A button
		if (OperatorStick.GetRawButton(1))
			PCM3->Set(true);
		else
			PCM3->Set(false);

		//PCM4 solenoid control
		if (OperatorStick.GetRawButton(4))
			PCM4->Set(true);				//press Y button
		if (OperatorStick.GetRawButton(3))
			PCM4->Set(false);				//press X button
	}

	void TestPeriodic() {
		lw->Run();
	}
	void SmartDashboardUpdate() {
		// PWM displays
		SmartDashboard::PutNumber("Drive L0 Output", DriveLeft0.Get());
		SmartDashboard::PutNumber("Drive L1 Output", DriveLeft1.Get());
		SmartDashboard::PutNumber("Drive L2 Output", DriveLeft2.Get());
		SmartDashboard::PutNumber("Drive R0 Output", DriveRight0.Get());
		SmartDashboard::PutNumber("Drive R1 Output", DriveRight1.Get());
		SmartDashboard::PutNumber("Drive R2 Output", DriveRight2.Get());

		SmartDashboard::PutNumber("PWM 6 Motor Output", PWM6.Get());
		SmartDashboard::PutNumber("PWM 7 Motor Output", PWM7.Get());
		SmartDashboard::PutNumber("PWM 8 Motor Output", PWM8.Get());
		SmartDashboard::PutNumber("PWM 9 Motor Output", PWM9.Get());

		// PCM displays
		SmartDashboard::PutBoolean("Drive Solenoid 0 Output",
				DriveSolenoid->Get());
		SmartDashboard::PutBoolean("Solenoid 1 Output", PCM1->Get());
		SmartDashboard::PutBoolean("Solenoid 2 Output", PCM2->Get());
		SmartDashboard::PutBoolean("Solenoid 3 Output", PCM3->Get());
		SmartDashboard::PutBoolean("Solenoid 4 Output", PCM4->Get());

		//limit switch displays
		SmartDashboard::PutBoolean("Upper Limit Switch PWM 7",
				LimSwUp_PWM7.Get());
		SmartDashboard::PutBoolean("Lower Limit Switch PWM 7",
				LimSwDwn_PWM7.Get());
		SmartDashboard::PutBoolean("Upper Limit Switch PWM 8",
				LimSwUp_PWM8.Get());
		SmartDashboard::PutBoolean("Lower Limit Switch PWM 8",
				LimSwDwn_PWM8.Get());


		// current channel displays

		SmartDashboard::PutNumber("TOTAL CURRENT", pdp->GetTotalCurrent());
		SmartDashboard::PutNumber("0_MotorCurrent", pdp->GetCurrent(0));
		SmartDashboard::PutNumber("1_MotorCurrent", pdp->GetCurrent(1));
		SmartDashboard::PutNumber("2_MotorCurrent", pdp->GetCurrent(2));
		SmartDashboard::PutNumber("3_MotorCurrent", pdp->GetCurrent(3));


	}
}
;

START_ROBOT_CLASS(Robot)
