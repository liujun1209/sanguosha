#include "General.h"
#include "Standard.h"

#include <boost\smart_ptr.hpp>

using namespace sgs::core;

namespace sgs {
namespace package {
    
    
    void StandardPackage::addGenerals() {
        //Îº
        //²Ü²Ù
        boost::shared_ptr<General> caocao(new General(shared_from_this(), "caocao$", "wei"));
        caocao->addSkill(boost::shared_ptr<Skill>(new Skill("jianxiong")));
        caocao->addSkill(boost::shared_ptr<Skill>(new Skill("hujia")));
        generals_.push_back(caocao);

        //ÕÅÁÉ
        boost::shared_ptr<General> zhangliao(new General(shared_from_this(), "zhangliao", "wei"));
        generals_.push_back(zhangliao);

        //¹ù¼Î
        boost::shared_ptr<General> guojia(new General(shared_from_this(), "guojia", "wei", 3));
        generals_.push_back(guojia);

        //ÏÄºîª
        boost::shared_ptr<General> xiahoudun(new General(shared_from_this(), "xiahoudun", "wei"));
        generals_.push_back(xiahoudun);
        
        //Ë¾ÂíÜ²
        boost::shared_ptr<General> simayi(new General(shared_from_this(), "simayi", "wei", 3));
        generals_.push_back(simayi);

        //ĞíñÒ
        boost::shared_ptr<General> xuchu(new General(shared_from_this(), "xuchu", "wei"));
        generals_.push_back(xuchu);

        //Õç¼§
        boost::shared_ptr<General> zhenji(new General(shared_from_this(), "zhenji", "wei", 3 , General::Female));
        generals_.push_back(zhenji);

        //Êñ
        //Áõ±¸
        boost::shared_ptr<General> liubei(new General(shared_from_this(), "liubei$", "shu"));
        liubei->addSkill(boost::shared_ptr<Skill>(new Skill("rende")));
        liubei->addSkill(boost::shared_ptr<Skill>(new Skill("jijiang")));
        generals_.push_back(liubei);

        //¹ØÓğ *zhaoyun, *machao, *zhugeliang, *huangyueying;
        boost::shared_ptr<General> guanyu(new General(shared_from_this(), "guanyu", "shu"));
        generals_.push_back(guanyu);

        //ÕÅ·É
        boost::shared_ptr<General> zhangfei(new General(shared_from_this(), "zhangfei", "shu"));
        generals_.push_back(zhangfei);

        //ÕÔÔÆ
        boost::shared_ptr<General> zhaoyun(new General(shared_from_this(), "zhaoyun", "shu"));
        generals_.push_back(zhaoyun);

        //Âí³¬
        boost::shared_ptr<General> machao(new General(shared_from_this(), "machao", "shu"));
        generals_.push_back(machao);

        //Öî¸ğÁÁ
        boost::shared_ptr<General> zhugeliang(new General(shared_from_this(), "zhugeliang", "shu", 3));
        generals_.push_back(zhugeliang);

        //»ÆÔÂÓ¢
        boost::shared_ptr<General> huangyueying(new General(shared_from_this(), "huangyueying", "shu", 3, General::Female));
        generals_.push_back(huangyueying);

        //Îâ
        //ËïÈ¨
        boost::shared_ptr<General> sunquan(new General(shared_from_this(), "sunquan$", "wu"));
        sunquan->addSkill(boost::shared_ptr<Skill>(new Skill("zhiheng")));
        sunquan->addSkill(boost::shared_ptr<Skill>(new Skill("jiuyuan")));
        generals_.push_back(sunquan);

        //ÖÜè¤
        boost::shared_ptr<General> zhouyu(new General(shared_from_this(), "zhouyu", "wu", 3));
        generals_.push_back(zhouyu);

        //ÂÀÃÉ
        boost::shared_ptr<General> lvmeng(new General(shared_from_this(), "lvmeng", "wu"));
        generals_.push_back(lvmeng);

        //Â½Ñ·
        boost::shared_ptr<General> luxun(new General(shared_from_this(), "luxun", "wu", 3));
        generals_.push_back(luxun);

        //¸ÊÄş
        boost::shared_ptr<General> ganning(new General(shared_from_this(), "ganning", "wu"));
        generals_.push_back(ganning);

        //»Æ¸Ç
        boost::shared_ptr<General> huanggai(new General(shared_from_this(), "huanggai", "wu"));
        generals_.push_back(huanggai);

        //´óÇÇ
        boost::shared_ptr<General> daqiao(new General(shared_from_this(), "daqiao", "wu", 3, General::Female));
        generals_.push_back(daqiao);

        //ËïÉĞÏã
        boost::shared_ptr<General> sunshangxiang(new General(shared_from_this(), "sunshangxiang", "wu", 3, General::Female));
        generals_.push_back(sunshangxiang);

        //Èº
        //ÂÀ²¼
        boost::shared_ptr<General> lvbu(new General(shared_from_this(), "lvbu", "qun"));
        generals_.push_back(lvbu);
        //»ªÙ¢
        boost::shared_ptr<General> huatuo(new General(shared_from_this(), "huatuo", "qun", 3));
        generals_.push_back(huatuo);
        //õõ²õ
        boost::shared_ptr<General> diaochan(new General(shared_from_this(), "diaochan", "qun", 3, General::Female));
        generals_.push_back(diaochan);
    }

}
}
