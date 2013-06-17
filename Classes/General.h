#ifndef __GENERAL_H__
#define __GENERAL_H__

#include <cocos2d.h>
#include <boost\smart_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>
#include <string>
#include <vector>
#include <set>
#include "Macros.h"

using namespace std;
using namespace cocos2d;

namespace sgs {
namespace package {
    class Package;
};
};
using namespace sgs::package;

namespace sgs {
namespace core {
    class Skill;
    class TriggerSkill;
    /**
    * @brief 武将类
    **/
    class General: public boost::enable_shared_from_this<General>
    {
    private:
        static CCDictionary * GENERAL_STRINGS;
    public:
        //性别枚举
        enum Gender {SexLess, Male, Female, Neuter};
        //所属包
        CC_SYNTHESIZE_PASS_BY_REF(boost::shared_ptr<Package>, packagePtr_, PackagePtr);
        //名字
        CC_SYNTHESIZE_PASS_BY_REF(string, name_, Name);
        //国家
        CC_SYNTHESIZE_PASS_BY_REF(string, kingdom_, Kingdom);
        //总血量
        CC_SYNTHESIZE(int, maxHp_, MaxHp);
        //性别
        CC_SYNTHESIZE(Gender, gender_, Gender);
        //是否隐藏（场景人物不能选择）
        CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(hidden_, Hidden);
        //是否为主公
        CC_SYNTHESIZE_PASS_BY_REF_FOR_BOOL(lord_, Lord);
        /**
        * @brief 构造函数
        * @param Package 所属包
        * @param string name 武将名
        * @param string kingdom 国家名
        * @param maxHp 最大血量
        * @param male 性别
        * @param hidden 是否隐藏
        * @param neverShown 
        **/
        explicit General(boost::shared_ptr<Package> packagePtr, const string &name, const string &kingdom, int max_hp = 4, Gender male = Male, bool hidden = false, bool never_shown = false);
        /**
        * @brief 析构函数
        */
        ~General(void);

        
        string getChineseName() const;
        /**
        * @brief 添加技能 适用于第一次添加，比如诸葛亮的观星
        * @param Skill 技能
        * @return void
        */
        void addSkill(boost::shared_ptr<Skill> skillPtr);
        /**
        * @brief 添加技能适用于非第一次添加，比如姜维的观星
        * @param string 技能名称
        * @return void
        */
        void addExtraSkill(boost::shared_ptr<Skill> skillPtr);

        /**
        * @brief 是否拥有技能
        * @param string 技能名称
        * @return bool
        */
        bool hasSkill(const string &skill_name) const;

        string getSkillDescription();
        /**
        * @brief 获取技能列表
        * @return vector<const boost::shared_ptr<Skill>> 技能列表
        */
        vector<boost::shared_ptr<Skill> > getSkillList() const;
        /**
        * @brief 获取可见的技能列表
        * @return vector<const boost::shared_ptr<Skill>> 技能列表
        */
        vector<boost::shared_ptr<Skill> > getVisibleSkillList() const;
         /**
        * @brief 获取可见的技能集合
        * @return vector<const boost::shared_ptr<Skill>> 技能列表
        */
        set<boost::shared_ptr<Skill> > getVisibleSkills() const;
         /**
        * @brief 获取触发技能列表（集智、枭姬等）
        * @return vector<const boost::shared_ptr<Skill>> 技能列表
        */
        set<boost::shared_ptr<TriggerSkill> > getTriggerSkills() const;
        /**
        * @brief 是否为男性
        * @return bool
        */
        bool isMale();
        /**
        * @brief 是否为女性
        * @return bool
        */
        bool isFeMale();
    private:
        //技能集合
        set<boost::shared_ptr<Skill> > skillSet_;
        //技能集合
        set<string> skillNameSet_;
        //扩展技能集合
        set<string> extraNameSet_;
        set<boost::shared_ptr<Skill> > extraSet_;
    };

};
};

#endif
