//
//  PantsList.cpp
//  PantsHero
//
//  Created by lyon on 14-3-9.
//
//

#include "PantsList.h"

bool PantsList::init(const std::string& pszPlist)
{
    bool ret=false;
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(pszPlist);
    ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(fullPath);
    
    for(int i=1;i<=100;i++)
    {
        ValueMap node=dict.at(String::createWithFormat("%d",i)->getCString()).asValueMap();
        if(node.at("Number").asString()!="null")
        {
            std::string name=node.at("Name").asString();
            std::cout<<name<<std::endl;
        }
    }
    ret=true;
    return ret;
}