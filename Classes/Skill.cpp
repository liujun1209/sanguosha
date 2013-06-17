#include "Skill.h"

namespace sgs {
    namespace core {

        CCDictionary* Skill::SKILL_STRINGS = NULL;//CCDictionary::createWithContentsOfFileThreadSafe("strings/skills.xml");;
        
        string Skill::getSkillString(const string& key) const {
            if(!SKILL_STRINGS) {
                SKILL_STRINGS = CCDictionary::createWithContentsOfFileThreadSafe("strings/skills.xml");
            }
            CCString* description = ((CCString*)(SKILL_STRINGS->objectForKey(key)));
            return description->m_sString;
        }

        Skill::Skill(const string &name, Frequency frequent): name_(name), frequency_(frequent)
        {
        }


        Skill::~Skill(void)
        {
        }
        //TODO
        /**
        * @brief 获取技能描述
        **/
        string Skill::getDescription() const {
            const string key = getName()+ "_description";
            return getSkillString(key);
        }
        /**
        * @brief 限定技、锁定技、觉醒技前面会有【xx技】的字样，就是这个函数得到
        **/
        string Skill::getText() const {
            return "";//((CCString*)(skillStrings->objectForKey(getName()+ "_text")))->m_sString;
        }

        
        string Skill::getChineseName() const {
            string key = getName()+ "_chinese";
            return getSkillString(key);
        }
        /**
        * @brief 获取默认选择，这个应该是在有出牌时限的情况下，对于一些需要选择发动的技能，如果玩家没有选择，系统自动分那会默认选项。
        *   如董卓的崩坏(上限>=血量+2就掉上线，否则掉血）
        **/
        string Skill::getDefaultChoice(boost::shared_ptr<ServerPlayer> player) const {
            return "";
        }
        /**
        * @brief 增加标记
        **/
        void Skill::setFlag(boost::shared_ptr<ServerPlayer> player) const {

        }
        /**
        * @brief 减少标记
        **/
        void Skill::unsetFlag(boost::shared_ptr<ServerPlayer> player) const {
        }

        ViewAsSkill::ViewAsSkill(const string &name):Skill(name) {

        }
       
        /**
        * @brief 用于判断技能是否可以使用，如果返回false, 技能按钮被禁用
        * @return bool
        **/
        bool ViewAsSkill::isAvailable(CardUseStruct::CardUseReason reason, const string &pattern) const {
            return false;
        }
        /**
        * @brief 判断技能是否可以在出牌阶段无其他事件激励的条件下是否可以发动
        *        如在没有判定的时候，司马的鬼才是被禁用的
        **/
         bool ViewAsSkill::isEnabledAtPlay(const boost::shared_ptr<Player> player) const {
            return false;
         }
        /**
        * @brief 一些技能只有在特定条件下才能被激活
        *        比如被杀时的倾国和龙胆、使用锦囊时的看破。
        *        这些条件由参数pattern给出，当条件符合时，这些技能会被激活
        **/
         bool ViewAsSkill::isEnabledAtResponse(const boost::shared_ptr<Player> player, const string &pattern) const {
            return false;
         }
        //virtual bool isEnabledAtNullification(const boost::shared_ptr<ServerPlayer> player) const;

        const ViewAsSkill* ViewAsSkill::parseViewAsSkill(const Skill* skill) {
            return NULL;
        }

        /**
        * @brief 用于发动技能时过滤掉不能使用的卡牌
        *        如倾国会过滤掉装备牌和红色手牌、青囊会过滤掉黑色装备手牌
        **/
        bool ZeroCardViewAsSkill::viewFilter(const vector<const boost::shared_ptr<Card> > &selected, const boost::shared_ptr<Card> to_select) const {
            return false;
        }
        /**
        * @brief 用于在发动技能时将牌视作转化后的卡牌
        *        如倾国会视为闪
        **/
        const boost::shared_ptr<Card> ZeroCardViewAsSkill::viewAs(const vector<const boost::shared_ptr<Card> > &cards) const {
            return boost::shared_ptr<Card>();
        }

        /**
        * @brief 用于发动技能时过滤掉不能使用的卡牌
        *        如倾国会过滤掉装备牌和红色手牌、青囊会过滤掉黑色装备手牌
        **/
        bool OneCardViewAsSkill::viewFilter(const vector<const boost::shared_ptr<Card> > &selected, const boost::shared_ptr<Card> to_select) const {
            return false;
        }
        /**
        * @brief 用于在发动技能时将牌视作转化后的卡牌
        *        如倾国会视为闪
        **/
        const boost::shared_ptr<Card> OneCardViewAsSkill::viewAs(const vector<const Card *> &cards) const {
            return boost::shared_ptr<Card>();
        }

        TriggerSkill::TriggerSkill(const string &name):Skill(name) {
        }
        const boost::shared_ptr<ViewAsSkill> TriggerSkill::getViewAsSkill() const {
            return boost::shared_ptr<ViewAsSkill>();
        }
        
        /**
        * @brief 获取时间优先级
        **/
        int TriggerSkill::getPriority() const {
            return 0;
        }
        bool TriggerSkill::triggerable(const boost::shared_ptr<ServerPlayer> target) const {
            return false;
        }

       

        int MasochismSkill::getPriority() const {
            return 0;
        }

        ProhibitSkill::ProhibitSkill(const string &name):Skill(name) {
        }

        DistanceSkill::DistanceSkill(const string &name):Skill(name) {
        }


        bool WeaponSkill::triggerable(const boost::shared_ptr<ServerPlayer> target) const {
            return false;
        }

        bool ArmorSkill::triggerable(const boost::shared_ptr<ServerPlayer>target) const {
            return false;
        }

        void MarkAssignSkill::onGameStart(ServerPlayer *player) const {
        }
    };
};