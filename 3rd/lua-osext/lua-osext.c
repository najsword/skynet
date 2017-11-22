/*
	扩展系统os方法
*/

#define LUA_LIB

#include <lua.h>
#include <lauxlib.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <uuid/uuid.h>

const char SEC  = 	2;
const char MSEC = 	4;
const char USEC = 	8;


//适用于90进制及以下
static const char* encoding = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~!@#$%^&*()_+-=[]{}|;:,./<>?";

//字节流转为任意进制 digital进制数
static int to_basex(const uint8_t * src, int src_len, int digital, char * output){
	//64进制
	int i = 0;
	int j = src_len / 8;
	const uint8_t * p = src;
	if (src_len % 8 != 0){
		j++;
	}
	int sz = 0;
	char * pout = output;
	for (i=0; i<j; i++){
		uint64_t v = *(uint64_t *)(p + i*8);
		while (v != 0){
			uint64_t mod = v % digital;
			*pout = encoding[mod];
			pout++;
			sz++;
			v = v / digital;
		}
	}
	*pout = '\0';
	sz++;
	return sz;
}


//转为62进制
static void gen_encoding_uuid(char* result, int len){
	unsigned char uuid[16];
	char output[128] = { 0 };
	uuid_generate(uuid);

	to_basex(uuid, 16, 62, output);

	len = (len > sizeof(output)) ? sizeof(output) : len;
	memcpy(result, output, len);
}


//从62进制里截取8位
static int luuid_str8(lua_State *L){
	char buffer[9] = { 0 };
	gen_encoding_uuid(buffer, 8);
	lua_pushstring(L, buffer);
	return 1;
}

//从62进制里截取20位
static int luuid_str20(lua_State *L){
	char buffer[21] = { 0 };
	gen_encoding_uuid(buffer, 20);
	lua_pushstring(L, buffer);
	return 1;
}

//产生正常的uuid
static int luuid_str(lua_State *L){
	uuid_t uuid;
	char buffer[37] = { 0 };
	uuid_generate(uuid);
	uuid_unparse_lower(uuid, buffer);
	lua_pushstring(L, buffer);
	return 1;
}

//产生hex表示的uuid
static int luuid_hex(lua_State *L){
	unsigned char uuid[16];
	uuid_generate(uuid);
	char buffer[17] = { 0 };
	snprintf(buffer, 17, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
		uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7],
		uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);
	lua_pushstring(L, buffer);
	return 1;
}

//生成binary的uuid
static int luuid_binary(lua_State *L){
	unsigned char uuid[16] = { 0 };
	uuid_generate(uuid);
	lua_pushlstring(L, (const char *)uuid, 16);
	return 1;
}

//产生以num表示的uuid
static int luuid_num(lua_State *L){
	unsigned char uuid[16];
	uuid_generate(uuid);
	int i;
	unsigned long long v1 = 0;
	unsigned long long v2 = 0;
	for (i=0; i<8; i++){
		v1 = uuid[i] + (v1 << 8);
	}
	for (i=8; i<16; i++){
		v2 = uuid[i] + (v2 << 8);
	}
	lua_pushinteger(L, v1);
	lua_pushinteger(L, v2);
	return 2;
}

/* time helper function */
static double get_time( const char k ){
	
	struct timeval tv;
	gettimeofday( &tv, NULL );
	
	if( k==SEC ) 		return tv.tv_sec;
	else if( k==MSEC )	return (tv.tv_sec + (double)((int)(tv.tv_usec*0.001) * 0.001));
	else if( k==USEC )	return (tv.tv_usec*0.000001);
	else 				return 0;
}

/* get miliseconds relative to seconds since EPOCH */
static int lmili (lua_State *L) {
	
	lua_pushnumber(L, get_time( MSEC ) );
	return 1;
}

/* get seconds since EPOCH */
static int lseconds (lua_State *L) {
	
	lua_pushnumber( L, get_time( SEC ) );
	return 1;
}

/* get microseconds relative to seconds since EPOCH */
static int lmicro (lua_State *L) {
	
	lua_pushnumber(L, get_time( USEC ) );
	return 1;
}

/* return seconds, miliseconds and microseconds */
static int ltime (lua_State *L){
	
	struct timeval tv;
	gettimeofday( &tv, NULL );
	
	lua_pushnumber( L, tv.tv_sec );
	lua_pushnumber( L, (double)((int)(tv.tv_usec * 0.001) * 0.001) );
	lua_pushnumber( L, (double)(tv.tv_usec * 0.000001) );
	
	return 3;
}

/* return the diference in miliseconds relative to seconds since EPOCH */
static int ldiff (lua_State *L){
	
	double v1= (double)luaL_checknumber( L, 1 );
	
	lua_pushnumber( L, ( get_time( MSEC ) - v1 ) );
	return 1;
}

/* return timestamp use millsecond */
static int ltimestamp(lua_State *L) {
	struct timeval tv;
	gettimeofday(&tv, 0);
	lua_pushnumber( L, (double)(tv.tv_sec * 1000 + tv.tv_usec / 1000) );
	return 1;
}

/* 二进制转为[2-90]进制 */
static int lto_basex(lua_State *L){
	size_t sz = 0;
	const uint8_t * text = (const uint8_t *)luaL_checklstring(L, 1, &sz);
	uint64_t x = (uint64_t)luaL_checknumber( L, 2 );
	char buffer[256] = { 0 };
	int ret = to_basex(text, sz, x, buffer);
	lua_pushlstring(L, buffer, ret);
	return 1;
}

LUAMOD_API int
luaopen_osext(lua_State *L){
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{ "mili", lmili },
		{ "seconds", lseconds },
		{ "micro", lmicro },
		{ "time", ltime },
		{ "timestamp", ltimestamp },
		{ "diff", ldiff },
		{ "uuid_str", luuid_str },
		{ "uuid_str8", luuid_str8 },
		{ "uuid_str20", luuid_str20 },
		{ "uuid_hex", luuid_hex },
		{ "uuid_num", luuid_num },
		{ "uuid_binary", luuid_binary },
		{ "to_basex", lto_basex },
		{ NULL, NULL },
	};
	luaL_newlib(L, l);
	return 1;
}