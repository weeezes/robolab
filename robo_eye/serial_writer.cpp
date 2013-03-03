#include "serial_writer.h"

SerialWriter::SerialWriter()
{
	
}

int SerialWriter::openPort(string serialPort)
{
	fcntl(this->serial, F_SETFL, O_NONBLOCK);
	this->serial = open(serialPort.c_str(), O_RDWR | O_NONBLOCK);

	return this->serial;
}

int SerialWriter::writeToPort(char *out, int size)
{
	if (this->serial != -1)
	{
		fcntl(this->serial, F_SETFL, 0);
		int wr = write(this->serial, out, size);
		
		return wr;
	} else {
		return -1;
	}
}