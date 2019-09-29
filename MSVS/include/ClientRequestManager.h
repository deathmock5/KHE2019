#pragma once

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include "GameDatabase.h"
#include "ClientRequest.h"
#include <vector>
#include "duktape.h"
#include "ResponceHeadder.h"
#include <time.h>
#include <iostream>
#include <string>
#include <functional>

//Takes a client request and a client, stores it then processes them in a queue returning the result to the client
class ClientRequestManager {
public:
	static ClientRequestManager* instance() {
		if (!_instance) {
			_instance = new ClientRequestManager();
		}
		return _instance;
	}
	static ResponceHeadder buildReturnHeadders(const int code, const std::string& content);
	static std::string getPageContent(const std::string& pagename);
	static bool bindFunctionToString(duk_ret_t(*func)(duk_context *ctx), const int args, const char* name) {
		return instance()->_bindFunctionToString(func, args, name);
	}

	static ResponceHeadder SubmitAction(ClientRequest& req) {
		std::string targ = req.getTarget();
		//std::map < std::string, std::function<ResponceHeadder(ClientRequest& req)>>::iterator it = rest_handle.find(targ);
		/*if (it != rest_handle.end()) {
			return rest_handle[targ](req);
		}*/
		//Assume its to retrive a file.
		if (FILE *file = fopen(targ.c_str(), "r")) {
			fclose(file);
			std::ifstream myfile(targ);
			std::string filecontents;
			if (myfile.is_open()) {
				myfile.seekg(0, std::ios::end);
				filecontents.reserve(myfile.tellg());
				myfile.seekg(0, std::ios::beg);
				filecontents.assign((std::istreambuf_iterator<char>(myfile)),
					std::istreambuf_iterator<char>());
				return buildReturnHeadders(200, filecontents);
			}
		}
		else {
			FILE* mfile = fopen((targ + "index.html").c_str(), "r");
			if (mfile) {
				fclose(mfile);
				std::ifstream myfile(targ + "index.html");
				std::string filecontents;
				if (myfile.is_open()) {
					myfile.seekg(0, std::ios::end);
					filecontents.reserve(myfile.tellg());
					myfile.seekg(0, std::ios::beg);
					filecontents.assign((std::istreambuf_iterator<char>(myfile)),
						std::istreambuf_iterator<char>());
					return buildReturnHeadders(200, filecontents);
				}
			}
		}
		return buildReturnHeadders(404, "");
	}

	static ResponceHeadder processClientRequest(ClientRequest& req) {
		return instance()->_processClientRequest(req);
	}
	static void replaceObject(const std::string& word2replace,const std::string& change,std::string& target) {
		std::string wordtoreplace = "${" + word2replace + "}";
		size_t pos;
		while ((pos = target.find(wordtoreplace)) != std::string::npos) {
			target = target.replace(pos, wordtoreplace.length(), change);
		}

	}
	static std::string generateButton(std::pair<std::string, std::string> p) {
		return "<td><form action=\"" + p.second + "\"><button type=\"submit\" value=\"" + p.first + "\">" + p.second + "</button></form></td>";
	}
private:
	ClientRequestManager() {
		ctx = duk_create_heap_default();
		rest_handle["/SubmitAction"] = SubmitAction;
	}
	~ClientRequestManager()
	{
		duk_destroy_heap(ctx);
	}

	ResponceHeadder _processClientRequest(ClientRequest& req) {
		std::string file = req.getTarget();
		std::string post = GameDatabase::getPage(file.c_str());
		std::ifstream myfile("index.html");
		std::string filecontents;
		if (myfile.is_open()) {
			myfile.seekg(0, std::ios::end);
			filecontents.reserve(myfile.tellg());
			myfile.seekg(0, std::ios::beg);
			filecontents.assign((std::istreambuf_iterator<char>(myfile)),
				std::istreambuf_iterator<char>());
		}
		std::string button_text;
		auto buttons = GameDatabase::getButtons(file.c_str());
		for (auto set : buttons) {
			button_text += generateButton(set);
		}
		replaceObject("CONTENT", post, filecontents);
		replaceObject("BUTTONS", button_text, filecontents);
		return buildReturnHeadders(200, filecontents);
	}

	bool _bindFunctionToString(duk_ret_t(*func)(duk_context *ctx), const int args,const char* name) {
		duk_push_c_function(ctx, func, args);
		duk_put_global_string(ctx, name);
		return true;
	}

	bool _loadFile(const char* filename) {

	}

	void _doFunc(const char* filename, const char* funcname) {
		/*
			if (!ctx) { exit(1); }
			duk_push_c_function(ctx, dummy_upper_case, 1);
			duk_push_string(ctx, filename);
			duk_call(ctx, 1);
			printf("%s -> %s\n", filename, duk_to_string(ctx, -1));
			duk_pop(ctx);
		*/
		duk_eval_string_noresult(ctx, "print('2+3=' + adder(2, 3));");
	}
	static ClientRequestManager* _instance;
	std::vector<ClientRequest> _clientRequests;
	duk_context *ctx;
	std::map < std::string, std::function<ResponceHeadder(ClientRequest& req)>> rest_handle;
};