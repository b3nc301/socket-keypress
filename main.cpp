#include"config.h"
#include"socket.h"
int main() {
	config cfg;
	char port[4];
	strcpy_s(port,7, cfg.getPort().c_str());
	//KEYBOARD INIT-------------------------------------------
	//Structure for the keyboard event
	INPUT ip;
	//Set up the INPUT structure
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.wVk = cfg.getVkey(); //Set the used key
	ip.ki.dwExtraInfo = 0;
	ip.ki.wScan = 0;  //Set 0 because we use wVK
	//KEYBOARD INIT END-------------------------------------
	std::cout << "Listener is starting...";
	sock s;
	if (s.resolveAddr(port) + s.createSocket() + s.tcpListen() == 0)
	{
		std::cout << "Listener started. Listening to " << cfg.getPort() << ". Use \"" << cfg.getData() <<"\" data to send a keypress. \n";
			while (1) 
			{
				s.tcpAccept();
				s.receiveData(cfg.getData(), ip);
			}
	}
	s.close();
	return 0;
}