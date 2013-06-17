#ifndef __SKILL_H__
#define __SKILL_H__

#include <cocos2d.h>
#include <string>
#include <boost\smart_ptr.hpp>
#include "Macros.h"
#include "Structs.h"
namespace sgs{
namespace server {
    class Room;
    class ServerPlayer;
}
}
using namespace sgs::server;
namespace sgs{
namespace core {
    class Card;
    class Player;
}
}
using namespace sgs::core;

using namespace std;
using namespace cocos2d;


namespace sgs {
namespace core {

    class General;
    /**
    * @brief 技能类
    **/
    class Skill
    {
    private:
        static CCDictionary * SKILL_STRINGS;
    public:
       
        enum Frequency{
            Frequent,//频繁使用技
            NotFrequent,//非频繁使用技
            Compulsory,//锁定技
            Limited,//限定技
            Wake//觉醒技能
        };
        //默认技能为非频繁使用技能，比如英姿、集智、连营等等。
        //非频繁使用的技能就是剩下的，一些主动使用，仁德，一些被动询问是否发动


        enum Location{
            Left,
            Right
        };
        //频率
        CC_SYNTHESIZE_PASS_BY_REF(Frequency, frequency_, Frequency);
        //名字
        CC_SYNTHESIZE_PASS_BY_REF(string, name_, Name);
        //所有者
        CC_SYNTHESIZE_PASS_BY_REF(boost::shared_ptr<General>, parent_, Parent);
        //是否可见
        CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(visible_, Visible);
        //是否为主公技能
        CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(lordSkill_, LordSkill);
        
        Skill(const string &name, Frequency frequent = NotFrequent);
        ~Skill(void);

        string getSkillString(const string& key) const;
        /**
        * @brief 获取技能描述
        **/
        string getDescription() const;
        /**
        * @brief 限定技、锁定技、觉醒技前面会有【xx技】的字样，就是这个函数得到
        **/
        string getText() const;

        string getChineseName() const;
        /**
        * @brief 获取默认选择，这个应该是在有出牌时限的情况下，对于一些需要选择发动的技能，如果玩家没有选择，系统自动分那会默认选项。
        *   如董卓的崩坏(上限>=血量+2就掉上线，否则掉血）
        **/
        virtual string getDefaultChoice(boost::shared_ptr<ServerPlayer> player) const;
        /**
        * @brief 增加标记
        **/
        void setFlag(boost::shared_ptr<ServerPlayer> player) const;
        /**
        * @brief 减少标记
        **/
        void unsetFlag(boost::shared_ptr<ServerPlayer> player) const;
    protected:
        string defaultChoice_;

    };
    /**
    * @brief 视作技，例如乱击，双雄、倾国、天香、武神，可以将某些牌当作其他牌使用
    *   这个其他牌可以是基本牌、锦囊牌、也可以使技能牌（如离间）
    **/
    class ViewAsSkill:public Skill{
    public:
        ViewAsSkill(const string &name);

        /**
        * @brief 用于发动技能时过滤掉不能使用的卡牌
        *        如倾国会过滤掉装备牌和红色手牌、青囊会过滤掉黑色装备手牌
        **/
        virtual bool viewFilter(const vector<const boost::shared_ptr<Card> > &selected, const Card *to_select) const = 0;
        /**
        * @brief 用于在发动技能时将牌视作转化后的卡牌
        *        如倾国会视为闪
        **/
        virtual const boost::shared_ptr<Card> viewAs(const vector<const boost::shared_ptr<Card> > &cards) const = 0;
        /**
        * @brief 用于判断技能是否可以使用，如果返回false, 技能按钮被禁用
        * @return bool
        **/
        bool isAvailable(CardUseStruct::CardUseReason reason, const string &pattern) const;
        /**
        * @brief 判断技能是否可以在出牌阶段无其他事件激励的条件下是否可以发动
        *        如在没有判定的时候，司马的鬼才是被禁用的
        **/
        virtual bool isEnabledAtPlay(const boost::shared_ptr<Player> player) const;
        /**
        * @brief 一些技能只有在特定条件下才能被激活
        *        比如被杀时的倾国和龙胆、使用锦囊时的看破。
        *        这些条件由参数pattern给出，当条件符合时，这些技能会被激活
        **/
        virtual bool isEnabledAtResponse(const boost::shared_ptr<Player> player, const string &pattern) const;
        //virtual bool isEnabledAtNullification(const boost::shared_ptr<ServerPlayer> player) const;
        
        static const ViewAsSkill* parseViewAsSkill(const Skill* skill);
    };

    /**
    * @brief 不需要牌可以发动的技能。
    *        突袭、激将、反间、苦肉、雷击、放逐、英魂。。。
    **/
    class ZeroCardViewAsSkill: public ViewAsSkill{
    public:
        ZeroCardViewAsSkill(const string &name);
        /**
        * @brief 用于发动技能时过滤掉不能使用的卡牌
        *        如倾国会过滤掉装备牌和红色手牌、青囊会过滤掉黑色装备手牌
        **/
        virtual bool viewFilter(const vector<const boost::shared_ptr<Card> > &selected, const boost::shared_ptr<Card> to_select) const;
        /**
        * @brief 用于在发动技能时将牌视作转化后的卡牌
        *        如倾国会视为闪
        **/
        virtual const boost::shared_ptr<Card> viewAs(const vector<const boost::shared_ptr<Card> > &cards) const;

        virtual const boost::shared_ptr<Card> viewAs() const = 0;
    };

    /**
    * @brief 使用一张牌可以发动的技能
    *        例如鬼才、倾国、武圣、龙胆等
    **/
    class OneCardViewAsSkill: public ViewAsSkill{
    public:
        OneCardViewAsSkill(const string &name);
        /**
        * @brief 用于发动技能时过滤掉不能使用的卡牌
        *        如倾国会过滤掉装备牌和红色手牌、青囊会过滤掉黑色装备手牌
        **/
        virtual bool viewFilter(const vector<const boost::shared_ptr<Card> > &selected, const boost::shared_ptr<Card> to_select) const;
        /**
        * @brief 用于在发动技能时将牌视作转化后的卡牌
        *        如倾国会视为闪
        **/
        virtual const boost::shared_ptr<Card> viewAs(const vector<const Card *> &cards) const;

        virtual bool viewFilter(const boost::shared_ptr<Card> to_select) const = 0;
        virtual const boost::shared_ptr<Card> viewAs(const boost::shared_ptr<Card> originalCard) const = 0;
    };

    /**
    * @brief 用于“视为”锁定技
    *        红颜、戒酒、武神
    **/
    class FilterSkill: public OneCardViewAsSkill{
    public:
        FilterSkill(const string &name);
    };

    /**
    * @brief 触发技，通过某一事件才能使用的技能
    *        如：刚烈、反馈、铁骑等
    **/
    class TriggerSkill:public Skill{
    public:
        /**
        * @brief 构造函数
        **/
        TriggerSkill(const string &name);
        const boost::shared_ptr<ViewAsSkill> getViewAsSkill() const;
        
        /**
        * @brief 获取时间优先级
        **/
        virtual int getPriority() const;
        virtual bool triggerable(const boost::shared_ptr<ServerPlayer> target) const;
    protected:
        const boost::shared_ptr<ViewAsSkill> view_as_skill;
    };

    /**
    * @brief 卖血技
    *        如：奸雄、遗技、刚烈、反馈、放逐、节命、新生、归心
    **/
    class MasochismSkill: public TriggerSkill{
    public:
        /**
        * @brief 构造函数
        **/
        MasochismSkill(const string &name);

        virtual int getPriority() const;
        /**
        * @brief 主题代码
        **/
        virtual void onDamaged(boost::shared_ptr<ServerPlayer> target, const DamageStruct &damage) const = 0;
    };

    /**
    * @brief 阶段转换时发动的技能
    *        如：仁德、突袭、观星、克己跳弃牌、闭月
    **/
    class PhaseChangeSkill: public TriggerSkill{
    public:
        PhaseChangeSkill(const string &name);

        virtual bool onPhaseChange(boost::shared_ptr<ServerPlayer> target) const =0;
    };
    /**
    * @brief 有改变摸牌阶段摸牌数量的技能
    *        如：裸衣、英姿、好施等
    **/
    class DrawCardsSkill: public TriggerSkill{
    public:
        DrawCardsSkill(const string &name);

        virtual int getDrawNum(boost::shared_ptr<ServerPlayer>  player, int n) const = 0;
    };

    /**
    * @brief 杀的辅助技能
    *        如：铁骑和烈弓
    **/
    class SlashBuffSkill: public TriggerSkill{
    public:
        SlashBuffSkill(const string &name);

        virtual bool buff(const SlashEffectStruct &effect) const = 0;
    };

    /**
    * @brief 游戏开始就可使用的技能
    *        如：张角主，群角色获得黄天技能
    **/
    class GameStartSkill: public TriggerSkill{
    public:
        GameStartSkill(const string &name);

        virtual void onGameStart(boost::shared_ptr<ServerPlayer> player) const = 0;
    };

    /**
    * @brief 禁用技能，表示拥有该类技能的武将不可成为某些牌的目标
    *        如：空城、谦逊、帷幕（注意不能成为目标和卡片对你无效是两个概念）
    **/
    class ProhibitSkill: public Skill{
    public:
        ProhibitSkill(const string &name);

        /**
        * @brief 过滤掉被禁用的牌
        **/
        virtual bool isProhibited(const boost::shared_ptr<Player> from, const boost::shared_ptr<Player>  to, const boost::shared_ptr<Card> card) const = 0;
    };

    /**
    * @brief 距离技能，拥有此类技能的武将在计算距离的时候需要进行修正
    *        如：马术、屯田、单骑
    **/
    class DistanceSkill: public Skill{
    public:
        DistanceSkill(const string &name);
        /**
        * @brief 修正距离
        **/
        virtual int getCorrect(const boost::shared_ptr<Player> from, const boost::shared_ptr<Player> to) const = 0;
    };

    /**
    * @brief 武器的技能
    **/
    class WeaponSkill: public TriggerSkill{
    public:
        WeaponSkill(const string &name);

        virtual bool triggerable(const boost::shared_ptr<ServerPlayer> target) const;
    };

    /**
    * @brief 防具的技能
    **/
    class ArmorSkill: public TriggerSkill{
    public:
        ArmorSkill(const string &name);

        virtual bool triggerable(const boost::shared_ptr<ServerPlayer>target) const;
    };

    /**
    * @brief 继承于GameStartSkill ，用于在游戏开始给吴疆标记，例如乱舞，用来标记限定技能是否使用过
    **/
    class MarkAssignSkill: public GameStartSkill{
    public:
        MarkAssignSkill(const string &mark, int n);

        virtual void onGameStart(ServerPlayer *player) const;

    private:
        string markName_;
        int n;
    };

};
};


#endif