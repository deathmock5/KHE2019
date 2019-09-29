#include "..\include\ClientRequestManager.h"

ResponceHeadder ClientRequestManager::buildReturnHeadders(const int code, const std::string & content) {
	ResponceHeadder res(code, content);
	res["Server"] = "NOPE";
	res["Connection"] = "Close";
	res["Content-Type"] = "text/html; charset=utf-8";
	char buf[1000];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S EST", &tm);
	res["Date"] = buf; //Date: Mon, 18 Jul 2016 16:06:00 GMT
	res["Content-Length"] = std::to_string(content.length());
	return res;
}

std::string ClientRequestManager::getPageContent(const std::string & pagename) {
	return GameDatabase::getPage(pagename.c_str());
}


