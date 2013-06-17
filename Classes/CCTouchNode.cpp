#include "CCTouchNode.h"


namespace sgs {
    namespace ui {

        bool CCTouchNode::CLICKED_ = false;

        CCTouchNode::CCTouchNode(int priority) :priority_(priority)
        {
        }


        CCTouchNode::~CCTouchNode(void)
        {
        }


        void CCTouchNode::onEnter() {
            CCNode::onEnter();
            CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, priority_, false);
        }

        void CCTouchNode::onExit() {
            CCNode::onExit();
            CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
        }

        bool CCTouchNode::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
            m_beginPos = pTouch->getLocationInView();
            m_beginPos = CCDirector::sharedDirector()->convertToGL(m_beginPos);
            if(CLICKED_ == false && touchThis(pTouch)) {
                CLICKED_ = true;
                longTouchInvoked_ = false;
                this->scheduleOnce(schedule_selector(CCTouchNode::ccLongTouch), 2);
                return true;
            }

            return false;
        }

        bool CCTouchNode::touchThis(CCTouch * pTouch) {
            
            if(rect_.size.width == 0 || rect_.size.height == 0) {
                CCSize size = this->getContentSize();

                if(size.width == 0 || size.height == 0) {

                    CCArray* childArray = this->getChildren();
                    int childrenCount = this->getChildrenCount();
                    for(int i = 0; i < childrenCount; i++) {
                        CCNode* child = dynamic_cast<CCNode*>(childArray->objectAtIndex(i));
                        if(child) {
                            CCSize size = child->getContentSize();
                            CCPoint point = child->getAnchorPoint();
                            rect_ = CCRect(child->getPosition().x - point.x*size.width, child->getPosition().y-point.y*size.height, size.width, size.height);
                            break;
                        }
                    }
                }
                else {
                    CCPoint point = getAnchorPoint();
                    rect_ = CCRect(0 - point.x*size.width,0 - point.y*size.height,size.width, size.height);
                }
            }
            


            if(rect_.containsPoint(convertTouchToNodeSpaceAR(pTouch))) {
                //CCLog("contain");
                return true;
            } 
            return false;
        }

        void CCTouchNode::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {
            CCPoint endPos = pTouch->getLocationInView();
            endPos = CCDirector::sharedDirector()->convertToGL(endPos);

            // 判断是否为点击事件
            if (::abs(endPos.x - m_beginPos.x) > 10.0f || 
                ::abs(endPos.y - m_beginPos.y)) 
            {
                cancelTimer();
            }
        }

        void CCTouchNode::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
            CLICKED_ = false;
            cancelTimer();

            CCPoint endPos = pTouch->getLocationInView();
            endPos = CCDirector::sharedDirector()->convertToGL(endPos);

            if(touchThis(pTouch) && !longTouchInvoked_ ) {
                touch();
            }
        }

        void CCTouchNode::cancelTimer() {
            this->unschedule(schedule_selector(CCTouchNode::ccLongTouch));
        }

        void CCTouchNode::ccLongTouch(float dt) {
            longTouchInvoked_ = true;
            longTouch();
        }

        void CCTouchNode::touch() {
        
        }

        void CCTouchNode::longTouch() {
        
        }



    }
}
