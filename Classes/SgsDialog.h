#ifndef __SGS_DIALOG_H__
#define __SGS_DIALOG_H__

#include <cocos2d.h>
#include <cocos-ext.h>
#include <boost\function.hpp>
#include "CursorTextField.h"

using namespace boost;
using namespace cocos2d;
using namespace cocos2d::extension;

namespace sgs {
    namespace ui {

        class SgsDialog :public CCNode
        {
        public:
            SgsDialog(const CCSize & size);
            ~SgsDialog(void);
            bool init();
            static SgsDialog * create(const CCSize & size);
            CC_SYNTHESIZE_PASS_BY_REF(CCSize, size_, Size);
        };

        class SgsInputDialog : public SgsDialog {
        public :
            SgsInputDialog(const CCSize & size);
            ~SgsInputDialog();
            bool init(const string& labelString);
            static SgsInputDialog* create(const CCSize & size, const string& lableString);
            
            void setCallbackFunc(boost::function<void(const string&)> func);
        private:
            void touchDownAction(CCObject* pSender, CCControlEvent event); 
            //提示文本
            CCLabelTTF * label_; 
            //输入框
            CursorTextField * textField_;
            //确定按钮
            CCControlButton * okButton_;
            //callback
            boost::function<void(const string&)> func_;
        };

    }
}

#endif