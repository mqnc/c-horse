void lua_pushentry(lua_State* L, string key, string value) {
    lua_pushstring(L, key.c_str());
    lua_pushstring(L, value.c_str());
    lua_settable(L, -3);
}

void lua_pushentry(lua_State* L, string key, int value) {
    lua_pushstring(L, key.c_str());
    lua_pushinteger(L, value);
    lua_settable(L, -3);
}

void lua_pushentry(lua_State* L, int key, string value) {
    lua_pushinteger(L, key);
    lua_pushstring(L, value.c_str());
    lua_settable(L, -3);
}





	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	auto result = luaL_loadfile(L, "parser.lua") || lua_pcall(L, 0, 0, 0);

	if (result){
		cout << "error loading parser.lua: " << lua_tostring(L, -1);
		return EXIT_FAILURE;
	}

	lua_getglobal(L, "grammar");
	if(!lua_isstring(L, 1)){
		cout << "no grammar string found!" << endl;
		return EXIT_FAILURE;
	}
	
	
		
	lua_close(L);
	
	
	
			lua_getglobal(L, rulename);
		if(lua_isfunction(L)){
			parser[rulename] = [](const SemanticValues& sv, any& dt) {
				std::cout << "yeah ABC" << std::endl;
				return sv[0].get<string>() + sv[1].get<string>() + sv[2].get<string>();
			};
		}