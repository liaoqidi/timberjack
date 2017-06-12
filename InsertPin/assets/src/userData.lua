DATA = {
     isPass    = {2,1,1,1,1,1,1,1,1,1,--玩家通关情况{1:尚未解锁 2:已解锁 3:已通关}
                 1,1,1,1,1,1,1,1,1,1,
                 1,1,1,1,1,1,1,1,1,1,
                 1,1,1,1,1,1,1,1,1,1,
                 1,1,1,1,1,1,1,1,1,1},
     curStage  = 1,--当前是第几关
     glassTime = 0,--玩家当前沙漏数量
     scalpel   = 0,--玩家当前手术刀数量
     revGift   = 0,--玩家是否已经领取过大礼包{0,未领 1，已领}
     sound     = 2 --声音{1,关 2,开}
}

--各关卡数据
stage_configure = {
    {3,6,1,1},{3,12,1,2},{6,10,1,1},{4,15,1,2},{6,8,1.2,1},
    {6,10,1.2,2},{8,12,1.2,1},{3,18,1.2,2},{4,17,1.3,1},{5,16,1.3,2},
    {8,12,1.3,1},{6,16,1.3,2},{2,18,1.4,1},{3,20,1.4,2},{8,14,1.4,1},
    {4,16,1.4,2},{5,17,1.5,1},{4,18,1.5,2},{8,14,1.5,1},{4,18,1.5,2},
    {6,16,1.6,1},{5,17,1.6,2},{4,17,1.6,1},{5,17,1.7,2},{6,17,1.7,1},
    {2,20,1.7,2},{6,16,1.8,1},{3,21,1.8,2},{4,16,1.8,1},{8,16,1.8,2},
    {4,21,1.9,1},{3,22,1.9,2},{8,18,1.9,1},{6,20,1.9,2},{3,24,2,1},
    {4,24,2,2},{8,19,2,1},{5,22,2,2},{6,21,2.1,1},{4,24,2.1,2},
    {3,25,2.1,1},{5,22,2.1,2},{4,22,2.1,1},{5,20,2.2,2},{6,20,2.2,1},
    {2,24,2.2,2},{6,20,2.2,1},{3,25,2.2,2},{4,26,2.2,1},{8,22,2.2,2}
};

--游戏配置信息
GameConfig = {};
GameConfig.status  =  0;  --{0:正常  1:闪退 } 
GameConfig.confirm =  0;  --{0:一次确认 1:两次确认}
GameConfig.clear   =  0;  --{0:清晰 1:模糊} 

local userData = class("userData")
--创建一个userData
function userData:ctor()
	
end


--初始化用户数据
function userData:initUserData()
	--保存关卡信息
	for i = 1,#DATA.isPass do
		cc.UserDefault:getInstance():setIntegerForKey(string.format("stage_%d",i),DATA.isPass[i]);
	end
    cc.UserDefault:getInstance():setIntegerForKey("curStage", DATA.curStage);
    cc.UserDefault:getInstance():setIntegerForKey("glassTime", DATA.glassTime);
    cc.UserDefault:getInstance():setIntegerForKey("scalpel", DATA.scalpel);
    cc.UserDefault:getInstance():setIntegerForKey("revGift", DATA.revGift);
    cc.UserDefault:getInstance():setIntegerForKey("sound", DATA.sound);
end


--返回用户数据
function userData:getUserData()
	local temp_data = {};
    if tonumber(cc.UserDefault:getInstance():getIntegerForKey("curStage")) > 0 then
        local  temp_arr = {};
        for i = 1,#DATA.isPass do
           local stage =  tonumber(cc.UserDefault:getInstance():getIntegerForKey(string.format("stage_%d",i)));
           table.insert(temp_arr,table.getn(temp_arr) + 1,stage);
        end
        temp_data.isPass = temp_arr;
        temp_data.curStage = tonumber(cc.UserDefault:getInstance():getIntegerForKey("curStage"));
        temp_data.glassTime = tonumber(cc.UserDefault:getInstance():getIntegerForKey("glassTime"));
        temp_data.scalpel = tonumber(cc.UserDefault:getInstance():getIntegerForKey("scalpel"));
        temp_data.revGift = tonumber(cc.UserDefault:getInstance():getIntegerForKey("revGift"));
        temp_data.sound = tonumber(cc.UserDefault:getInstance():getIntegerForKey("sound"));
    else
        self:initUserData();
        temp_data =DATA; 
    end
    return temp_data;
end

return userData;
