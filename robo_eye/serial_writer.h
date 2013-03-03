#include <string>
#include <stdio.h>
#include <fcntl.h>

using namespace std;

class SerialWriter
{
	public:
		SerialWriter();
		
		int openPort(string serialPort);
		
		int writeToPort(char *out, int size);
		
	private:
		int serial;
};