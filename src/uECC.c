// uECC lua bindings

#include <lua.h>
#include <lauxlib.h>
#include <uECC.h>
#include <assert.h>

static int luECC_keygen(lua_State *L) {
    uECC_Curve curve = luaL_checkudata(L, 1, "uECC_curve");
    luaL_Buffer bpk, bsk;
    size_t curve_size = uECC_curve_private_key_size(curve);
    uint8_t* pk = (uint8_t*)luaL_buffinitsize(L, &bpk, curve_size * 2);
    uint8_t* sk = (uint8_t*)luaL_buffinitsize(L, &bsk, curve_size);
    uECC_make_key(pk, sk, curve);
    lua_createtable(L, 0, 2);
    luaL_pushresultsize(&bpk, curve_size * 2);
    lua_setfield(L, -2, "pk");
    luaL_pushresultsize(&bsk, curve_size);
    lua_setfield(L, -2, "sk");
    return 1;
};

static int luECC_shared_secret(lua_State *L) {
    uECC_Curve curve = luaL_checkudata(L, 1, "uECC_curve");
    size_t curve_size = uECC_curve_private_key_size(curve);
	size_t pklen, sklen;
	const char *pk = luaL_checklstring(L, 2, &pklen);
	luaL_argcheck(L, pklen == curve_size * 2, 2, "invalid key length");
	const char *sk = luaL_checklstring(L, 3, &sklen);
	luaL_argcheck(L, sklen == curve_size, 3, "invalid key length");
    luaL_Buffer b;
    uint8_t* secret = (uint8_t*)luaL_buffinitsize(L, &b, curve_size);
    if(!uECC_shared_secret((const uint8_t*) pk, (const uint8_t*) sk, secret, curve)) {
		lua_pushnil(L);
		return 1;
    }
    luaL_pushresultsize(&b, curve_size);
    return 1;
}

static int luECC_secp192r1(lua_State *L) {
    uECC_Curve curve = uECC_secp192r1();
    lua_pushlightuserdata(L, (void*) curve);
	luaL_setmetatable(L, "uECC_curve");
    return 1;
};

int luaopen_uECC(lua_State *L) {
    static const luaL_Reg lib[] = {
        {"secp192r1", luECC_secp192r1},
        {NULL, NULL}
    };
    static const luaL_Reg methods[] = {
        {"keygen", luECC_keygen},
        {"sharedsecret", luECC_shared_secret},
        {NULL, NULL}
    };
    luaL_newmetatable(L, "uECC_curve");
    luaL_newlib(L, methods);
    lua_setfield(L, -2, "__index");
    lua_pop(L, 1);

    luaL_newlib(L, lib);
    return 1;
}
