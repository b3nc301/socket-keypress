#pragma once
#include <string>
#include <fstream>
#include<iostream>

class config
{
	std::string data;	//Data to trigget button
	std::string port;	//Port where app is listening
	int vkey;	//button what is pressed
	std::ifstream file;
	std::string line;
public:
	config();
	std::string getPort() const;
	std::string getData() const;
	int getVkey() const;
};

