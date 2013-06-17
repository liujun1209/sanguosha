#ifndef __CC_SGS_TOUCH_DELEGATE_H__
#define __CC_SGS_TOUCH_DELEGATE_H__

#include <cocos2d.h>

using namespace cocos2d;

namespace sgs {
    namespace ui {

        class CCTouchNode : public CCNode, public CCTargetedTouchDelegate 
        {
        public:
            CCTouchNode(int priority = 0);
            ~CCTouchNode(void);

            virtual void onEnter();
            virtual void onExit();

            virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
            virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
            
            virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
            
            virtual void ccLongTouch(float dt);

            bool touchThis(CCTouch *pTouch);
            void cancelTimer();
            
            virtual void touch();
            virtual void longTouch();

        protected:
            int priority_;
            CCPoint m_beginPos;
            bool longTouchInvoked_;
            CCRect rect_;
            //全局变量 保证只有一个控件处理事件
            static bool CLICKED_;
        };


    }
}

#endif
