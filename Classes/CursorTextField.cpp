#include "CursorTextField.h"
#include <cocos-ext.h>

const static float DELTA = 0.5f;

using namespace cocos2d::extension;

namespace sgs {
    namespace ui {

        CursorTextField::CursorTextField()
        {
            CCTextFieldTTF();
            m_pCursorSprite = NULL;
            m_pCursorAction = NULL;

            m_pInputText = NULL;
        }

        CursorTextField::~CursorTextField()
        {
            delete m_pInputText;
        }

        void CursorTextField::onEnter()
        {
            CCTextFieldTTF::onEnter();
            CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
            this->setDelegate(this);
        }

        string CursorTextField::getInputText() {
            return *m_pInputText;
        }

        CursorTextField * CursorTextField::textFieldWithPlaceHolder(const char *placeholder, const char *fontName, CCSize inputSize)
        {
            CursorTextField *pRet = new CursorTextField();
            
            if(pRet && pRet->initWithString("", fontName, inputSize.height))
            {
                CCScale9Sprite* sprite = CCScale9Sprite::create("dialog_input_frame.png");
                sprite->setContentSize(CCSizeMake((inputSize.width+18), (inputSize.height + 10)));
                sprite->setPosition(ccp(-9,-5));
                sprite->setAnchorPoint(ccp(0, 0));
                pRet->addChild(sprite, -1);
                pRet->autorelease();
                if (placeholder)
                {
                    pRet->setPlaceHolder(placeholder);
                }
                pRet->initCursorSprite(inputSize.height);
                pRet->m_fieldSize = inputSize;
                pRet->setContentSize(inputSize);
                return pRet;
            }

            CC_SAFE_DELETE(pRet);

            return NULL;
        }

        void CursorTextField::initCursorSprite(int nHeight)
        {
            // 初始化光标
            int column = 4;
            int * pixels = new int[nHeight * column];
            for (int i=0; i<nHeight; ++i) {
                for (int j=0; j<column; ++j) {
                    pixels[i * column + j] = 0xffffffff;
                }
            }

            CCTexture2D *texture = new CCTexture2D();
            texture->initWithData(pixels, kCCTexture2DPixelFormat_RGB888, 1, 1, CCSizeMake(column, nHeight));
            delete pixels;

            m_pCursorSprite = CCSprite::createWithTexture(texture);
            CCSize winSize = getContentSize();
            m_cursorPos = ccp(0, winSize.height / 2);
            m_pCursorSprite->setPosition(m_cursorPos);
            this->addChild(m_pCursorSprite);

            m_pCursorAction = CCRepeatForever::create((CCActionInterval *) CCSequence::create(CCFadeOut::create(0.25f), CCFadeIn::create(0.25f), NULL));

            m_pCursorSprite->runAction(m_pCursorAction);

            m_pInputText = new std::string();
            m_pCursorSprite->setVisible(false);
        }

        bool CursorTextField::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
        {    
            m_beginPos = pTouch->getLocationInView();
            m_beginPos = CCDirector::sharedDirector()->convertToGL(m_beginPos);

            return true;
        }

        CCRect CursorTextField::getRect()
        {
            CCPoint point = this->getPosition();
            CCPoint anchorPoint = this->getAnchorPoint();
            CCRect rect =   CCRectMake(point.x - anchorPoint.x*m_fieldSize.width  , point.y -anchorPoint.y*m_fieldSize.height ,
                m_fieldSize.width, m_fieldSize.height);
            return rect;
        }

        bool CursorTextField::isInTextField(cocos2d::CCTouch *pTouch)
        {
            return getRect().containsPoint(convertTouchToNodeSpaceAR(pTouch));
        }

        void CursorTextField::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
        {
            CCPoint endPos = pTouch->getLocationInView();
            endPos = CCDirector::sharedDirector()->convertToGL(endPos);

            // 判断是否为点击事件
            if (::abs(endPos.x - m_beginPos.x) > DELTA || 
                ::abs(endPos.y - m_beginPos.y)) 
            {
                // 不是点击事件
                m_beginPos.x = m_beginPos.y = -1;

                return;
            }

            CCLOG("width: %f, height: %f.", getContentSize().width, getContentSize().height);

            // 判断是打开输入法还是关闭输入法
            isInTextField(pTouch) ? openIME() : closeIME();
        }

        bool CursorTextField::onTextFieldAttachWithIME(cocos2d::CCTextFieldTTF *pSender)
        {
            if (m_pInputText->empty()) {
                return false;
            }

            m_pCursorSprite->setPositionX(contentSize.width);

            return false;
        }

        bool CursorTextField::onTextFieldInsertText(cocos2d::CCTextFieldTTF *pSender, const char *text, int nLen)
        {
            //CCLOG("Width: %f", pSender->getContentSize().width);
            //CCLOG("Text: %s", text);
            //CCLOG("Length: %d", nLen);
            if(*text == ' ') {
                return true;
            }
            if(*text != '\r' && *text != '\n') {
                m_pInputText->append(text);
                setString(m_pInputText->c_str());
                contentSize = getContentSize();
                m_pCursorSprite->setPositionX(contentSize.width);
                this->setContentSize(m_fieldSize);
            }
            else {
                closeIME();
                //回车回调
                if(func_) func_(*this->m_pInputText);
            }
            
            return true;
        }

        bool CursorTextField::onTextFieldDeleteBackward(cocos2d::CCTextFieldTTF *pSender, const char *delText, int nLen)
        {
            m_pInputText->resize(m_pInputText->size() - nLen);
            setString(m_pInputText->c_str());
            contentSize = getContentSize();
            m_pCursorSprite->setPositionX(contentSize.width);

            if (m_pInputText->empty()) {
                m_pCursorSprite->setPositionX(0);
            }
            
            this->setContentSize(m_fieldSize);
            return false;
        }

        bool CursorTextField::onTextFieldDetachWithIME(cocos2d::CCTextFieldTTF *pSender)
        {
            return false;
        }

        void CursorTextField::openIME()
        {
            m_pCursorSprite->setVisible(true);
            this->attachWithIME();
        }

        void CursorTextField::closeIME()
        {
            m_pCursorSprite->setVisible(false);
            this->detachWithIME();
        }
        void CursorTextField::setOnEnterFunc(boost::function<void(const string&)> func) {
            func_ = func;
        }

        void CursorTextField::onExit()
        {
            CCTextFieldTTF::onExit();
            CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
        }

    }
}