//
//  DataValue.h
//  胖次英雄
//
//  Created by lj on 14-3-5.
//
//

#ifndef ________DataValue__
#define ________DataValue__

#include <iostream>
#include "cocos2d.h"

class DataValue
{
public:
    DataValue(){};
    DataValue(int i)
    {
        update_req();
        set_data(i);
    };
    ~DataValue(){};
    
    //score function
    void set_data(int i)
    {
        _data=i;
        _data^=_req;
    };
    void offset_data(int i)
    {
        set_data(get_data()+i);
    };
    int get_data()
    {
        return _data^_req;
    };
    
    void update_req()
    {
        _req=CCRANDOM_0_1()*10+2;
    }
private:
    int _data;
    int _req;
};

#endif /* defined(________DataValue__) */
