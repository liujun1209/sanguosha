
#include <boost\foreach.hpp>
#include "General.h"
#include "Engine.h"
#include "Standard.h"

namespace sgs {
    namespace core {

        CCDictionary* General::GENERAL_STRINGS = NULL;//CCDictionary::createWithContentsOfFileThreadSafe("strings/general.xml");;

        General::General(boost::shared_ptr<Package> packagePtr, const string &name, const string &kingdom, int maxHp, Gender gender, bool hidden, bool never_shown):
        packagePtr_(packagePtr), name_(name), kingdom_(kingdom), maxHp_(maxHp), gender_(gender) , hidden_(hidden)
        {
            static char lord_symbol('$');
            int index = name.find(lord_symbol);
            if( index != string::npos){
                string copy = name;
                copy.erase(index, 1);
                lord_ = true;
                setName(copy);
            }else{
                lord_ = false;
                setName(name);
            }
        }

        General::~General(void)
        {
        }
        
        string General::getChineseName() const {
            if(!GENERAL_STRINGS) {
                GENERAL_STRINGS = CCDictionary::createWithContentsOfFileThreadSafe("strings/general.xml");
            }
            CCString* description = ((CCString*)(GENERAL_STRINGS->objectForKey(getName())));
            return description->m_sString;
        } 
        void General::addSkill(boost::shared_ptr<Skill> skillPtr) {
            skillPtr->setParent(this->shared_from_this());
            skillSet_.insert(skillPtr);
            skillNameSet_.insert(skillPtr->getName());
        }

        void General::addExtraSkill(boost::shared_ptr<Skill> skillPtr) {
            extraSet_.insert(skillPtr);
            extraNameSet_.insert(skillPtr->getName());
        }

        bool General::hasSkill(const string &skill_name) const {
            return skillNameSet_.find(skill_name) != skillNameSet_.end()
                || extraNameSet_.find(skill_name) != extraNameSet_.end();
        }
        /**
        * @brief 获取技能列表
        * @return vector<const boost::shared_ptr<Skill>> 技能列表
        */
        vector<boost::shared_ptr<Skill> > General::getSkillList() const{
            vector<boost::shared_ptr<Skill> > skills;
            BOOST_FOREACH(const boost::shared_ptr<Skill> skill, skillSet_) {
                skills.push_back(skill);
            }
            return skills;
        }


        string General::getSkillDescription() {
            string skillDescription;
            BOOST_FOREACH(boost::shared_ptr<Skill> skillPtr, getVisibleSkillList()) {
                skillDescription.append(skillPtr->getChineseName());
                skillDescription.append(":\n");
                skillDescription.append(skillPtr->getDescription());
                skillDescription.append("\n");
            }
            return skillDescription;
        }
        /**
        * @brief 获取可见的技能列表
        * @return vector<const boost::shared_ptr<Skill>> 技能列表
        */
        vector<boost::shared_ptr<Skill> > General::getVisibleSkillList() const {
            vector<boost::shared_ptr<Skill> > visibleSkillList;
            BOOST_FOREACH(boost::shared_ptr<Skill> skillPtr, getSkillList()) {
                if(skillPtr->isVisible()) {
                    visibleSkillList.push_back(skillPtr);
                }
            }
            return visibleSkillList;
        }

        /**
        * @brief 获取可见的技能集合
        * @return vector<const boost::shared_ptr<Skill>> 技能列表
        */
        set<boost::shared_ptr<Skill> > General::getVisibleSkills() const {
            set<boost::shared_ptr<Skill> > visibleSkillSet;
            BOOST_FOREACH(boost::shared_ptr<Skill> skillPtr, getSkillList()) {
                if(skillPtr->isVisible()) {
                    visibleSkillSet.insert(skillPtr);
                }
            }
            return visibleSkillSet;
        }

        /**
        * @brief 获取触发技能列表（集智、枭姬等）
        * @return vector<const boost::shared_ptr<Skill>> 技能列表
        */
        set<boost::shared_ptr<TriggerSkill> > General::getTriggerSkills() const {
            set<boost::shared_ptr<TriggerSkill> > skills;
            /*BOOST_FOREACH(string skillName, skillSet_) {
            boost::shared_ptr<TriggerSkill> skill = Engine::getInstance()->getTriggerSkill(skillName);
            if(skill.get() != NULL) {
            skills.insert(skill);
            }
            }*/
            return skills;
        }
        /**
        * @brief 是否为男性
        * @return bool
        */
        bool General::isMale(){
            return gender_ == Male;
        }

        /**
        * @brief 是否为女性
        * @return bool
        */
        bool General::isFeMale() {
            return gender_ == Female;
        }
    };
};