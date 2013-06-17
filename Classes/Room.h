#ifndef __ROOM_H__
#define __ROOM_H__

#include <vector>
#include <boost\smart_ptr.hpp>
#include "Structs.h"
#include "ServerPlayer.h"
#include "Card.h"
#include "IoHandler.h"

namespace sgs {
    namespace package {
        class TrickCard;
    }
}
using namespace sgs::package;

using namespace std;
using namespace sgs::core;
using namespace sgs::net;

namespace sgs {
    namespace server {

        /**
        * @brief 房间类
        **/
        class Room : public sgs::net::IoHandler
        {
        public:
            Room(void);
            ~Room(void);
            /**
            * @brief 返回被激活的用户（回合内）
            **/
            boost::shared_ptr<ServerPlayer> getCurrent() const;
            /**
            * @brief 激活某用户
            **/
            void setCurrent(boost::shared_ptr<ServerPlayer> current);
            /**
            * @brief 计算活着的玩家
            * @return int
            **/
            int alivePlayerCount() const;
            /**
            * @brief 获取其他玩家
            **/
            vector<boost::shared_ptr<ServerPlayer> > getOtherPlayers(boost::shared_ptr<ServerPlayer> except) const;
            /**
            * @brief 获取所有玩家
            **/
            vector<boost::shared_ptr<ServerPlayer> > getPlayers() const;
            /**
            * @brief 以当前玩家为起点获取所有玩家
            **/
            vector<boost::shared_ptr<ServerPlayer> > getAllPlayers() const;
            /**
            * @brief 获取所有活着的玩家
            **/
            vector<boost::shared_ptr<ServerPlayer> > getAlivePlayers() const;
            /**
            * @brief 进入临死状态
            **/
            void enterDying(boost::shared_ptr<ServerPlayer> player, boost::shared_ptr<DamageStruct> reason);
            /**
            * @brief 杀死玩家
            **/
            void killPlayer(boost::shared_ptr<ServerPlayer> player, boost::shared_ptr<DamageStruct> reason = boost::shared_ptr<DamageStruct>());
            /**
            * @brief 复活玩家
            **/
            void revivePlayer(boost::shared_ptr<ServerPlayer> player);
            /**
            * @brief 获取活着的角色
            **/
            vector<string> aliveRoles(boost::shared_ptr<ServerPlayer> except = boost::shared_ptr<ServerPlayer>()) const;
            /**
            * @brief 游戏结束
            **/
            void gameOver(const string &winner);
            /**
            * @brief 杀效果，函数中包含SlashEffect\SlashEffected两个事件的触发。
            *        他们一个是造成杀的效果，例如朱雀羽扇使无属性杀变火杀、纵火、雌雄双股剑、青钢剑、激昂摸牌
            *        另一个是受到杀的效果，如仁王盾使黑杀无效、藤甲使普通杀无效、享乐令杀者弃基本牌、激昂摸牌
            **/
            void slashEffect(const SlashEffectStruct &effect);
            /**
            * @brief 杀的结果，函数中包含SlashHit、SlashMissed两个事件的触发。
            *        前者是杀命中，如麒麟弓、寒冰剑，结果是造成1点杀的伤害，若此杀的杀伤力被酒加强，则造成2点伤害
            *        后者是杀被闪避，如贯石斧、青龙偃月刀、猛进。
            **/
            void slashResult(const SlashEffectStruct &effect, const boost::shared_ptr<Card> jink);
            /**
            * @brief 给玩家附加技能，一般是与他人有关或武器的技能，如黄天、制霸、义舍要牌等
            **/
            void attachSkillToPlayer(boost::shared_ptr<ServerPlayer> player, const string &skillName);
            /**
            * @brief 给玩家卸掉技能，情况如上
            **/
            void detachSkillFromPlayer(boost::shared_ptr<ServerPlayer> player, const string &skillName);
            /**
            * @brief 给玩家添加货去掉某标记
            **/
            void setPlayerFlag(boost::shared_ptr<ServerPlayer> player, const string &flag);
            /**
            * @brief 修改某玩家的某属性，多用于觉醒技修改血量上线，sp武将与原版武将变身时修改国籍
            **/
            void setPlayerProperty(boost::shared_ptr<ServerPlayer> player, const char *property_name, const string &value);
            /**
            * @brief 设置玩家的标记物
            **/
            void setPlayerMark(boost::shared_ptr<ServerPlayer> player, const string &mark, int value);
            /**
            * @brief 使用某卡牌
            **/
            void useCard(const CardUseStruct &card_use, bool add_history = true);
            /**
            * @brief 处理伤害
            **/
            void damage(DamageStruct &data);
            /**
            * @brief 流失体力
            **/
            void loseHp(boost::shared_ptr<ServerPlayer> victim, int lose = 1);
            /**
            * @brief 流失体力上限
            **/
            void loseMaxHp(boost::shared_ptr<ServerPlayer> victim, int lose = 1);
            /**
            * @brief 恢复体力
            **/
            void recover(boost::shared_ptr<ServerPlayer> player, const RecoverStruct &recover, bool set_emotion = false);
            /**
            * @brief 卡牌使用效果
            **/
            bool cardEffect(const boost::shared_ptr<Card> card, boost::shared_ptr<ServerPlayer> from, boost::shared_ptr<ServerPlayer> to);
            bool cardEffect(const CardEffectStruct &effect);
            /**
            * @brief 判定
            **/
            void judge(JudgeStruct &judge_struct);
            /**
            * @brief 抓n张牌
            **/
            vector<int> getNCards(int n, bool update_pile_number = true);
            /**
            * @brief 获取主公玩家
            **/
            boost::shared_ptr<ServerPlayer> getLord() const;
            /**
            * @brief 询问是否观星
            **/
            void askForGuanxing(boost::shared_ptr<ServerPlayer> zhuge, const vector<int> &cards, bool up_only);
            void doGongxin(boost::shared_ptr<ServerPlayer> shenlvmeng, boost::shared_ptr<ServerPlayer> *target);
            /**
            * @brief 摸一张牌
            **/
            int drawCard();
            /**
            * @brief 填充所有合理牌到五谷丰登
            **/
            void fillAG(const vector<int> &card_ids, boost::shared_ptr<ServerPlayer> who = boost::shared_ptr<ServerPlayer>());
            /**
            * @brief 从五谷丰登中抓取一张牌
            **/
            void takeAG(boost::shared_ptr<ServerPlayer> player, int card_id);
            /**
            * @brief 由系统提供某牌，即无需自己打出。如八卦判红的闪，激将的杀，护驾的闪
            **/
            void provide(const boost::shared_ptr<Card> card);
            /**
            * @brief 获得臣民，用于返回某主公麾下的武将
            **/
            vector<boost::shared_ptr<ServerPlayer> > getLieges(const string &kingdom, boost::shared_ptr<ServerPlayer> lord) const;
            /**
            * @brief 展示某牌，如火攻
            **/
            void showCard(boost::shared_ptr<ServerPlayer> player, int card_id, boost::shared_ptr<ServerPlayer> only_viewer = boost::shared_ptr<ServerPlayer>());
            /**
            * @brief 展示所有牌
            **/
            void showAllCards(boost::shared_ptr<ServerPlayer> player, boost::shared_ptr<ServerPlayer> to = boost::shared_ptr<ServerPlayer>());   
            /**
            * @brief 摸牌
            **/
            void drawCards(boost::shared_ptr<ServerPlayer> player, int n, const string &reason = string());
            /**
            * @brief 获得牌
            **/
            void obtainCard(boost::shared_ptr<ServerPlayer> target, const boost::shared_ptr<Card> card, bool unhide = true);
            /**
            * @brief 弃牌
            **/
            void throwCard(int card_id, boost::shared_ptr<ServerPlayer> who);
            /**
            * @brief 移动牌
            **/
            void moveCardTo(const boost::shared_ptr<Card> card, boost::shared_ptr<ServerPlayer> dstPlayer, Player::Place dstPlace,
                bool forceMoveVisible = false);

            /**
            * @brief 周瑜反间提示花色
            **/
            Card::Suit askForSuit(boost::shared_ptr<ServerPlayer> player, const string &reason);
            /**
            * @brief 提示技能触发、选择是否发动
            **/
            bool askForSkillInvoke(boost::shared_ptr<ServerPlayer> player, const string &skill_name, const string &data = string());
            /**
            * @brief 提示弃牌
            **/
            bool askForDiscard(boost::shared_ptr<ServerPlayer> target, const string &reason, int discard_num, int min_num,
                bool optional = false, bool include_equip = false, const string &prompt = string());
            /**
            * @brief 提示是否打出无懈可击
            **/
            bool askForNullification(const boost::shared_ptr<TrickCard> trick, boost::shared_ptr<ServerPlayer> from, boost::shared_ptr<ServerPlayer> to, bool positive);
            /**
            * @brief 提示要求打出某牌，如杀和万箭提示闪，南蛮和挑衅提示杀
            **/
            //const boost::shared_ptr<Card> askForCard(boost::shared_ptr<ServerPlayer> player, const string &pattern, const string &prompt,
            //                  const QVariant &data = QVariant(), TriggerEvent trigger_event = CardResponsed, ServerPlayer *to = NULL);

            bool isFull() const;
            bool isFinished() const;
        protected:
            //游戏模式
            string mode_;
            //所有玩家，活着的玩家
            vector<boost::shared_ptr<ServerPlayer> > players_, alivePlayers_;
            //
            boost::shared_ptr<ServerPlayer> owner_;
            //玩家数量
            int playerCount_;
            //当前激活玩家
            boost::shared_ptr<ServerPlayer> current_;
            boost::shared_ptr<ServerPlayer> replyPlayer_;
            vector<int> pile1_, pile2_;
            //摆在桌面上但未进入棋牌区的牌，如判定牌，孟获的再起牌
            vector<int> tableCards_;
            //摸牌堆，弃牌堆
            vector<int> drawPile_, discardPile_;
            //游戏是否开始
            bool gameStarted_;
            //游戏是否结束
            bool gameFinished_;
            int signupCount_;
            //记录了每张牌及其位置
            map<int, Player::Place> placeMap_;
            //记录了每张牌及其拥有者
            map<int, boost::shared_ptr<ServerPlayer> > ownerMap_;
            //服务
            //boost::shared_ptr<AsioServer> asioServerPtr_;
        };

    };
};

#endif