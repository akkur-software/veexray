#ifdef USE_LUAJIT_ONE
#pragma comment(lib, "LuaJIT-1.1.8.lib")
#else
#pragma comment(lib, "lua51.lib")
#endif //-USE_LUAJIT_ONE

#include <luajit/src/lua.hpp>
#include "script_additional_libs.h"


extern "C"{
    #include "lfs.h"
    #include "lmarshal.h"
}

static const struct luaL_reg R[] =
{
	{ NULL,	    NULL },
};

int luaopen_lua_extensions(lua_State *L){

    open_additional_libs(L);

    luaopen_lfs(L);
    luaopen_marshal(L);

	luaL_register(L, "lua_extensions", R);
	return 0;
}