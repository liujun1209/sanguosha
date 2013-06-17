#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <string>
#include <boost\smart_ptr.hpp>
namespace sgs {
namespace server {
    class Room;
    class ServerPlayer;
}
}
namespace sgs {
namespace core {
    class Card;
}
}
#include "ServerPlayer.h"

using namespace std;

using namespace sgs::core;

using namespace sgs::server;

namespace sgs {
namespace core {

    /**
    * @brief 伤害结构体
    **/
    struct DamageStruct{
        DamageStruct();

        enum Nature{
            Normal, // normal slash, duel and most damage caused by skill
            Fire,  // fire slash, fire attack and few damage skill (Yeyan, etc)
            Thunder // lightning, thunder slash, and few damage skill (Leiji, etc)
        };
        //伤害来源
        boost::shared_ptr<ServerPlayer> from;
        //伤害对象
        boost::shared_ptr<ServerPlayer> to;
        //造成伤害的卡牌
        const boost::shared_ptr<Card> card;
        //伤害点数
        int damage;
        //伤害属性
        Nature nature;
        //是否为铁锁传递来的伤害
        bool chain;
        bool transfer;
        bool trigger_chain;
    };
    /**
    * @brief 杀的结构体
    **/
    struct SlashEffectStruct{
        SlashEffectStruct();
        //使用的杀
        const boost::shared_ptr<Card> slash;
        //抵消这张杀用的闪
        const boost::shared_ptr<Card> jink;
        //杀的使用者
        boost::shared_ptr<ServerPlayer> from;
        //杀的目标
        boost::shared_ptr<ServerPlayer> to;
        //是否喝酒
        bool drank;
        //杀的属性
        DamageStruct::Nature nature;
    };

    /**
    * @brief 将死结构体
    **/
    struct DyingStruct{
        DyingStruct();
        //将死的玩家
        boost::shared_ptr<ServerPlayer> who; // who is ask for help
        //如果为空，则为体力流失造成的死亡
        boost::shared_ptr<DamageStruct> damage; // if it is NULL that means the dying is caused by losing hp
    };
    /**
    * @brief 回复结构体
    **/
    struct RecoverStruct{
        RecoverStruct();
        //回复点数
        int recover;
        //回复作用对象
        boost::shared_ptr<ServerPlayer> who;
        //产生回复效果的卡牌
        const boost::shared_ptr<Card> card;
    };

     /**
    * @brief 拼点结构体
    **/
    struct PindianStruct{
        PindianStruct();
        /**
        * @brief 是否成功
        **/
        bool isSuccess() const;
        //拼点发起者
        boost::shared_ptr<ServerPlayer> from;
        //拼点响应者
        boost::shared_ptr<ServerPlayer> to;
        //拼点发起者卡牌
        const boost::shared_ptr<Card> from_card;
        //拼点响应者卡牌
        const boost::shared_ptr<Card> to_card;
        //原因
        string reason;
    };

    /**
    * @breif 卡牌效果结构体
    **/
    struct CardEffectStruct{
        CardEffectStruct();
        //卡牌指针
        const boost::shared_ptr<Card> card;
        //卡牌使用者
        boost::shared_ptr<ServerPlayer> from;
        //卡牌使用目标
        boost::shared_ptr<ServerPlayer> to;
        //卡牌目标是否唯一
        bool multiple;
    };

    
    /**
    * @breif 卡牌使用结构体
    **/
    struct CardUseStruct{
        enum CardUseReason
        {
            CARD_USE_REASON_UNKNOWN,
            CARD_USE_REASON_PLAY,
            CARD_USE_REASON_RESPONSE
        };

        CardUseStruct();
        //判断卡牌是否合法
        bool isValid(CardUseReason reason, const string &pattern) const;
        //通过string内容给CardUseStruct赋值
        void parse(const string &str, Room *room);
        //卡牌指针
        const boost::shared_ptr<Card> card;
        //卡牌使用这
        boost::shared_ptr<ServerPlayer> from;
        //卡牌目标
        vector<boost::shared_ptr<ServerPlayer> > to;
    };

    /**
    * @brief 判定结构体模式
    **/
    class JudgeStructPattern{
    private:
        string pattern;
        bool isRegex;

    public:
        JudgeStructPattern();
        //JudgeStructPattern &operator=(const QRegExp &rx);
        //JudgeStructPattern &operator=(const QString &str);
        bool match(const Player *player, const boost::shared_ptr<Card> card) const;
    };

    /**
    * @brief 判定结构体
    **/
    struct JudgeStruct{
        JudgeStruct();
        /**
        * @brief 判定结构体
        **/
        bool isGood() const;
        bool isGood(const boost::shared_ptr<Card> card) const;
        void updateResult();
        bool isEffected();
        bool isBad() const;

        bool negative;
        bool play_animation;
        //接受判定的玩家
        boost::shared_ptr<ServerPlayer> who;
        //判定牌
        const boost::shared_ptr<Card> card;
        JudgeStructPattern pattern;
        //结果是否有利
        bool good;
        //判定原因
        string reason;
        bool time_consuming;
    private:
        enum TrialResult
        {
            TRIAL_RESULT_UNKNOWN,
            TRIAL_RESULT_GOOD,
            TRIAL_RESULT_BAD
        } _m_result;
    };


};
};
#endif