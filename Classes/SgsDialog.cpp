#include "SgsDialog.h"

namespace sgs {
    namespace ui {


        SgsDialog::SgsDialog(const CCSize & size): size_(size)
        {
        }


        SgsDialog::~SgsDialog(void)
        {
        }


        bool SgsDialog::init() {
            CCScale9Sprite* sprite = CCScale9Sprite::create("dialogbg.png");
            sprite->setPreferredSize(CCSizeMake(size_.width+ 34, size_.height+ 34));
            sprite->setPosition(0, 0);
            this->addChild(sprite, -1);

            CCSprite * titleSprite = CCSprite::create("dialog_title_bg.png");
            titleSprite->setPosition(ccp(0, size_.height/2 + 10));
            this->addChild(titleSprite, -1);

            return true;
        }

        SgsDialog * SgsDialog::create(const CCSize & size) {
            SgsDialog * pRet =  new SgsDialog(size);
            if(pRet && pRet->init()) {
                pRet->autorelease();
                return pRet;
            }
            CC_SAFE_DELETE(pRet);

            return NULL;
        }

        SgsInputDialog* SgsInputDialog::create(const CCSize & size, const string& lable) {
            SgsInputDialog * pRet =  new SgsInputDialog(size);
            if(pRet && pRet->init(lable)) {
                pRet->autorelease();
                return pRet;
            }
            CC_SAFE_DELETE(pRet);

            return NULL;
        }
        SgsInputDialog::SgsInputDialog(const CCSize & size): SgsDialog(size),label_(NULL),textField_(NULL), okButton_(NULL) {

        }

        SgsInputDialog::~SgsInputDialog() {
        }


        bool SgsInputDialog::init(const string& labelString) {
            SgsDialog::init();
            label_ = CCLabelTTF::create(labelString.c_str(), "Thonburi", 25);
            label_->setColor(ccc3(255 ,255, 0));
            //label_->setPosition(ccp(size_.width/2 , size_.height - 20 ));
            label_->setPosition(ccp(0, 60 ));
            label_->setAnchorPoint(ccp(0.5,0.5));
            this->addChild(label_);
            textField_ = CursorTextField::textFieldWithPlaceHolder("input your name here", "Thonburi", CCSizeMake(size_.width - 80,20));
            textField_->setPosition(ccp(0 , 0 ));;
            textField_->setAnchorPoint(ccp(0.5,0.5));
            this->addChild(textField_);

            /* 正常状态下的按钮图片 */  
            CCScale9Sprite* btnNormal = CCScale9Sprite::create("button_common_button_creat_table_1.png");  
            /* 按钮标题 */  
            CCLabelTTF *title = CCLabelTTF::create("", "Marker Felt", 30);  
      
            /* 按钮的大小会根据标题自动调整 */  
            CCControlButton* controlBtn = CCControlButton::create(title, btnNormal);  
            controlBtn->setPosition(ccp( 0 , textField_->getPosition().y - 52 ));
            controlBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(SgsInputDialog::touchDownAction), CCControlEventTouchDown);  
            controlBtn->setPreferredSize(CCSizeMake(80,30));
            this->addChild(controlBtn);  
            return true;
        }


        void SgsInputDialog::setCallbackFunc(boost::function<void(const string &)> func) {
            func_ = func;
            if(textField_) {
                textField_->setOnEnterFunc(func);
            }
        }

        void SgsInputDialog::touchDownAction(CCObject* pSender, CCControlEvent event){
            if(func_ && textField_) {

                func_(string(textField_->getInputText()));
            }
        }

    }
}