#include "Engine.h"
#include <time.h>
#include <boost\foreach.hpp>

namespace sgs {
    namespace core {

        Engine*  Engine::instance = NULL;

        Engine::Engine(void)
        {
            this->addPackage("Standard");
        }


        Engine::~Engine(void)
        {
        }

        Engine* Engine::getInstance() {
            if(instance == NULL) {
                instance =  new Engine();
            }
            return instance;
        }

        const boost::shared_ptr<Skill> Engine::getSkill(const string &skillName) const {
            map<string, const boost::shared_ptr<Skill> >::const_iterator it = skills_.find(skillName);
            if( it != skills_.end()) {
                return it->second;
            }
            return boost::shared_ptr<Skill>();
        }


        void Engine::addSkills(vector<boost::shared_ptr<Skill> >& skills) {
            BOOST_FOREACH(const boost::shared_ptr<Skill> skill, skills) {
                //插入
                skills_.insert(pair<string,boost::shared_ptr<Skill> >(skill->getName(), skill));
            }
        }

        const boost::shared_ptr<TriggerSkill> Engine::getTriggerSkill(const string &skillName) const {
            map<string, const boost::shared_ptr<TriggerSkill> >::const_iterator it = triggerSkills_.find(skillName);
            if( it != triggerSkills_.end()) {
                return it->second;
            }
            return boost::shared_ptr<TriggerSkill>();
        }


        void Engine::addTriggerSkills(const vector<boost::shared_ptr<TriggerSkill> > &skills) {
            BOOST_FOREACH(boost::shared_ptr<TriggerSkill> skill, skills) {
                //插入
                triggerSkills_.insert(pair<string,boost::shared_ptr<TriggerSkill> >(skill->getName(), skill));
            }
        }
        void Engine::addPackage(const string& name) {
            boost::shared_ptr<Package> pack = PackageAdder::packages[name];
            if(pack)
                addPackage(pack);
        }

        void Engine::addPackage(boost::shared_ptr<Package> package) {
            //TODO避免重复插入

            vector<boost::shared_ptr<General> > generals = package->getGenerals();
            BOOST_FOREACH(boost::shared_ptr<General> general , generals) {
                //添加技能
                vector< boost::shared_ptr<Skill> > skills = general->getSkillList();
                addSkills(skills);

                if(general->isLord()) {
                    lordList_.push_back(general->getName());
                }
                else {
                    nonlordList_.push_back(general->getName());
                }

                generals_.insert(pair<string, const boost::shared_ptr<General> >(general->getName(),general));
            }

            vector<boost::shared_ptr<Card> > cards = package->getCards();
             BOOST_FOREACH(boost::shared_ptr<Card> card , cards) {
                 cardList_.push_back(card->getId());
                 cards_.insert(pair<int, const boost::shared_ptr<Card> >(card->getId(), card));
             }
        }

        vector<string> Engine::getRandomGenerals(int count, const set<string> &banSet) const {
            vector<string> allGenerals;
            BOOST_FOREACH(string general , lordList_) {
                if(banSet.find(general) == banSet.end()) {
                    allGenerals.push_back(general);
                }
            }

            BOOST_FOREACH(string general , nonlordList_) {
                if(banSet.find(general) == banSet.end()) {
                    allGenerals.push_back(general);
                }
            }

            //random_shuffle(allGenerals.begin(), allGenerals.end());
            vector<string> generals;
            srand( (unsigned)time( NULL ) );
            if( allGenerals.size() > 0) {
                for(int i = 0; i< count; i++ ){
                    int index = rand() % allGenerals.size();
                    string general = allGenerals.at(index);
                    generals.push_back(general);
                    allGenerals.erase(allGenerals.begin() + index);
                }
            }
            return generals;
        }

        
        boost::shared_ptr<General> Engine::getGeneral(const string& name) {
            return generals_[name];
        }
        
        vector<boost::shared_ptr<General> > Engine::getGeneral(const vector<string> names) {
            vector<boost::shared_ptr<General> >  generals;
            BOOST_FOREACH(string name,names) {
                generals.push_back(getGeneral(name));
            }
            return generals;
        }

         /**
        * @brief 获取卡片张数
        **/
        int Engine::getCardCount() const {
            return cardList_.size();
        }

        /**
        * @brief 获取卡片实体
        **/
        boost::shared_ptr<Card> Engine::getCard(int cardId) {
            return cards_[cardId];
        }

        
        set<boost::shared_ptr<Card> > Engine::getCards(set<int> cardIds) {
            set<boost::shared_ptr<Card> > temp;
            BOOST_FOREACH(int id, cardIds) {
                temp.insert(getCard(id));
            }
            return temp;
        }

        vector<boost::shared_ptr<Card> > Engine::getCards(vector<int> cardIds) {
            vector<boost::shared_ptr<Card> > temp;
            BOOST_FOREACH(int id, cardIds) {
                temp.push_back(getCard(id));
            }
            return temp;
        }

        /**
        * @brief 获取获取所有卡片id
        **/
        vector<int> Engine::getAllCardsId() {
            return cardList_;
        }

        /**
        * @brief 获取获取所有卡片实体
        **/
        vector<boost::shared_ptr<Card> > Engine::getAllCardsEntity() {
            return getCards(getAllCardsId());
        }

    }
}
