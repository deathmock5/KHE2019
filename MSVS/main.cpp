#include <iostream>
#include <ios>
#include "GameDatabase.h"

#include "SocketServer.h"
#include "ClientRequestManager.h"

GameDatabase* GameDatabase::_instance;
ClientRequestManager* ClientRequestManager::_instance;

static int dummy_upper_case(duk_context *ctx) {
	size_t sz;
	const char *val = duk_require_lstring(ctx, 0, &sz);
	size_t i;

	/* We're going to need 'sz' additional entries on the stack. */
	duk_require_stack(ctx, sz);

	for (i = 0; i < sz; i++) {
		char ch = val[i];
		if (ch >= 'a' && ch <= 'z') {
			ch = ch - 'a' + 'A';
		}
		duk_push_lstring(ctx, (const char *)&ch, 1);
	}

	duk_concat(ctx, sz);
	return 1;
}

static duk_ret_t native_print(duk_context *ctx) {
	printf("%s\n", duk_to_string(ctx, 0));
	return 0;  /* no return value (= undefined) */
}

/* Adder: add argument values. */
static duk_ret_t native_adder(duk_context *ctx) {
	int i;
	int n = duk_get_top(ctx);  /* #args */
	double res = 0.0;

	for (i = 0; i < n; i++) {
		res += duk_to_number(ctx, i);
	}

	duk_push_number(ctx, res);
	return 1;  /* one return value */
}

int main(int* argc, int ** args) {
	
	//Start the server.
	SocketServer server(80, "127.0.0.1");
	ClientRequestManager::bindFunctionToString(dummy_upper_case, 1, "upper");
	ClientRequestManager::bindFunctionToString(native_print, 1, "print");
	ClientRequestManager::bindFunctionToString(native_adder, -1, "adder");
	ClientRequestManager::processClientRequest("test", "test2");

	//TODO: Create instance of the socket server.
	//TODO: load XML documents to take text.
	//TODO: Create instance of SQLite to manage data?

	GameDatabase::instance();
	std::cin.get();
	return EXIT_SUCCESS;
}