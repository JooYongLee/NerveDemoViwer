#include "Logger.h"

void Logger_test()
{
	string message1 = "logg message 1 ...";
	string message2 = "logg message 2 ...";
	int    nNum = 10;
	CLogger::GetLogger()->Log("message to be logged");
	CLogger::GetLogger()->Log(message1);
	LOGGER->Log(" Message is:%s Number is:%d", message2.c_str(), nNum);
}
int main(int argc, const char *argv[]) {
	
	//view_test();
	Logger_test();

	return 0;
}
