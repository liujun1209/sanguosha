#include "CardSelector.h"


namespace sgs {
    namespace selector {

        CardSelector::CardSelector(void):selectCount_(2), cardArray_(new CCArray())
        {

        }


        CardSelector::~CardSelector(void)
        {
            cardArray_->removeAllObjects();
            delete cardArray_;
        }

        void CardSelector::selecteCard(CardItem* cardPtr) {
            int count = cardArray_->count();
            if(count >= selectCount_) {
                int removeCount = selectCount_ - count + 1;
                for(int i =0 ;i < removeCount ;i++ ) {
                    CardItem* cardItem = (CardItem*)cardArray_->objectAtIndex(0);
                    cardItem->unselect();
                    cardArray_->removeObject(cardItem);
                }
            }
            cardPtr->select();
            cardArray_->addObject(cardPtr);
        }
        
        void CardSelector::unselecteCard(CardItem* cardPtr) {
            if(cardArray_->containsObject(cardPtr)) {
                cardArray_->removeObject(cardPtr);
                cardPtr->unselect();
            }
        }

        CCArray* CardSelector::getSelectedCards() {
            return cardArray_;
        }

        void CardSelector::clear() {
            cardArray_->removeAllObjects();
        }

    }
}