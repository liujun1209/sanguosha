#ifndef __STANDARD_H__
#define __STANDARD_H__

#include <string>
#include <boost\enable_shared_from_this.hpp>

namespace sgs {
namespace server {
    class Room;
}
}
#include "Package.h"
#include "Card.h"
#include "Structs.h"
#include "Skill.h"

using namespace std;
using namespace sgs::core;
using namespace sgs::server;

namespace sgs {
namespace package {

    class StandardPackage : public Package{
    public:
        StandardPackage();
        virtual bool init();
        virtual void addGenerals();
        virtual void addCards();
    protected:
        map<string, const boost::shared_ptr<CardPattern> > patterns;
    };

    //基本卡
    class BasicCard : public Card {
    public:
        BasicCard(Suit suit, int number):Card(suit, number){}
        virtual string getType() const;
        virtual CardType getTypeId() const;
    };

    //杀
    class Slash: public BasicCard{
    public:
        Slash(Card::Suit suit, int number);
        DamageStruct::Nature getNature() const;
        void setNature(DamageStruct::Nature nature);

        virtual string getSubtype() const;
        virtual void onUse(boost::shared_ptr<Room> room, const CardUseStruct &card_use) const;
        virtual void use(boost::shared_ptr<Room> room, boost::shared_ptr<ServerPlayer> source, vector<boost::shared_ptr<ServerPlayer> > &targets) const;
        virtual void onEffect(const CardEffectStruct &effect) const;

        virtual bool targetsFeasible(const vector<const boost::shared_ptr<Player> > &targets, const boost::shared_ptr<Player> Self) const;
        virtual bool targetFilter(const vector<const boost::shared_ptr<Player> > &targets, const boost::shared_ptr<Player> to_select, const boost::shared_ptr<Player> Self) const;
        virtual bool isAvailable(const boost::shared_ptr<Player> player) const;

        static bool IsAvailable(const boost::shared_ptr<Player> player);

    protected:
        DamageStruct::Nature nature;
    };
    /*
    //锦囊卡
    class TrickCard:public Card {
    public:
        TrickCard(Suit suit, int number, bool aggressive);
        bool isAggressive() const;
        void setCancelable(bool cancelable);

        virtual string getType() const;
        virtual CardType getTypeId() const;
        virtual bool isCancelable(const CardEffectStruct &effect) const;

    private:
        //攻击性的
        bool aggressive;
        //可取消的
        bool cancelable;
    };

    //装备
    class EquipCard : public Card{
    public:
        enum Location {
            WeaponLocation = 0,
            ArmorLocation = 1,
            DefensiveHorseLocation = 2,
            OffensiveHorseLocation = 3
        };

        EquipCard(Suit suit, int number):Card(suit, number, true) {}
        boost::shared_ptr<TriggerSkill> getSkill() const;

        virtual string getType() const;
        virtual CardType getTypeId() const;

        virtual void onUse(boost::shared_ptr<Room> room, const CardUseStruct &card_use) const;
        virtual void use(boost::shared_ptr<Room> room, boost::shared_ptr<ServerPlayer> source, vector<boost::shared_ptr<ServerPlayer> > &targets) const;

        virtual void onInstall(boost::shared_ptr<ServerPlayer> player) const;
        virtual void onUninstall(boost::shared_ptr<ServerPlayer> player) const;

        virtual Location location() const = 0;
        virtual string label() const = 0;

    protected:
        boost::shared_ptr<TriggerSkill> skill;
    };

    //武器
    class Weapon:public EquipCard{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(EquipCard)
        Weapon(Suit suit, int number, int range);
        int getRange() const;

        virtual string getSubtype() const;

        virtual Location location() const;
        virtual string label() const;
        virtual string getCommonEffectName() const;

    protected:
        int range;
    };

    //防具
    class Armor : public EquipCard{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(EquipCard)
        Armor(Suit suit, int number):EquipCard(suit, number), name_("Armor"){}
        virtual string getSubtype() const;

        virtual Location location() const;
        virtual string label() const;
        virtual string getCommonEffectName() const;
    };
    //马
    class Horse:public EquipCard{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(EquipCard)
        Horse(Suit suit, int number, int correct);
        int getCorrect() const;

        virtual Location location() const;
        virtual void onInstall(boost::shared_ptr<ServerPlayer> player) const;
        virtual void onUninstall(boost::shared_ptr<ServerPlayer> player) const;

        virtual string label() const;
        virtual string getCommonEffectName() const;

    private:
        int correct;
    };
    //攻击马
    class OffensiveHorse: public Horse{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(Horse)
        OffensiveHorse(Card::Suit suit, int number, int correct = -1);
        virtual string getSubtype() const;
    };
    //防御马
    class DefensiveHorse: public Horse{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(Horse)
        DefensiveHorse(Card::Suit suit, int number, int correct = +1);
        virtual string getSubtype() const;
    };
    //全局效果
    class GlobalEffect:public TrickCard{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(TrickCard)
        GlobalEffect(Card::Suit suit, int number):TrickCard(suit, number, false), name_("GlobalEffect"){ targetFixed_ = true;}
        virtual string getSubtype() const;
        virtual void onUse(boost::shared_ptr<Room> room, const CardUseStruct &card_use) const;
    };
    //桃园结义
    class GodSalvation:public GlobalEffect{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(GlobalEffect)
        GodSalvation(Card::Suit suit = Heart, int number = 1);
        virtual bool isCancelable(const CardEffectStruct &effect) const;
        virtual void onEffect(const CardEffectStruct &effect) const;
    };
    //五谷丰登
    class AmazingGrace:public GlobalEffect{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(GlobalEffect)
        AmazingGrace(Card::Suit suit, int number);
        virtual void doPreAction(boost::shared_ptr<Room> room, const CardUseStruct &card_use) const;
        virtual void use(boost::shared_ptr<Room> room, boost::shared_ptr<ServerPlayer> source, vector<boost::shared_ptr<ServerPlayer> > &) const;
        virtual void onEffect(const CardEffectStruct &effect) const;
    };
    //群攻锦囊
    class AOE:public TrickCard{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(TrickCard)
        AOE(Suit suit, int number):TrickCard(suit, number, true), name_("AOE"){ targetFixed_ = true;}
        virtual string getSubtype() const;
        virtual bool isAvailable(const boost::shared_ptr<Player> player) const;
        virtual void onUse(boost::shared_ptr<Room> room, const CardUseStruct &card_use) const;
    };
    //南蛮入侵
    class SavageAssault:public AOE{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(AOE)
        SavageAssault(Card::Suit suit, int number);
        virtual void onEffect(const CardEffectStruct &effect) const;
    };
    //万箭齐发
    class ArcheryAttack:public AOE{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(AOE)
        ArcheryAttack(Card::Suit suit = Heart, int number = 1);
        virtual void onEffect(const CardEffectStruct &effect) const;
    };
    //单对象锦囊
    class SingleTargetTrick: public TrickCard{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(TrickCard)
        SingleTargetTrick(Suit suit, int number, bool aggressive):TrickCard(suit, number, aggressive){}
        virtual string getSubtype() const;

        virtual bool targetFilter(const vector<const boost::shared_ptr<Player> > &targets, const boost::shared_ptr<Player> to_select, const boost::shared_ptr<Player> Self) const;
        virtual void use(boost::shared_ptr<Room> room, boost::shared_ptr<ServerPlayer> source, vector<boost::shared_ptr<ServerPlayer> > &targets) const;
    };
    //借刀杀人
    class Collateral:public SingleTargetTrick{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(SingleTargetTrick)
        Collateral(Card::Suit suit, int number);
        virtual bool isAvailable(const boost::shared_ptr<Player> player) const;
        virtual bool targetsFeasible(const vector<const boost::shared_ptr<Player> > &targets, const boost::shared_ptr<Player> Self) const;
        virtual bool targetFilter(const vector<const boost::shared_ptr<Player> > &targets, const boost::shared_ptr<Player> to_select, const boost::shared_ptr<Player> Self) const;
        virtual void onUse(boost::shared_ptr<Room> room, const CardUseStruct &card_use) const;
        virtual void onEffect(const CardEffectStruct &effect) const;

    private:
        bool doCollateral(boost::shared_ptr<Room> room, boost::shared_ptr<ServerPlayer> killer, boost::shared_ptr<ServerPlayer> victim, const string &prompt) const;
    };
    //无中生有
    class ExNihilo: public SingleTargetTrick{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(SingleTargetTrick)
        ExNihilo(Card::Suit suit, int number);
        virtual void onEffect(const CardEffectStruct &effect) const;
    };
    //决斗
    class Duel:public SingleTargetTrick{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(SingleTargetTrick)
        Duel(Card::Suit suit, int number);
        virtual bool targetFilter(const vector<const boost::shared_ptr<Player> > &targets, const boost::shared_ptr<Player> to_select, const boost::shared_ptr<Player> Self) const;
        virtual void onEffect(const CardEffectStruct &effect) const;
    };
    //延时性锦囊
    class DelayedTrick:public TrickCard{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(TrickCard)
        DelayedTrick(Suit suit, int number, bool movable = false);
        virtual void onNullified(boost::shared_ptr<ServerPlayer> target) const;

        virtual void onUse(boost::shared_ptr<Room> room, const CardUseStruct &card_use) const;
        virtual void use(boost::shared_ptr<Room> room, boost::shared_ptr<ServerPlayer> source, vector<boost::shared_ptr<ServerPlayer> > &targets) const;
        virtual string getSubtype() const;
        virtual void onEffect(const CardEffectStruct &effect) const;
        virtual void takeEffect(boost::shared_ptr<ServerPlayer> target) const = 0;
        virtual bool isNDTrick() const;
    protected:
        JudgeStruct judge;

    private:
        bool movable;
    };
    //乐不思蜀
    class Indulgence:public DelayedTrick{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(DelayedTrick)
        Indulgence(Card::Suit suit, int number);

        virtual bool targetFilter(const vector<const boost::shared_ptr<Player> > &targets, const boost::shared_ptr<Player> to_select, const boost::shared_ptr<Player> Self) const;
        virtual void takeEffect(boost::shared_ptr<ServerPlayer> target) const;
    };
    //灾难
    class Disaster: public DelayedTrick{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(DelayedTrick)
        Disaster(Card::Suit suit, int number);

        virtual bool isAvailable(const boost::shared_ptr<Player> player) const;
    };
    //闪电
    class Lightning: public Disaster{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(Disaster)
        Lightning(Card::Suit suit, int number);
        virtual void takeEffect(boost::shared_ptr<ServerPlayer> target) const;
    };
    //无懈可击
    class Nullification:public SingleTargetTrick{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(SingleTargetTrick)
        Nullification(Card::Suit suit, int number);

        virtual void use(boost::shared_ptr<Room> room, boost::shared_ptr<ServerPlayer> source, vector<boost::shared_ptr<ServerPlayer> > &targets) const;
        virtual bool isAvailable(const boost::shared_ptr<Player> player) const;
    };
    
    // cards of standard package
    
    //闪
    class Jink: public BasicCard{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(BasicCard)
        Jink(Card::Suit suit, int number);
        virtual string getSubtype() const;
        virtual bool isAvailable(const boost::shared_ptr<Player> player) const;
    };
    //桃
    class Peach: public BasicCard{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(BasicCard)
        Peach(Card::Suit suit, int number);
        virtual string getSubtype() const;
        virtual void use(boost::shared_ptr<Room> room, boost::shared_ptr<ServerPlayer> source, vector<boost::shared_ptr<ServerPlayer> > &targets) const;
        virtual void onEffect(const CardEffectStruct &effect) const;
        virtual bool isAvailable(const boost::shared_ptr<Player> player) const;
    };
    //顺手牵羊
    class Snatch:public SingleTargetTrick{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(SingleTargetTrick)
        Snatch(Card::Suit suit, int number);

        virtual bool targetFilter(const vector<const boost::shared_ptr<Player> > &targets, const boost::shared_ptr<Player> to_select, const boost::shared_ptr<Player> Self) const;
        virtual void onEffect(const CardEffectStruct &effect) const;
    };
    //过河拆桥
    class Dismantlement: public SingleTargetTrick{
    public:
        IS_KIND_OF_DECLARE_FOR_ONE_PARENT(SingleTargetTrick)
        Dismantlement(Card::Suit suit, int number);

        virtual bool targetFilter(const vector<const boost::shared_ptr<Player> > &targets, const boost::shared_ptr<Player> to_select, const boost::shared_ptr<Player> Self) const;
        virtual void onEffect(const CardEffectStruct &effect) const;
    };
    */
};
};

#endif
