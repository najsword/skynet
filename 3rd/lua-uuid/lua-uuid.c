#define LUA_LIB

#include <lua.h>
#include <lauxlib.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <uuid/uuid.h>



LUAMOD_API int
luaopen_uuid(lua_State *L){
	luaL_checkversion(L);
	luaL_Reg l[] = {
		
		{ NULL, NULL },
	};
	luaL_newlib(L, l);
	return 1;
}