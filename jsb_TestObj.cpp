//
//  jsb_ls_auto.cpp
//  jsb
//
//  Created by leafsoar on 8/1/13.
//
//

#include "jsb_TestObj.h"
#include "TestObj.h"

#include "cocos2d.h"
#include "cocos2d_specifics.hpp"
#include "js_manual_conversions.h"

static JSClass  *jsb_TestObj_class;
static JSObject *jsb_TestObj_prototype;
static JSObject *jsb_namespace_binding;

USING_NS_CC;

//js binding register
void jsb_register_TestObj(JSContext* cx, JSObject* obj)
{
    //namespace
    jsb_TestObj_regNamespace(cx, obj, "ns");
    
    //class
    jsb_TestObj_initClass(cx, jsb_namespace_binding, "TestObj");
    
    JS_DefineFunction(cx, jsb_TestObj_prototype, "retain", jsb_TestObj_retain, 0, JSPROP_ENUMERATE | JSPROP_PERMANENT);
    JS_DefineFunction(cx, jsb_TestObj_prototype, "release", jsb_TestObj_release, 0, JSPROP_ENUMERATE | JSPROP_PERMANENT);
}

static bool jsb_TestObj_regNamespace(JSContext* cx, JSObject* obj, const char* name)
{
    JS::RootedValue  nsval(cx);
    JS::RootedObject ns(cx);
    JS_GetProperty(cx, obj, name, &nsval);
    if (nsval == JSVAL_VOID) {
        ns = JS_NewObject(cx, NULL, NULL, NULL);
        nsval = OBJECT_TO_JSVAL(ns);
        JS_SetProperty(cx, obj, name, nsval);
    } else {
        JS_ValueToObject(cx, nsval, &ns);
    }
    jsb_namespace_binding = ns;
    
    return true;
}

//() & retain()
static bool jsb_TestObj_constructor(JSContext *cx, unsigned argc, JS::Value *vp)
{
    if (argc == 0) {
        JSObject *jsobj = JS_NewObject(cx, jsb_TestObj_class, jsb_TestObj_prototype, NULL);
        TestObj *newObject = new TestObj();
        JS_SetPrivate(jsobj, (void *)newObject);
        JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(jsobj));
        return true;
    }
    
    JS_ReportError(cx, "Wrong number of arguments: %d, was expecting: %d", argc, 0);
    return false;
}

//~() & release()
static void jsb_TestObj_finalize(JSFreeOp *fop, JSObject *obj)
{
    log("jsbindings: finalizing JS object %p (TestObj)", obj);
    TestObj *newObject = (TestObj *)JS_GetPrivate(obj);
    JS_SetPrivate(obj, NULL);
    delete newObject;
}

//alloc & init
static void jsb_TestObj_initClass(JSContext *cx, JSObject* globalObj, const char* name)
{
    jsb_TestObj_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_TestObj_class->name = name;
    jsb_TestObj_class->addProperty = JS_PropertyStub;
    jsb_TestObj_class->delProperty = JS_DeletePropertyStub;
    jsb_TestObj_class->getProperty = JS_PropertyStub;
    jsb_TestObj_class->setProperty = JS_StrictPropertyStub;
    jsb_TestObj_class->enumerate = JS_EnumerateStub;
    jsb_TestObj_class->resolve = JS_ResolveStub;
    jsb_TestObj_class->convert = JS_ConvertStub;
    jsb_TestObj_class->finalize = jsb_TestObj_finalize;
    jsb_TestObj_class->flags = JSCLASS_HAS_PRIVATE;
    
    // no property for this class
    static JSPropertySpec properties[] = {
        {0, 0, 0, 0, 0}
    };
    
    // define member function
    static JSFunctionSpec funcs[] = {
        JS_FN("functionTest", jsb_TestObj_functionTest, 2, JSPROP_PERMANENT  | JSPROP_ENUMERATE),
        JS_FS_END
    };
    
    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", jsb_TestObj_create, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };
    
    jsb_TestObj_prototype = JS_InitClass(
                                         cx,
                                         globalObj,
                                         NULL,  //parent proto
                                         jsb_TestObj_class,
                                         jsb_TestObj_constructor,
                                         0,
                                         properties,
                                         funcs,
                                         NULL,  //no static properties
                                         st_funcs);
    
    TypeTest<cocos2d::TestObj> t;
    js_type_class_t *p;
    std::string typeName = t.s_name();
    if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
    {
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->jsclass = jsb_TestObj_class;
        p->proto = jsb_TestObj_prototype;
        p->parentProto = NULL;
        _js_global_type_map.insert(std::make_pair(typeName, p));
    }
}

// 静态函数 create 的具体实现
static bool jsb_TestObj_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc == 0, cx, false, "Invalid number of arguments");
    
    TestObj* ret = TestObj::create();
    
    jsval jsret;
    do {
        if (ret) {
            js_proxy_t *proxy = js_get_or_create_proxy<TestObj>(cx, ret);
            jsret = OBJECT_TO_JSVAL(proxy->obj);
        } else {
            jsret = JSVAL_NULL;
        }
    } while (0);
    JS_SET_RVAL(cx, vp, jsret);
    
    return true;
}

static bool jsb_TestObj_retain(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *thisObj = JS_THIS_OBJECT(cx, vp);
    if (thisObj) {
        js_proxy_t *proxy = jsb_get_js_proxy(thisObj);
        if (proxy) {
            ((Ref *)proxy->ptr)->retain();
            return true;
        }
    }
    JS_ReportError(cx, "Invalid Native Object.");
    return false;
}

static bool jsb_TestObj_release(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *thisObj = JS_THIS_OBJECT(cx, vp);
    if (thisObj) {
        js_proxy_t *proxy = jsb_get_js_proxy(thisObj);
        if (proxy) {
            ((Ref *)proxy->ptr)->release();
            return true;
        }
    }
    JS_ReportError(cx, "Invalid Native Object.");
    return false;
}

//functionTest function bind
static bool jsb_TestObj_functionTest(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSB_PRECONDITION2(argc == 2, cx, false, "Invalid number of arguments");
    jsval *argv = JS_ARGV(cx,vp);
    JSObject *jsObj = (JSObject *)JS_THIS_OBJECT(cx, vp);
    TestObj *newObject = (TestObj *)JS_GetPrivate(jsObj);
    if (!newObject) {
        if (jsObj) {
            js_proxy_t *proxy = jsb_get_js_proxy(jsObj);
            if (proxy) {
                newObject = (TestObj *)proxy->ptr;
            }
        }
    }
    
    JSB_PRECONDITION2(newObject, cx, false, "Invalid number of arguments");
    
    bool ok = true;
    std::string arg0;
    std::function<void (const std::string & ret)> arg1;
    ok &= jsval_to_std_string(cx, argv[0], &arg0);
    do {
        if(JS_TypeOfValue(cx, argv[1]) == JSTYPE_FUNCTION)
        {
            std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[1]));
            auto lambda = [=](const std::string & larg0) -> void {
                JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
                jsval largv[1];
                do {
                    if (larg0.length()) {
                        //if param exist, pass value
                        largv[0] = std_string_to_jsval(cx, larg0);
                    } else {
                        //if not pass null
                        largv[0] = JSVAL_NULL;
                    }
                } while (0);
                jsval rval;
                bool ok = func->invoke(1, &largv[0], rval);
                if (!ok && JS_IsExceptionPending(cx)) {
                    JS_ReportPendingException(cx);
                }
            };
            arg1 = lambda;
        }
        else
        {
            arg1 = nullptr;
        }
    } while(0)
        ;
    JSB_PRECONDITION2(ok, cx, false, "js_cocos2dx_EventListenerCustom_create : Error processing arguments");
    newObject->functionTest(arg0, arg1);
    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return true;
}
