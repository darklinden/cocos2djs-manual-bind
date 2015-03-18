//
//  TestObj.h
//  clearTestJS
//
//  Created by HanShaokun on 17/3/15.
//
//

#ifndef __clearTestJS__TestObj__
#define __clearTestJS__TestObj__

#include "cocos2d.h"

namespace cocos2d {
    
    class TestObj: public cocos2d::Ref
    {
        
    public:
        
        TestObj();
        ~TestObj();
        
        virtual bool init();
        
        CREATE_FUNC(TestObj);
        
        void functionTest(const std::string & param,
                          std::function<void (const std::string & ret)> callback);
        
    private:
        
        std::string  _param;
        
        std::function<void (const std::string & ret)> _callback;
        
        void functionEnd(float delta);
    };
}


#endif /* defined(__clearTestJS__TestObj__) */
