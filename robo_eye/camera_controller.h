#include <string>
#include <stdio.h>
#include <fcntl.h>

#include <algorithm>

#include "serial_writer.h"

using namespace std;

class CameraController
{
    public:
        CameraController(string serialPort, int target_x, int target_y);
        
        void turnCamera(int new_x, int new_y);
        
        int* getNewAngleFromPoint(int new_x, int new_y);
        
		int writeToSerial(char x, char y);
		
    private:
        SerialWriter serial;
        
        int last_x;
        int last_y;
        
        int target_x;
        int target_y;
        
        int lastAngle_x;
        int lastAngle_y;
};