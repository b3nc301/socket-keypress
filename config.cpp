#include "config.h"
config::config() :file("config.cfg"), data("F5"),port("2510"),vkey(0x74) {
	if (!file.is_open()) {
		std::cerr << "No config file found. Fallback to original values. \n";
	}
	if (file.is_open()) {
		while (getline(file, line))
		{
			line.erase(std::remove_if(line.begin(), line.end(), isspace),
				line.end());
			if (line.empty() || line[0] == '#')
			{
				continue;
			}
			auto delimiterPos = line.find("=");
			auto name = line.substr(0, delimiterPos);
			auto value = line.substr(delimiterPos + 1);
			if (name == "DATA")
				data = value;
			else if (name == "PORT")
				if (value.length() == 4 && value!="0000")
					port = value;
				else std::cerr << "Config error. Wrong port number. \n";
			else if (name == "VKEY")
				vkey =  stoi(value);
		}

	}

};
std::string config::getData() const{
	return data;
}
std::string config::getPort() const{
	return port;
}
int config::getVkey() const{
	return vkey;
}