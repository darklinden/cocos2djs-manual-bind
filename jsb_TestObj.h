//
//  jsb_ls_auto.h
//  jsb
//
//  Created by leafsoar on 8/1/13.
//
//

#ifndef jsb_jsb_ls_auto_h
#define jsb_jsb_ls_auto_h

#include "jsapi.h"
#include "jsfriendapi.h"
#include "ScriptingCore.h"

void jsb_register_TestObj(JSContext* cx, JSObject* obj);

static bool jsb_TestObj_regNamespace(JSContext* cx, JSObject* obj, const char* name);
static bool jsb_TestObj_constructor(JSContext *cx, unsigned argc, JS::Value *vp);
static void jsb_TestObj_finalize(JSFreeOp *fop, JSObject *obj);
static void jsb_TestObj_initClass(JSContext *cx, JSObject* globalObj, const char* name);

static bool jsb_TestObj_create(JSContext *cx, uint32_t argc, jsval *vp);
static bool jsb_TestObj_retain(JSContext *cx, uint32_t argc, jsval *vp);
static bool jsb_TestObj_release(JSContext *cx, uint32_t argc, jsval *vp);

static bool jsb_TestObj_functionTest(JSContext *cx, uint32_t argc, jsval *vp);

#endif
