#include <stdlib.h>
#include "lua.h"
#include "lauxlib.h"
#include "mt64.h"

static int g_inited = 0;

/* mt19937 init with array */
static int lrandomseed(lua_State *L) {
	if (g_inited) {
		lua_pushboolean(L, 1);
		return 1;
	}
    int args = lua_gettop(L);
    if (args >= 1) {
		uint64_t *array = (uint64_t *)malloc(sizeof(uint64_t) * args);
		if (!array) {
			lua_pushboolean(L, 0);
			lua_pushstring(L, "init error, not enough memory.");
			return 2;
		}
		int i = 0;
		for (i = 0; i < args; ++i) {
			array[i] = luaL_checkinteger(L, i + 1);
		}
		init_by_array64(array, args);
		free(array);
    } else {
        return luaL_error(L, "mt19937.init need one or more seeds.");
    }
	g_inited = 1;
	lua_pushboolean(L, 1);
	return 1;
}

/* mt19937 rand integer, need 2 integer, return [a, b] or [1, a] */
static int lmtrandom(lua_State *L) {
	int args = lua_gettop(L);
	if (args > 2){
		return luaL_error(L, "mt19937.randi need 0 numbers or 1 number or 2 numbers for a range.");
	}

	/* return double num */
	if (args == 0){
		lua_pushnumber(L, (lua_Number)genrand64_real2());
		return 1;
	}

	/* return int */
	lua_Integer a = 0;
	lua_Integer b = 0;
	if (args == 1) {
		a = 1;
		b = luaL_checkinteger(L, 1) + 1;
	} else if(args == 2) {
		a = luaL_checkinteger(L, 1);
		b = luaL_checkinteger(L, 2) + 1;
	} 
	
	lua_Integer from = a < b ? a : b;
	lua_Integer to = a > b ? a : b;
	if (from == to) {
		lua_pushinteger(L, from);
		return 1;
	}
	lua_pushinteger(L, genrand64_int63() % (to - from) + from);
	return 1;
}


int
luaopen_random(lua_State *L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{ "randomseed", lrandomseed },
		{ "random", lmtrandom },
		{ NULL, NULL }
	};
	luaL_newlib(L,l);
	return 1;
}