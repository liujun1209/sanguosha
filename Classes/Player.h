#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <cocos2d.h>
#include <boost\smart_ptr.hpp>
#include <boost\thread.hpp>
#include <string>
#include "Macros.h"
#include "General.h"

using namespace std;
using namespace boost;
using namespace cocos2d;

namespace sgs {
    namespace core {
        class Card;
        /**
        * @brief 玩家类
        **/
        class Player
        {
        public:
            //游戏阶段：回合开始、开始阶段、判定阶段、摸牌阶段、出牌阶段、弃牌阶段、结束阶段、回合外、
            enum Phase {RoundStart, Start, Judge, Draw, Play, Discard, Finish, NotActive, PhaseNone};
            //卡牌位置：手牌区、装配区、
            enum Place { PlaceHand, PlaceEquip, PlaceDelayedTrick, PlaceJudge,
                PlaceSpecial, DiscardPile, DrawPile, PlaceTable, PlaceUnknown,
                PlaceWuGu};
            //游戏角色：主忠反内
            enum Role {Lord, Loyalist, Rebel, Renegade, Unknow};

            Player(void);
            ~Player(void);
            //id
            CC_SYNTHESIZE(int, id_, Id);
            //玩家名字
            CC_SYNTHESIZE_PASS_BY_REF(string, name_, Name);
            
            CC_SYNTHESIZE_PASS_BY_REF(string, head_, Head);
            //玩家指针，是一个循环列表
            CC_SYNTHESIZE_PASS_BY_REF(boost::shared_ptr<Player>, nextPlayer_, NextPlayer);
            CC_SYNTHESIZE_PASS_BY_REF(boost::shared_ptr<Player>, nextLivePlayer_, NextLivePlayer);
            CC_SYNTHESIZE_PASS_BY_REF(boost::shared_ptr<Player>, prePlayer_, PrePlayer);
            CC_SYNTHESIZE_PASS_BY_REF(boost::shared_ptr<Player>, preLivePlayer_, PreLivePlayer);
            //血量
            CC_SYNTHESIZE(int, hp_, Hp);
            //血量上限
            CC_SYNTHESIZE(int, maxHp_, MaxHp);
            //国家
            CC_SYNTHESIZE_PASS_BY_REF(string, kingdom_, Kingdom);
            //手牌数
            //CC_SYNTHESIZE_READONLY(int, handcardNum_, HandCardNum);
            //座位号
            CC_SYNTHESIZE(int, seat_, Seat);
            //是否被翻面
            CC_SYNTHESIZE_READONLY_FOR_BOOL(faceup_, Faceup);
            //是否活着
            CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(alive_, Alive);
            //是否被横置
            CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(chained_, Chained);
            //攻击范围
            CC_PROPERTY_READONLY(int, attackRange_, AttackRange);
            //owner
            CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(owner_, Owner);
            //ready
            CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(ready_, Ready);

            //武将
            CC_PROPERTY(boost::shared_ptr<General>, general_, General);
            //角色
            CC_SYNTHESIZE(Role, role_, Role);
            //Place
            CC_SYNTHESIZE(Place, place_, Place);
            //阶段
            CC_SYNTHESIZE(Phase, phase_, Phase);
            //武器
            CC_SYNTHESIZE(boost::shared_ptr<Card>, weapon_, Weapon);
            //防具
            CC_SYNTHESIZE(boost::shared_ptr<Card>, armor_, Armor);
            //攻击马
            CC_SYNTHESIZE(boost::shared_ptr<Card>, defensiveHorse_, DefensiveHorse);
            //防御马
            CC_SYNTHESIZE(boost::shared_ptr<Card>, offensiveHorse_, OffensiveHorse);

            //bool hasFlag(const string &flag) const;
            //void clearFlags();

            /**
            * @brief 获取手牌数量
            **/
            virtual int getHandCardNum();

            //抓牌
            virtual void drawCards(vector<int> cardIds);

            //弃牌
            virtual void discardCards(const set<int>& discardCards);

            /**
            * @brief 用于游戏过程中获取技能，觉醒技都是这样获得的
            *        如志继获得观星，魂姿获得英魂英姿
            **/
            void acquireSkill(const string &skill_name);
            /**
            * @brief 用于游戏过程中失去技能
            **/
            virtual void loseSkill(const string &skill_name);
            bool hasSkill(const string &skill_name) const;
            bool hasInnateSkill(const string &skill_name) const;
            /**
            * @brief 判断是否有某主公技能
            **/
            bool hasLordSkill(const string &skill_name, bool include_lose = false) const;

            /**
            * @brief 穿装备
            **/
            void setEquip(boost::shared_ptr<Card> equip);
            /**
            * @brief 卸装备
            **/
            void removeEquip(boost::shared_ptr<Card> equip);
            /**
            * @brief 是否有某武器
            **/
            bool hasEquip(const boost::shared_ptr<Card> card) const;
            /**
            * @brief 是否有装备
            **/
            bool hasEquip() const;
            /**
            * @brief 是否有指定武器
            **/
            bool hasWeapon(const string &weapon_name) const;
            /**
            * @brief 是否可以发动防具技能
            **/
            bool hasArmorEffect(const string &armor_name) const;

            //判定区方面
            /**
            * @brief 获取判定区的所有牌
            **/
            vector<boost::shared_ptr<Card> > getJudgingArea() const;
            /**
            * @brief 贴上延迟锦囊
            **/
            void addDelayedTrick(const boost::shared_ptr<Card> trick);
            /**
            * @brief 去掉延迟锦囊
            **/
            void removeDelayedTrick(const boost::shared_ptr<Card> trick);
            /**
            * @brief 查看判定区是否有指定延迟锦囊
            **/
            bool containsTrick(const string &trick_name) const;

            //标记物方面
            /**
            * @brief 添加标记
            **/
            void addMark(const string &mark);
            /**
            * @brief 去除标记
            **/
            void removeMark(const string &mark);
            /**
            * @brief 设置标记
            **/
            virtual void setMark(const string &mark, int value);
            /**
            * @brief 获取某标记
            **/
            int getMark(const string &mark) const;
            /**
            * @brief 获取特殊牌堆的牌，如七星、屯田、不屈
            **/
            vector<int> getPile(const string &pile_name) const;
            //技能使用方面
            /**
            * @brief 是否用过某牌，主要用于每回合只能发动一次的技能，判断技能是否发动过
            **/
            bool hasUsed(const string &card_class) const;
            /**
            * @brief 用于回合内可多次使用但是次数限制的技能
            **/
            int usedTimes(const string &card_class) const;
            //卡牌使用方面
            /**
            * @brief 禁止技就是通过它实现的
            **/
            virtual bool isProhibited(const boost::shared_ptr<Player> to, const boost::shared_ptr<Card> card) const;
            /**
            * @brief 是否是最后一张手牌，用于方天画戟三杀的发动
            **/
            virtual bool isLastHandCard(const boost::shared_ptr<Card> card) const;// = 0;
            /**
            * @brief 是否可以连续出杀
            **/
            bool canSlashWithoutCrossbow() const;

            /**
            * @brief 是否有可攻击的对象
            **/
            bool hasCanAttackTarget();
        protected:
            //标记物
            map<string, int> marks_;
            //特殊牌堆
            map<string, vector<int> > piles_;
            //游戏中获得技能
            set<string> acquiredSkills_;
            vector<string> skills_;
            //标志flag
            set<string> flags_;
            //map<string, int> history_;
            vector<int> handCards_;
            
            boost::shared_mutex cardMutex_;
        private:
            //判定区
            vector<int> judgingArea_;
            //其他玩家的距离
            map<const boost::shared_ptr<Player> , int> fixedDistance_;

        };

    };
};

#endif
