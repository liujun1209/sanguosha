#ifndef __CARD_H__
#define __CARD_H__

namespace sgs {
namespace server {
    class Room;
    class ServerPlayer;
}
}
using namespace sgs::server;

#include <cocos2d.h>
#include <string>
#include <vector>
#include <boost\smart_ptr.hpp>
#include "Macros.h"
#include "Structs.h"

using namespace cocos2d;
using namespace std;



namespace sgs {
namespace core {
    
    class Player;
    /**
    * @brief 卡片类
    **/
    class Card
    {
    private:
        static int GLOBAL_ID;
    public:
        IS_KIND_OF_DECLARE_FOR_ROOT
        //花色类，Nosuit为无色牌，比如夏侯渊的神速
        enum Suit {Spade, Club, Heart, Diamond, NoSuit};
        //颜色
        enum Color{Red, Black, Colorless};
        //卡片类型，技能，基本，锦囊，装备。
        //技能牌就是武将发动技能所打出的牌，比如貂蝉离间所打出的牌
        enum CardType{Skill, Basic, Trick, Equip };
        
        CC_SYNTHESIZE(Suit , suit_ , Suit);
        //CC_SYNTHESIZE(Color , color_ ,Color);
        CC_SYNTHESIZE(CardType , cardType_ ,CardType);
        //卡片id
        CC_SYNTHESIZE(int , id_ ,Id);
        //卡片名字
        CC_SYNTHESIZE_PASS_BY_REF(string, name_, Name);
        //number
        CC_SYNTHESIZE(int , number_, Number);
        
        //是否为红色
        //CC_SYNTHESIZE_READONLY_FOR_BOOL(red_, Red);
        //是否为黑色
        //CC_SYNTHESIZE_READONLY_FOR_BOOL(black_, Black);
        //出牌时是否不需要手动选择目标
        CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(targetFixed_, TargetFixed);
        //最低选择人数
        CC_SYNTHESIZE_PASS_BY_REF(int, minTarget_, MinTarget);
        //最高选择人数
        CC_SYNTHESIZE_PASS_BY_REF(int, maxTarget_, MaxTarget);
        //目标能否为自己
        CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(targetCanBeSelf_, TargetCanBeSelf);
        //是否允许修正最高人数（例如有方天画戟可以修正杀1->3）
        CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(allowMaxTargetWhenFixed_, AllowMaxTargetWhenFixed);
        //是否有距离限制
        CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(rangeLimit_, RangeLimit);

        
        //是否有次数限制
        CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(limitUseTimes_, LimitUseTimes);
        //使用次数限制
        CC_SYNTHESIZE_PASS_BY_REF(int, useTimesLimit_, UseTimesLimit);


        //是否多台词
        CC_SYNTHESIZE_READONLY_FOR_BOOL(mute_, Mute);
        //使用后是否立即进入弃牌堆
        CC_SYNTHESIZE_READONLY_FOR_BOOL(willThrow_, WillThrow);
        //equipped
        CC_SYNTHESIZE_READONLY_FOR_BOOL(equipped_, Equipped);
        //skill_name,当转化系的技能发动时，该技能的名字就会是打出这张牌的技能名
        CC_SYNTHESIZE_PASS_BY_REF(string, skillName_, SkillName);

        /**
        * @brief 构造函数
        * @param Suit 花色
        * @param number 大小
        * @param targetFixed 出牌时是否不需要手动选择目标，神速是一个例外
        **/
        Card(Suit suit, int number, bool targetFixed = false);
        ~Card(void);
        /**
        * @brief 获取花色文本
        **/
        string getSuitString();
        Color getColor();
        /**
        * @brief 是否为虚拟卡片
        * @return bool 
        **/
        bool isVirtualCard() const;

        int getEffectiveId() const;
        /**
        * @brief 添加子卡,所谓子卡片，就是拥有卡片转换技能的武将使用该技能时用
        *       的牌，比如甄姬使用了一张黑桃A的闪电当作闪，那么主卡牌是一张闪，
        *       花色是黑桃A，但子卡牌要加上这张黑桃A的id
        *       需要用到addSubcard的武将技能有：鬼才、倾国、龙胆、奇袭、离间、青囊等等
        * @param int cardId 
        * @return void 
        **/
        virtual void addSubcard(int cardId);
        /**
        * @brief 添加子卡
        * @param boost::shared_ptr<Card>
        * @return void 
        **/
        virtual void addSubcard(const boost::shared_ptr<Card> card);
        
        /**
        * @brief 获取所有子卡
        * @return vector<int> 
        **/
        virtual vector<int> getSubcards() const;
        
        /**
        * @brief 清除子卡
        * @return void
        **/
        virtual void clearSubcards();
        
        /**
        * @brief 获取子卡个数
        * @return int 
        **/
        virtual int getSubcardsCount() const;
         /**
        * @brief 获取卡片类型 basic trick equip
        * @return string 
        **/
        virtual string getType() const = 0;
        /**
        * @brief 获取子卡片类型，分为：
        *        攻击牌（attack_card: 雷火普通杀）
        *        防御牌（defense_card：闪）
        *        回复牌（revover_card：桃）
        *        全局效果（global_effect：五谷丰登，桃园结义）
        *        AOE（aoe：南蛮入侵，万箭齐发）
        *        单体锦囊（single_target_trick：无中、无懈、顺、拆、火攻、决斗、借刀）
        *        延时锦囊（delayed_trick：乐、兵、天灾）
        *        武器牌（weapon） 防具牌（armor）
        *        进攻马（offensive_horse）防御马（offensive_horse）
        *        辅助牌（buff_card：酒）
        *        攻击范围扩展牌（demage_spread：铁锁）
        *        恶心牌（disgusting_card：屎）
        * @return string 
        **/
        virtual string getSubtype() const = 0;
        
        /**
        * @brief 是否为非延迟锦囊
        * @return vector<int> 
        **/
        virtual bool isNDTrick() const;

        /**
        * @brief 选择的对象是否合理
        * @return vector<int> 
        **/
        //virtual bool targetsFeasible(const vector<const boost::shared_ptr<Player> > &targets, const  boost::shared_ptr<Player> self) const;
        /**
        * @brief 用于暗掉不合理的目标，比如使用杀时会暗掉攻击范围内的玩家
        * @return vector<int> 
        **/
        //virtual bool Card::targetFilter(const vector<const boost::shared_ptr<Player> > &targets, const  boost::shared_ptr<Player> to_select, const  boost::shared_ptr<Player> self) const;
        
        /**
        * @brief 用于暗掉出牌阶段不能使用的卡牌，比如没人杀时的闪
        **/
        virtual bool isAvailable(const boost::shared_ptr<Player> player) const;

        //virtual void doPreAction(Room *room, const CardUseStruct &card_use) const;
        /**
        * @brief 使用前的预处理，极少情况会需要重载这个，如极略，急袭，铁锁重铸
        **/
        virtual void onUse(boost::shared_ptr<Room> room, const CardUseStruct &card_use) const;
        /**
        * @brief 之动了目标，确定开始使用，指定目标后从room里调用playCardEffect
        **/
        virtual void use(boost::shared_ptr<Room> room, boost::shared_ptr<ServerPlayer> source, vector<boost::shared_ptr<ServerPlayer> > &targets) const;
        /**
        * @brief 卡牌使用后造成的效果
        **/
        virtual void onEffect(const CardEffectStruct &effect) const;
        /**
        * 上面三个函数例子，就是铁锁连环的使用：打出铁锁连环时，最先响应的onUser函数, 在onUse函数里首先判断这张铁锁指定了几个目标，如果没有目标就视为重铸，弃掉这张铁锁再摸一张，如果指定了1~2个目标，调用use函数
        * 在use函数中，首先要弃掉这张铁锁，然后想room申请执行使用铁锁的效果，体现在代码里就是playCardEffect这个函数，最后指定onEffect函数，将选定的目标重置或横置
        **/
        //========================================

    protected:
        //子卡
        vector<int> subcards_;
    };

    /**
    * @brief 技能牌, 例如：制衡、仁德、结姻、突袭、反间、苦肉等
    **/
    class SkillCard: public Card {
    public:
        void setUserString(const string &user_string);

        virtual string getType() const;
        virtual string getSubtype() const;
    };

    /**
    * @brief 用于弃牌和交换牌时使用，如弃牌阶段、主杀忠、行殇、缔盟。
    *   此外，无双、蹂躏的两张【闪】是放到一个DummyCard里做为subCard,再把这个DummyCard赋值为Jink的。还有贯石斧用的也是这个
    **/
    class DummyCard: public SkillCard {
    public:
        virtual string getType() const;
        virtual string getSubtype() const;
    };

};
};

#endif