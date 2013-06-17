#include "Card.h"

namespace sgs {
    namespace core {

        int Card::GLOBAL_ID = 0;

        Card::Card(Suit suit, int number, bool targetFixed): suit_(suit), id_(GLOBAL_ID++), number_(number), targetFixed_(targetFixed)
        {

        }


        Card::~Card(void)
        {
        }

        string Card::getSuitString() {
            switch(suit_){
            case Spade: return "spade";
            case Heart: return "heart";
            case Club: return "club";
            case Diamond: return "diamond";
            default: return "no_suit";
            }
        }

        Card::Color Card::getColor() {
            if(suit_ == Spade || Spade == Heart ) {
                return Card::Red;
            }
            else {
                return Card::Black;
            }
        }

        bool Card::isVirtualCard() const {
            return id_ < 0;
        }

        int Card::getEffectiveId() const {
            if(isVirtualCard()){
                if(subcards_.empty())
                    return -1;
                else
                    return subcards_.front();
            }else
                return id_;
        }

        void Card::addSubcard(int cardId) {
            if(cardId > 0 )
                subcards_.push_back(cardId);
        }

        void Card::addSubcard(const boost::shared_ptr<Card> card) {
            addSubcard(card->getEffectiveId());
        }

        vector<int> Card::getSubcards() const {
            return subcards_;
        }

        void Card::clearSubcards() {
            subcards_.clear();
        }

        int Card::getSubcardsCount() const {
            return subcards_.size();
        }

        bool Card::isNDTrick() const{
            return false;
        }

        /*
        bool Card::targetsFeasible(const vector<const  boost::shared_ptr<Player> > &targets, const  boost::shared_ptr<Player> self) const{
            if (targetFixed_)
                return true;
            else
                return !targets.empty();
        }

        
        bool Card::targetFilter(const vector<const  boost::shared_ptr<Player> > &targets, const  boost::shared_ptr<Player> to_select, const  boost::shared_ptr<Player> self) const{
            return targets.empty() && to_select.get() != self.get();
        }*/

        bool Card::isAvailable(const boost::shared_ptr<Player> player) const{
            if(limitUseTimes_) {
                string useMark = "use_" + name_;
                int count = player->getMark(useMark);
                if(count > useTimesLimit_) {
                    return false;
                }
            }
            
            return true;
        }

        void Card::onUse(boost::shared_ptr<Room> room, const CardUseStruct &use) const{
            CardUseStruct card_use = use;
            boost::shared_ptr<ServerPlayer> player = card_use.from;
            /*
            if(card_use.from && card_use.to.size() > 1){
            qSort(card_use.to.begin(), card_use.to.end(), ServerPlayer::CompareByActionOrder);
            }

            LogMessage log;
            log.from = player;
            log.to = card_use.to;
            log.type = "#UseCard";
            log.card_str = toString();
            room->sendLog(log);

            vector<int> used_cards;
            vector<CardsMoveStruct> moves;
            if(card_use.card->isVirtualCard())
            used_cards.append(card_use.card->getSubcards());
            else used_cards << card_use.card->getEffectiveId();

            QVariant data = QVariant::fromValue(card_use);
            RoomThread *thread = room->getThread();

            if(getTypeId() != Card::Skill){
            CardMoveReason reason(CardMoveReason::S_REASON_USE, player->objectName(), QString(), this->getSkillName(), QString());
            if (card_use.to.size() == 1)
            reason.m_targetId = card_use.to.first()->objectName();
            CardsMoveStruct move(used_cards, card_use.from, NULL, Player::PlaceTable, reason);
            moves.append(move);
            room->moveCardsAtomic(moves, true);
            }
            else if(willThrow()){
            CardMoveReason reason(CardMoveReason::S_REASON_THROW, player->objectName(), QString(), this->getSkillName(), QString());
            room->moveCardTo(this, player, NULL, Player::DiscardPile, reason, true);
            }

            thread->trigger(CardUsed, room, player, data);

            thread->trigger(CardFinished, room, player, data);
            */
        }

        void Card::use(boost::shared_ptr<Room> room, boost::shared_ptr<ServerPlayer> source, vector<boost::shared_ptr<ServerPlayer> > &targets) const{
            /*
            if(targets.size() == 1){
            room->cardEffect(this, source, targets.first());
            }else{
            vector<ServerPlayer *> players = targets;
            if(room->getMode() == "06_3v3"){
            if(isKindOf("AOE") || isKindOf("GlobalEffect"))
            room->reverseFor3v3(this, source, players);
            }

            foreach(ServerPlayer *target, players){
            CardEffectStruct effect;
            effect.card = this;
            effect.from = source;
            effect.to = target;
            effect.multiple = true;

            room->cardEffect(effect);
            }
            }

            if(room->getCardPlace(getEffectiveId()) == Player::PlaceTable){
            CardMoveReason reason(CardMoveReason::S_REASON_USE, source->objectName(), QString(), this->getSkillName(), QString());
            if (targets.size() == 1) reason.m_targetId = targets.first()->objectName();
            room->moveCardTo(this, source, NULL, Player::DiscardPile, reason, true);
            CardUseStruct card_use;
            card_use.card = this;
            card_use.from = source;
            card_use.to = targets;
            QVariant data = QVariant::fromValue(card_use);
            room->getThread()->trigger(PostCardEffected, room, source, data);
            }
            */
        }

        void Card::onEffect(const CardEffectStruct &) const{

        }
    };
};