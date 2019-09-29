#pragma once
#include <map>


//GET / HTTP/1.1
//Host: 127.0.0.1
//Connection: keep-alive
//Cache-Control: max-age=0
//Upgrade-Insecure-Requests: 1
//User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/76.0.3809.132 Safari/537.36
//Sec-Fetch-Mode: navigate
//Sec-Fetch-User: ?1
//Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q = 0.8, application / signed - exchange;v = b3
//Sec - Fetch - Site: none
//Accept - Encoding : gzip, deflate, br
//Accept - Language : en - US, en;q = 0.9
//Cookie: dicks = out

class ClientRequest {
public:
	ClientRequest(const std::string& req) {
		_request = req.substr(0,req.find("\r\n")); ////GET / HTTP/1.1
		_type = _request.substr(0, _request.find(" ")); //GET
		_request = _request.substr(_request.find(" ") + 1);
		_request = _request.substr(0,_request.find(" "));
		std::string submessage = req.substr(req.find("\r\n")+2);
		while (submessage.length() > 2) {
			std::string pair = submessage.substr(0, submessage.find("\r\n"));
			submessage = submessage.substr(submessage.find("\r\n")+2);
			// Host: 127.0.0.1
			size_t pos = pair.find(": ");
			if (pos != std::string::npos) {
				_map[pair.substr(0, pos)] = pair.substr(pos + 2);
			}
		}
	}
	std::string &operator[](const std::string& key) {
		return _map[key];
	}
	std::string getTarget() const{
		std::string req = _request.substr(1);
		size_t endq = req.find('?');
		if (endq != std::string::npos) {
			req = req.erase(endq);
		}
		return req;
	}
private:
	std::string _request;
	std::string _type;
	std::map<std::string, std::string> _map;
};