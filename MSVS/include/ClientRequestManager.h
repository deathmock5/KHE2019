#pragma once
#include "ClientRequest.h"
#include <vector>
#include "duktape.h"

//Takes a client request and a client, stores it then processes them in a queue returning the result to the client
class ClientRequestManager {
public:
	static ClientRequestManager* instance() {
		if (!_instance) {
			_instance = new ClientRequestManager();
		}
		return _instance;
	}

	static bool bindFunctionToString(duk_ret_t(*func)(duk_context *ctx), const int args, const char* name) {
		return instance()->_bindFunctionToString(func, args, name);
	}
	static void processClientRequest(const char* filename, const char* funcname) {
		instance()->_processClientRequest(filename, funcname);
	}
private:
	ClientRequestManager() {
		ctx = duk_create_heap_default();
	}
	~ClientRequestManager()
	{
		duk_destroy_heap(ctx);
	}

	bool _bindFunctionToString(duk_ret_t(*func)(duk_context *ctx), const int args,const char* name) {
		duk_push_c_function(ctx, func, args);
		duk_put_global_string(ctx, name);
		return true;
	}

	bool _loadFile(const char* filename) {

	}

	void _processClientRequest(const char* filename, const char* funcname) {
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
};