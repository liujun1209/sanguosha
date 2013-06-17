#include "Player.h"
#include <boost\foreach.hpp>
#include "MyLock.h"

using namespace sgs::utils;

namespace sgs {
    namespace core {

        Player::Player(void): phase_(PhaseNone)
        {
        }


        Player::~Player(void)
        {
        }

        void Player::setGeneral(boost::shared_ptr<General> general) {
            general_ = general;
            int hp = general->getMaxHp();
            if(getRole() == Lord) {
                hp ++;//主公血+1
            }
            setHp(hp);
            setMaxHp(hp);
        }

        boost::shared_ptr<General> Player::getGeneral() {
            return general_;
        }

        int Player::getHandCardNum() {
            return handCards_.size();
        }

        //todo
        int Player::getAttackRange() {
            int range = 1;
            return range;
        }
        
        void Player::drawCards(vector<int> cardIds) {
            WriteLock lock(cardMutex_);
            BOOST_FOREACH(int id, cardIds) {
                handCards_.push_back(id);
            }
        }

        void Player::discardCards(const set<int>& discardCards) {
            WriteLock lock(cardMutex_);
            vector<int>::iterator it;
            for(it = handCards_.begin(); it != handCards_.end(); ) {
                if(discardCards.find(*it) != discardCards.end()) {
                    it = handCards_.erase(it);
                }else {
                    it++;
                }
            }
        }
        //bool Player::hasFlag(const string &flag) const {
        //    return flags_.find(flag) != flags_.end();
        //}

        /**
        * @brief 用于游戏过程中获取技能，觉醒技都是这样获得的
        *        如志继获得观星，魂姿获得英魂英姿
        **/
        void Player::acquireSkill(const string &skill_name) {
        }
        /**
        * @brief 用于游戏过程中失去技能
        **/
        void Player::loseSkill(const string &skill_name) {
        }
        bool Player::hasSkill(const string &skill_name) const {
            return false;
        }
        bool Player::hasInnateSkill(const string &skill_name) const {
            return false;
        }
        /**
        * @brief 判断是否有某主公技能
        **/
        bool Player::hasLordSkill(const string &skill_name, bool include_lose) const {
            return false;
        }

        /**
        * @brief 穿装备
        **/
        void Player::setEquip(boost::shared_ptr<Card> equip) {
        
        }
        /**
        * @brief 卸装备
        **/
        void Player::removeEquip(boost::shared_ptr<Card> equip) {
        }
        /**
        * @brief 是否有某武器
        **/
        bool Player::hasEquip(const boost::shared_ptr<Card> card) const {
            return false;
        }
        /**
        * @brief 是否有装备
        **/
        bool Player::hasEquip() const {
            return false;
        }
        /**
        * @brief 是否有指定武器
        **/
        bool Player::hasWeapon(const string &weapon_name) const {
            return false;
        }
        /**
        * @brief 是否可以发动防具技能
        **/
        bool Player::hasArmorEffect(const string &armor_name) const {
            return false;
        }

        //判定区方面
        /**
        * @brief 获取判定区的所有牌
        **/
        vector<boost::shared_ptr<Card> > Player::getJudgingArea() const {
            return vector<boost::shared_ptr<Card> >();
        }
        /**
        * @brief 贴上延迟锦囊
        **/
        void Player::addDelayedTrick(const boost::shared_ptr<Card> trick) {
        }
        /**
        * @brief 去掉延迟锦囊
        **/
        void Player::removeDelayedTrick(const boost::shared_ptr<Card> trick) {
        }
        /**
        * @brief 查看判定区是否有指定延迟锦囊
        **/
        bool Player::containsTrick(const string &trick_name) const {
            return false;
        }

        //标记物方面
        /**
        * @brief 添加标记
        **/
        void Player::addMark(const string &mark) {
            marks_[mark]++;
        }
        /**
        * @brief 去除标记
        **/
        void Player::removeMark(const string &mark) {
            if(marks_.find(mark) != marks_.end()) {
                marks_[mark]--;
            }
        }
        /**
        * @brief 设置标记
        **/
        void Player::setMark(const string &mark, int value) {
            marks_[mark] = value;
        }
        /**
        * @brief 获取某标记
        **/
        int Player::getMark(const string &mark) const {
            map<string, int>::const_iterator it = marks_.find(mark);
            if(it != marks_.end()) {
                return it->second;
            }
            return 0;
        }
        /**
        * @brief 获取特殊牌堆的牌，如七星、屯田、不屈
        **/
        vector<int> Player::getPile(const string &pile_name) const {
            return vector<int>();
        }
        //技能使用方面
        /**
        * @brief 是否用过某牌，主要用于每回合只能发动一次的技能，判断技能是否发动过
        **/
        bool Player::hasUsed(const string &card_class) const {
            return false;
        }
        /**
        * @brief 用于回合内可多次使用但是次数限制的技能
        **/
        int Player::usedTimes(const string &card_class) const {
            return 0;
        }
        //卡牌使用方面
        /**
        * @brief 禁止技就是通过它实现的
        **/
        bool Player::isProhibited(const boost::shared_ptr<Player> to, const boost::shared_ptr<Card> card) const {
            return false;
        }

        /**
        * @brief 是否可以连续出杀
        **/
        bool Player::canSlashWithoutCrossbow() const {
            return false;
        }

        bool Player::isLastHandCard(const boost::shared_ptr<Card> card) const {
            return false;
        }


        bool Player::hasCanAttackTarget() {
            int attackRange = 1;
            
            map<const boost::shared_ptr<Player> , int>::const_iterator it;
            for(it = fixedDistance_.begin(); it!= fixedDistance_.end(); it++) {
                if(attackRange >= it->second) {
                    return true;
                }
            }

            return false;
        }

    };
};