#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <boost\smart_ptr.hpp>
#include <string>
#include <map>
#include <set>
#include <vector>
#include "Skill.h"
#include "Package.h"

using namespace std;
using namespace sgs::package;
using namespace boost;

namespace sgs {
namespace core {
    /**
    * @brief 引擎类, 单例
    **/
    class Engine
    {
    public:
        static Engine* getInstance();
        ~Engine(void);
    public:
        /**
        * @brief 获取技能
        * @param string 技能名称
        * @return boost::shared_ptr<Skill> 技能
        **/
        const boost::shared_ptr<Skill> getSkill(const string &skillName) const;
        /**
        * @brief 添加技能
        * @param vector<boost::shared_ptr<Skill> > 技能列表
        * @return void
        **/
        void addSkills( vector< boost::shared_ptr<Skill> > &skills);
        /**
        * @brief 获取触发技能
        * @param string 技能名称
        * @return boost::shared_ptr<Skill> 技能
        **/
        const boost::shared_ptr<TriggerSkill> getTriggerSkill(const string &skillName) const;
        
        /**
        * @brief 添加触发技能
        * @param vector<boost::shared_ptr<Skill> > 技能列表
        * @return void
        **/
        void addTriggerSkills(const vector<boost::shared_ptr<TriggerSkill> > &skills);

         /**
        * @brief 添加包
        * @param boost::shared_ptr<Package> package 包
        * @return void
        **/
        void addPackage(boost::shared_ptr<Package> package);

         /**
        * @brief 添加包
        * @param const string &name
        * @return void
        **/
        void addPackage(const string &name);

         /**
        * @brief 获取随机武将
        * @param int count 个数
        * @param const set<string> &ban_set 已被选武将
        * @return void
        **/
        vector<string> getRandomGenerals(int count, const set<string> &banSet = set<string>()) const;

        /**
        * @brief 获取武将实体
        **/
        boost::shared_ptr<General> getGeneral(const string& name);
        
        /**
        * @brief 获取武将实体列表
        **/
        vector<boost::shared_ptr<General> > getGeneral(const vector<string> names);

        /**
        * @brief 获取卡片张数
        **/
        int getCardCount() const;

        /**
        * @brief 获取卡片实体
        **/
        boost::shared_ptr<Card> getCard(int cardId);
        vector<boost::shared_ptr<Card> > getCards(vector<int> cardIds);
        set<boost::shared_ptr<Card> > getCards(set<int> cardIds);

        /**
        * @brief 获取获取所有卡片id
        **/
        vector<int> getAllCardsId();
        /**
        * @brief 获取获取所有卡片实体
        **/
        vector<boost::shared_ptr<Card> > getAllCardsEntity();

    private:
        Engine(void);
    private:
        //唯一实例
        static Engine* instance;
        //技能表
        map<string, const boost::shared_ptr<Skill> > skills_; 
        //触发技能表
        map<string, const boost::shared_ptr<TriggerSkill> > triggerSkills_;
        //武将表
        map<string, const boost::shared_ptr<General> > generals_;
        //主公
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(vector<string> ,lordList_ , LordList);
        //非主公
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(vector<string> ,nonlordList_ , NonLordList);

        //卡片表
        map<int, const boost::shared_ptr<Card> > cards_; 
        CC_SYNTHESIZE_READONLY_PASS_BY_REF(vector<int> ,cardList_ , CardList);
        //vector<string> lordList_, nonlordList_;
    };

}
}
#endif