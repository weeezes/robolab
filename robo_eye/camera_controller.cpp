#include "camera_controller.h"

CameraController::CameraController(string serialPort, int target_x, int target_y)
{
	this->last_x = 0;
	this->last_y = 0;
	
	this->target_x = target_x;
	this->target_y = target_y;
	
	this->lastAngle_x = 90;
	this->lastAngle_y = 90;
	
	int serial_status = this->serial.openPort(serialPort);
	if (serial_status == -1)
	{
		printf("Failed to open serial port\n");
	}
	else
	{
		writeToSerial(this->lastAngle_x, this->lastAngle_y);
	}
}

void CameraController::turnCamera(int new_x, int new_y)
{
	int *angles = getNewAngleFromPoint(new_x, new_y);
	
	int s = writeToSerial(angles[0], angles[1]);
	if (s != -1)
	{
		printf("SENT %d %d\n", angles[0], angles[1]);
	}
	else
	{
		printf("Failed to send data to serial device");
	}
	
	this->last_x = new_x;
	this->last_y = new_y;
	
	this->lastAngle_x = angles[0];
	this->lastAngle_y = angles[1];
	
	delete [] angles;
}

int* CameraController::getNewAngleFromPoint(int new_x, int new_y)
{
	int *angles = new int[2];

	if (new_x < this->target_x)
		angles[0] = std::max(0, this->lastAngle_x-1);
	else
		angles[0] = std::min(180, this->lastAngle_x+1);
		
	if (new_y < this->target_y)
		angles[1] = std::max(0, this->lastAngle_y-1);
	else
		angles[1] = std::min(180, this->lastAngle_y+1);
	
	return angles;
}

int CameraController::writeToSerial(char x, char y)
{
	char out[8];
	sprintf(out, "%d\t%d\n", x, y);
	
	return this->serial.writeToPort(out, 8);
}