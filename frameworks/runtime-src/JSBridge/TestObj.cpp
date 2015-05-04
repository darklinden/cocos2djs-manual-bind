//
//  TestObj.cpp
//  clearTestJS
//
//  Created by HanShaokun on 17/3/15.
//
//

#include "TestObj.h"

namespace cocos2d {

    
    TestObj::TestObj()
    {
        log("TestObj %p", this);
    }
    
    TestObj::~TestObj()
    {
        log("~TestObj %p", this);
    }
    
    bool TestObj::init()
    {
        return true;
    }
    
    void TestObj::functionTest(const std::string &param, std::function<void (const std::string &)> callback)
    {
        _param = std::string(param);
        _callback = callback;
        
        Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(TestObj::functionEnd), this, 3, 0, 1, false);
    }
    
    void TestObj::functionEnd(float delta)
    {
        if (_callback) {
            _callback(_param);
        }
    }
}
