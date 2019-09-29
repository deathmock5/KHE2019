#pragma once
#include <string>
#include <map>
#include <windows.h>
#include <stringapiset.h>
#include <map>


class ResponceHeadder {
public:
	std::map<int, std::string> responces{
		{ 200,"OK" },
		{ 404,"Not Found" },
		{ 403,"Forbidden" },
		{ 400,"Bad Request" },
		{ 405,"Method Not Allowed" }
	};
	ResponceHeadder(int returncode, const std::string& content) {
		_code = returncode;
		_content = content;
	}
	std::string &operator[](const std::string& key) {
		return _map[key];
	}
	std::string outPut() {
		std::string output;
		output += "HTTP/1.1 " + std::to_string(_code) + " " + responces[_code] + "\r\n";
		for (std::map<std::string, std::string>::iterator it = _map.begin(); it != _map.end(); ++it){
			output += it->first + ": " + it->second + "\r\n";
		}
		output += "\r\n";
		output += _content;
		/*int size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, output.c_str(),
			output.length(), nullptr, 0);
		std::wstring utf16_str(size, '\0');
		MultiByteToWideChar(CP_ACP, MB_COMPOSITE, output.c_str(),
			output.length(), &utf16_str[0], size);

		int utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
			utf16_str.length(), nullptr, 0,
			nullptr, nullptr);
		std::string utf8_str(utf8_size, '\0');
		WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
			utf16_str.length(), &utf8_str[0], utf8_size,
			nullptr, nullptr);*/
		return output;
	}
private:
	int _code;
	std::string _content;
	std::map<std::string, std::string> _map;
};