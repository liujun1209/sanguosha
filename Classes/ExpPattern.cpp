#include "ExpPattern.h"
#include <boost\foreach.hpp>
#include <boost\algorithm\string.hpp>
#include <boost\lexical_cast.hpp>

namespace sgs {
namespace package {


    ExpPattern::ExpPattern(const string &exp)
    {
        this->exp = exp;
    }

    bool ExpPattern::match(const boost::shared_ptr<Player> player, const boost::shared_ptr<Card> card) const
    {
        
        vector<string> tempVector;
        boost::find_all(tempVector, exp, "#");
        BOOST_FOREACH(string one_exp, tempVector)
            if(this->matchOne(player,card,one_exp))return true;
            
        return false;
    }

    // '|' means 'and', '#' means 'or'.
    // the expression splited by '#' has 3 parts,
    // 1st part means the card name, and ',' means more than one options.
    // 2nd patt means the card suit, and ',' means more than one options.
    // 3rd part means the card number, and ',' means more than one options,
    // the number uses '~' to make a scale for valid expressions
    bool ExpPattern::matchOne(const boost::shared_ptr<Player> player, const boost::shared_ptr<Card> card, string exp) const
    {
        vector<string> factors;
        boost::find_all(factors, exp, "|");

        bool checkpoint = false;
        vector<string> card_types;
        boost::find_all(card_types, factors.at(0), ",");
        BOOST_FOREACH(string name, card_types)
            if(card->isKindOf(name) || name == ".")checkpoint = true;
        if(!checkpoint)return false;
        if(factors.size() < 2)return true;

        checkpoint = false;
        vector<string> card_suits;
        boost::find_all(card_suits, factors.at(1), ",");
        BOOST_FOREACH(string suit,card_suits)
            if(card->getSuitString() == suit || suit == ".")checkpoint = true;
        if(!checkpoint)return false;
        if(factors.size()<3)return true;

        checkpoint = false;
        vector<string> card_numbers;
        boost::find_all(card_numbers, factors.at(2), ",");
        int cdn = card->getNumber();

        BOOST_FOREACH(string number,card_numbers)
        {
            if(number.find('~') != string::npos)
            {
                vector<string> params;
                boost::find_all(card_numbers, number, "~");
                int from,to;
                if(!params.at(0).size()) {
                    from = 1;
                } else {
                    try{
                        from = boost::lexical_cast<int,string>(params.at(0));
                    }catch(...){}
                }
                if(!params.at(1).size()) { 
                    to = 13;
                } else { 
                    try{
                        to = boost::lexical_cast<int,string>(params.at(1));
                    }catch(...){}
                }
                if(from<= cdn && cdn <= to)checkpoint = true;
            }
            else {
                int tempNumber = -1;
                try{
                    tempNumber = boost::lexical_cast<int,string>(number);
                }catch(...){}
                if(tempNumber == cdn)checkpoint = true;
                else if(number == ".")checkpoint = true;
            }
        }
        if(!checkpoint)return false;
        if(factors.size()<4)return true;

        checkpoint = false;
        string place = factors.at(3);
        if(place == ".")checkpoint = true;
        else if(place == "equipped" && player->hasEquip(card))checkpoint = true;
        else if(place == "hand" && !player->hasEquip(card))checkpoint = true;
        if(!checkpoint)return false;
        if(factors.size()<5)return true;

        string color = factors.at(4);
        if(color == ".")return true;
        //else if (color == "red" && card->isRed())return true;
        //else if (color == "black" && card->isBlack())return true;
        
        return false;

    }

};
};