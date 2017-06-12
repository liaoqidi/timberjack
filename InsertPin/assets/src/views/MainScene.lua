local MainSceneRes = "res/playGame.csb";          -- 开始游戏场景
local PinRes       = "res/pin.csb";               -- 针模板
local ReadyPinRes  = "res/readyPin.csb";          -- 准备发射针模板
local ReadyPinPng  = "failure-sk%d.png";          -- 待发射针各式图片
local gameBgMusic  = "res/bgSound.mp3";           -- 背景音乐
local winMusic     = "res/win.mp3";               -- 胜利音乐
local lostMusic    = "res/lost.mp3";              -- 失败音乐
local shootMusic   = "res/shoot.mp3";             -- 射击音乐
local openMusicRes  = "publicLayer- volume1.png"; -- 打开声音图标
local closeMusicRes = "publicLayer- volume2.png"; -- 关闭声音图标

local MainScene = class("MainScene", function()
    return cc.Layer:create()
end)

-- 创建场景
function MainScene:createScene()
    local scene = cc.Scene:create()
    scene:addChild(self)
    return scene
end

-- 构造函数
function MainScene:ctor()
    -- 注册enter与exit事件
    self:registerScriptHandler(function(state)
        if state == "enter" then
            self:_onEnter()
        elseif state == "exit" then
            self:_onExit()
        end
    end)
    self:addChild(cc.CSLoader:createNode(MainSceneRes));
    self.uiHelp   = require("UIHelper"):new();
    self.userData = require("userData"):new();
    self.cur_userData = self.userData:getUserData();
    --更换背景音乐
    if tonumber(self.cur_userData.sound) == 2 then
        AudioEngine.playMusic(gameBgMusic, true);
    else
        self.uiHelp:seekChild(self, "voice_bnt"):loadTexture(closeMusicRes,UI_TEX_TYPE_PLIST);
    end
    
    --针容器
    self.pin_plane = self.uiHelp:seekChild(self,"Panel_7");
    self.allPin_rotation = {};
    self.allStage_num = 50;
    self:setBall(self.cur_userData.curStage);
    self:setPropNum(self.cur_userData.glassTime,self.cur_userData.scalpel); 
    --玩家是否使用过沙漏
    self.isGlassTime = true;
    --玩家现在的状态
    self.playState = true;
    -- 点击事件
    self.uiHelp:seekChild(self, "playGameBg"):addTouchEventListener(handler(self,self.insertPin));
    self.uiHelp:seekChild(self, "expression_bg"):addTouchEventListener(handler(self,self.insertPin));
    self.uiHelp:seekChild(self,"toMainScene1"):addClickEventListener(handler(self,self.toMainScene));
    self.uiHelp:seekChild(self,"toMainScene2"):addClickEventListener(handler(self,self.toMainScene));
    self.uiHelp:seekChild(self,"resurrection_no"):addClickEventListener(handler(self,self.resurrection_no));
    self.uiHelp:seekChild(self,"resurrection_yes"):addClickEventListener(handler(self,self.resurrection_yes));
    self.uiHelp:seekChild(self,"toNextStage"):addClickEventListener(handler(self,self.toNextStage));
    self.uiHelp:seekChild(self,"toRestart"):addClickEventListener(handler(self,self.toRestart));
    self.uiHelp:seekChild(self,"rev_yes"):addClickEventListener(handler(self,self.rev_yes));
    self.uiHelp:seekChild(self,"rev_no"):addClickEventListener(handler(self,self.rev_no));
    self.uiHelp:seekChild(self,"hourglass_use"):addClickEventListener(handler(self,self.hourglass_use));
    self.uiHelp:seekChild(self,"pin_use"):addClickEventListener(handler(self,self.pin_use));
    self.uiHelp:seekChild(self,"revglass_yes"):addClickEventListener(handler(self,self.revglass_yes));
    self.uiHelp:seekChild(self,"revglass_no"):addClickEventListener(handler(self,self.revglass_no));
    self.uiHelp:seekChild(self,"revpin_yes"):addClickEventListener(handler(self,self.revpin_yes));
    self.uiHelp:seekChild(self,"revpin_no"):addClickEventListener(handler(self,self.revpin_no));
    self.uiHelp:seekChild(self,"voice_bnt"):addClickEventListener(handler(self,self.voice_bnt));
    self.uiHelp:seekChild(self,"confirmSure"):addClickEventListener(handler(self,self.confirmSure));
    self.uiHelp:seekChild(self,"confirmClose"):addClickEventListener(handler(self,self.confirmClose));
end

-- 进入场景
function MainScene:_onEnter()
    
end

-- 退出场景
function MainScene:_onExit()  
end

--重新进入
function MainScene:init()
    local newScene = require("views.MainScene"):new():createScene();
    local transition = cc.TransitionFade:create(0.01,newScene);
    cc.Director:getInstance():replaceScene(transition);
end


-- 设置转动的针数和待发射的针数
-- @param stage_num 当前的关数
function  MainScene:setBall(stage_num)
    self.cur_userData = self.userData:getUserData();
    local tempArr = stage_configure[tonumber(stage_num)];
    --当前针的数量
    local pin_num = tempArr[1];
    --当前针的转动方向
    local rotatePoint = 360;
    --每个枝干隔的角度
    local Rotation = 360/pin_num;
    for i = 1,pin_num do
        local temp_pin = cc.CSLoader:createNode(PinRes);
        local temp_Rotation = Rotation * (i-1);
        temp_pin:setRotation(temp_Rotation);
        self.pin_plane:addChild(temp_pin);
        --将角度保存起来
        self:saveRotation(temp_Rotation);
    end
    --设置待发射针
    self:setPinPlane(tempArr[2]);
    --设置球体转动
    self:setRotationSpeed(tonumber(stage_num),false);
    --设置关卡数字
    self.uiHelp:seekChild(self,"stage_num"):setString(self.cur_userData.curStage);

    --第一次进入游戏就提示领取大礼包
    if  tonumber(self.cur_userData.revGift) == 0  then
        local layer = self.uiHelp:seekChild(self,"rev_gift");
        self.uiHelp:setTanChuang(self,layer);
    end
end

-- 设置转动
-- @param stage_num 当前的关卡数
-- @param speed     当前的速度倍率{true:减速  false:不减速}
function MainScene:setRotationSpeed(stage_num,isHalfSpeed)
    local tempArr = stage_configure[tonumber(stage_num)];
    --默认转完一圈所需要的时间
    local SpeedTime = 4;
    local curSpeedTime = SpeedTime/tempArr[3];
    if isHalfSpeed then
        curSpeedTime = curSpeedTime *2;
    end
    --当前针的转动方向
    if tempArr[4] == 1 then
        self.rotatePoint = 360;
    else
        self.rotatePoint = -360;
    end
    local rotateAction = cc.RotateBy:create(curSpeedTime,self.rotatePoint);
    self.pin_plane:runAction(cc.RepeatForever:create(rotateAction));
end

-- 设置待发射针
-- @param readyPin_num 待发射针的数量
function  MainScene:setPinPlane(readyPin_num)
    --设置发射的针容器
    self.readyPin_plane  = self.uiHelp:seekChild(self,"usePin_panel");
    --单个待发射针高度   
    local pinHeight = 60;
    --初始坐标
    local posX = self.readyPin_plane:getContentSize().width/2;
    local posY = self.readyPin_plane:getContentSize().height - pinHeight/2; 
    --偏移量
    local moveY = pinHeight;

    for i = readyPin_num,1,-1 do
        local temp_pin   = cc.CSLoader:createNode(ReadyPinRes);
        local random_num = math.random(0,5);
        self.uiHelp:seekChild(temp_pin,"bg"):loadTexture(string.format(ReadyPinPng,random_num),UI_TEX_TYPE_PLIST);
        self.uiHelp:seekChild(temp_pin,"bg"):setName("bg"..random_num);
        temp_pin:setPosition(cc.p(posX,posY));
        posY = posY - moveY;
        self.uiHelp:seekChild(temp_pin,"readyBall_num"):setString(i);
        temp_pin:setTag(i);
        self.readyPin_plane:addChild(temp_pin);
    end
end

--将针插到最下方
function MainScene:insertPin(sender, eventType)
    if eventType == ccui.TouchEventType.began then
        if #self.readyPin_plane:getChildren() > 0 then
            local temp_pin = cc.CSLoader:createNode(PinRes);
            local allRotation = self.pin_plane:getRotation();
            --当前针的角度
            local curRotation = self:getPinRotation();
            local insertRotation = 0;
            if allRotation > 0 then
               if allRotation%360 >=  curRotation then
                    insertRotation = 360 - allRotation%360;
               else
                    insertRotation = allRotation%360;
               end             
            elseif allRotation < 0 then
                if math.abs(allRotation%360) >=  curRotation then
                    insertRotation = 360 - math.abs(allRotation%360);
                else
                    insertRotation = math.abs(allRotation%360);
                end
            else
                insertRotation = 0;
            end
            --进行碰撞测试
            if self:impactCheck(insertRotation) then
                if AudioEngine.isMusicPlaying () then
                    AudioEngine.playEffect(lostMusic);
                end

                local layer = self.uiHelp:seekChild(self,"resurrection");
                self.uiHelp:setTanChuang(self,layer);
                self:playInsertPin(2);
                self.playState = false;
            else
                if AudioEngine.isMusicPlaying () then
                    AudioEngine.playEffect(shootMusic);
                end
                self:playInsertPin(3);
                --获取当前被删除针的名字
                local pin_obj =  self.readyPin_plane:getChildByTag(#self.readyPin_plane:getChildren());
                local pin_name = self.uiHelp:seekChild(pin_obj,"ready_panel"):getChildByTag(999999):getName();
                local name_num = string.sub(pin_name,-1,-1);
                self.uiHelp:seekChild(temp_pin,"pin"):loadTexture("failure-z"..name_num..".png",UI_TEX_TYPE_PLIST);
                --设置角度
                temp_pin:setRotation(insertRotation);
                self.pin_plane:addChild(temp_pin);
                --删除待发射针
                self:delReadyPin();
                --保存当前针所curRotation在的角度的区间
                self:saveRotation(insertRotation);
            end
        end
    elseif eventType == ccui.TouchEventType.moved then
        
    elseif eventType == ccui.TouchEventType.ended then
        
    elseif eventType == ccui.TouchEventType.canceled then
       
    end
end


-- 播放各种动画
-- @param playStyle{1:游戏胜利,2:游戏失败,3:被针扎}
function MainScene:playInsertPin(playStyle)
    local face = self.uiHelp:seekChild(self,"expression_img");
    face:stopAllActions();
    if     playStyle == 1 then
        face:loadTexture("failure-sl.png",UI_TEX_TYPE_PLIST);
    elseif playStyle == 2 then
        face:loadTexture("failure-gl.png",UI_TEX_TYPE_PLIST);
    else
        local function action()
            face:loadTexture("failure-jz.png",UI_TEX_TYPE_PLIST);
        end
        local function action1()
            face:loadTexture("failure-zc.png",UI_TEX_TYPE_PLIST);
        end
        face:runAction(cc.Sequence:create(cc.CallFunc:create(action),cc.DelayTime:create(0.5),cc.CallFunc:create(action1)));
    end
end

--删除待发射的针
function MainScene:delReadyPin()
    self.cur_userData = self.userData:getUserData();
    --偏移量
    local moveY = 60;
    self.readyPin_plane:removeChildByTag(#self.readyPin_plane:getChildren(),true);
    local child = self.readyPin_plane:getChildren();
    for i = 1,#child do
        child[i]:setPositionY(child[i]:getPositionY() + moveY);
    end
    if #self.readyPin_plane:getChildren() == 0 then
        --获取当前关卡
        local Stage =  self.cur_userData.curStage;
        cc.UserDefault:getInstance():setIntegerForKey(string.format("stage_%d",Stage),3);
        Stage = Stage +1;
        cc.UserDefault:getInstance():setIntegerForKey(string.format("stage_%d",Stage),2);
        cc.UserDefault:getInstance():setIntegerForKey("curStage",Stage);
        self:playInsertPin(1);
        if Stage <= self.allStage_num then
            local layer = self.uiHelp:seekChild(self,"playSucc");
            self.uiHelp:setTanChuang(self,layer);
            if AudioEngine.isMusicPlaying () then
                AudioEngine.playEffect(winMusic);
            end
        else
            local newScene = require("views.StartScene"):new():createScene();
            local transition = cc.TransitionFade:create(0.5,newScene);
            cc.Director:getInstance():replaceScene(transition);
        end
    end
end

-- 碰撞检测
-- @param  insertRotation 当前的插入角度
-- return {true:碰撞 false:未碰撞}
function MainScene:impactCheck(insertRotation)
     for i = 1,#self.allPin_rotation do
        if (insertRotation >= self.allPin_rotation[i][1]) and (insertRotation <= self.allPin_rotation[i][2]) then
            return true;
        end
     end
     return false;
end

-- 获取当前针角度
function MainScene:getPinRotation()
    local pin_node = cc.CSLoader:createNode(PinRes);
    local pin = self.uiHelp:seekChild(pin_node,"pin");
    local pin_x = pin:getContentSize().width/2;
    local pin_y = pin:getContentSize().height + math.abs(pin:getPositionY()) - pin_x;
    local tan = pin_x/pin_y;
    local radina = math.atan(tan);
    local angle =  math.ceil(180/(math.pi/radina));
    return angle *1.8;
end

-- 获取最大角度和最小角度
-- @ param rotation_num  1:{最大角度},2:{最小角度}
function MainScene:getRotationNum(rotation_num,insertRotation)
     --获取当前针的角度
    local curRotation = self:getPinRotation();
    local temp_num = 0;
    if rotation_num == 1 then
        temp_num = insertRotation + curRotation;
    else
        temp_num = insertRotation - curRotation;
    end 
    return temp_num;
end

-- 将角度区间记录在数组中
-- @param insertRotation 插入的角度
function MainScene:saveRotation(insertRotation)
    local temp_arr = {};
    local curRotation_max = self:getRotationNum(1,insertRotation);
    local curRotation_min = self:getRotationNum(2,insertRotation);
    table.insert(temp_arr,table.getn(temp_arr) + 1,curRotation_min);
    table.insert(temp_arr,table.getn(temp_arr) + 1,curRotation_max);
    table.insert(self.allPin_rotation,table.getn(self.allPin_rotation) + 1,temp_arr);
end

-- 设置道具数量
-- @param  glassTime_num 沙漏数量
-- @param  scalpel_num   手术刀数量
function MainScene:setPropNum(glassTime_num,scalpel_num)
    self.uiHelp:seekChild(self,"hourglass_num"):setString(glassTime_num);
    self.uiHelp:seekChild(self,"pin_num"):setString(scalpel_num);
end

--返回主界面
function MainScene:toMainScene()
    self.uiHelp:seekChild(self,"playSucc"):setVisible(false);
    self.uiHelp:seekChild(self,"playFail"):setVisible(false);

    local newScene = require("views.StartScene"):new():createScene();
    local transition = cc.TransitionFade:create(0.5,newScene);
    cc.Director:getInstance():replaceScene(transition);
end

--下一关
function MainScene:toNextStage()
    self.uiHelp:seekChild(self,"mask_layer"):setVisible(false);
    self:init();
end

--重新开始
function MainScene:toRestart()
    self.uiHelp:seekChild(self,"mask_layer"):setVisible(false);
    self:init();
end

--不复活
function MainScene:resurrection_no()
    self.uiHelp:seekChild(self,"resurrection"):setVisible(false);
    local layer = self.uiHelp:seekChild(self,"playFail");
    self.uiHelp:setTanChuang(self,layer);
end

--玩家复活
function MainScene:toResurrection()
    if not self.playState then
        local face = self.uiHelp:seekChild(self,"expression_img");
        face:loadTexture("failure-zc.png",UI_TEX_TYPE_PLIST);
        self.uiHelp:seekChild(self,"mask_layer"):setVisible(false);
        self.uiHelp:seekChild(self,"playFail"):setVisible(false);
        self.uiHelp:seekChild(self,"resurrection"):setVisible(false);
        self:confirmClose();
        self.playState = true;
    end
end

--点击复活
function  MainScene:resurrection_yes()
    local tag =  1;
    if GameConfig.confirm ==  0 then
        SDKManager.pay("2000","3",function(result)
           self:toResurrection();
        end);
    else 
        local layer = self.uiHelp:seekChild(self,"confirm_bnt");
        self.uiHelp:confirmTanChuang(self,layer,tag);
    end
end

--关闭领取界面
function MainScene:rev_no()
    self.uiHelp:seekChild(self,"mask_layer"):setVisible(false);
    self.uiHelp:seekChild(self,"rev_gift"):setVisible(false);
end

--玩家领取礼包
function MainScene:toRevGift()
    self.cur_userData = self.userData:getUserData();
    cc.UserDefault:getInstance():setIntegerForKey("revGift",1);
    cc.UserDefault:getInstance():setIntegerForKey("glassTime", tonumber(self.cur_userData.glassTime) + 3);
    cc.UserDefault:getInstance():setIntegerForKey("scalpel", tonumber(self.cur_userData.scalpel) + 10);
    self.cur_userData = self.userData:getUserData();
    self:setPropNum(self.cur_userData.glassTime,self.cur_userData.scalpel);
    self:confirmClose();
    self.uiHelp:seekChild(self,"mask_layer"):setVisible(false);
    self.uiHelp:seekChild(self,"rev_gift"):setVisible(false);
    self:confirmClose();
end

--点击领取礼包
function MainScene:rev_yes()
    local tag = 2;
    if GameConfig.confirm ==  0 then
        SDKManager.pay("2000","2",function(result)
            self:toRevGift();
        end);
    else 
        local layer = self.uiHelp:seekChild(self,"confirm_bnt");
        self.uiHelp:confirmTanChuang(self,layer,tag);
    end
end

--使用沙漏
function  MainScene:hourglass_use()
    self.cur_userData = self.userData:getUserData();
    if tonumber(self.cur_userData.glassTime) > 0 then
        if self.isGlassTime then
            self.isGlassTime = false;
            cc.UserDefault:getInstance():setIntegerForKey("glassTime", tonumber(self.cur_userData.glassTime) - 1);
            self:setRotationSpeed(self.cur_userData.curStage,true);
            self.cur_userData = self.userData:getUserData();
            self:setPropNum(self.cur_userData.glassTime,self.cur_userData.scalpel);
        end
    else 
        local layer = self.uiHelp:seekChild(self,"rev_glasstime");
        self.uiHelp:setTanChuang(self,layer);
    end
end

--使用手术刀
function  MainScene:pin_use()
    self.cur_userData = self.userData:getUserData();
    if tonumber(self.cur_userData.scalpel) > 0 then
        cc.UserDefault:getInstance():setIntegerForKey("scalpel", tonumber(self.cur_userData.scalpel) - 1);
        self:delReadyPin();
        self.cur_userData = self.userData:getUserData();
        self:setPropNum(self.cur_userData.glassTime,self.cur_userData.scalpel);
    else 
        local layer = self.uiHelp:seekChild(self,"rev_pin");
        self.uiHelp:setTanChuang(self,layer);
    end
end

--关闭漏斗界面
function  MainScene:revglass_no()
    self.uiHelp:seekChild(self,"mask_layer"):setVisible(false);
    self.uiHelp:seekChild(self,"rev_glasstime"):setVisible(false);
end

--增加漏斗
function MainScene:addRevglass()
    self.cur_userData = self.userData:getUserData();
    cc.UserDefault:getInstance():setIntegerForKey("glassTime", tonumber(self.cur_userData.glassTime) + 3);
    self.cur_userData = self.userData:getUserData();
    self:setPropNum(self.cur_userData.glassTime,self.cur_userData.scalpel);
    self.uiHelp:seekChild(self,"mask_layer"):setVisible(false);
    self.uiHelp:seekChild(self,"rev_glasstime"):setVisible(false);
    self:confirmClose();
end

--领取沙漏
function MainScene:revglass_yes()
    local tag = 3;
    if GameConfig.confirm ==  0 then
        SDKManager.pay("2000","1",function(result)
            self:addRevglass();
        end);
    else 
        local layer = self.uiHelp:seekChild(self,"confirm_bnt");
        self.uiHelp:confirmTanChuang(self,layer,tag);
    end
end

--关闭手术刀界面
function  MainScene:revpin_no()
    self.uiHelp:seekChild(self,"mask_layer"):setVisible(false);
    self.uiHelp:seekChild(self,"rev_pin"):setVisible(false);
end

--增加手术刀
function MainScene:addRevpin()
    self.cur_userData = self.userData:getUserData();
    cc.UserDefault:getInstance():setIntegerForKey("scalpel", tonumber(self.cur_userData.scalpel) + 10);
    self.cur_userData = self.userData:getUserData();
    self:setPropNum(self.cur_userData.glassTime,self.cur_userData.scalpel);
    self.uiHelp:seekChild(self,"mask_layer"):setVisible(false);
    self.uiHelp:seekChild(self,"rev_pin"):setVisible(false);
    self:confirmClose();
end

--领取手术刀
function MainScene:revpin_yes()
   local tag = 4;
    if GameConfig.confirm ==  0 then
        SDKManager.pay("2000","0",function(result)
            self:addRevpin();
        end);
    else 
        local layer = self.uiHelp:seekChild(self,"confirm_bnt");
        self.uiHelp:confirmTanChuang(self,layer,tag);
    end
end

--确认购买
function MainScene:confirmSure(sender)
    local arr = {"3","2","1","0"};
    SDKManager.pay("2000",arr[tonumber(sender:getTag())],function(result)
        if sender:getTag() == 1 then 
            self:toResurrection();
        elseif sender:getTag() == 2 then
            self:toRevGift();
        elseif sender:getTag() == 3 then
            self:addRevglass();
        elseif sender:getTag() == 4 then
            self:addRevpin();
        end
    end);
end

--二次确认取消
function MainScene:confirmClose()
    self.uiHelp:seekChild(self,"mask_layer2"):setVisible(false);
    self.uiHelp:seekChild(self,"confirm_bnt"):setVisible(false);
end

--开关声音
function MainScene:voice_bnt()
    self.userData = require("userData"):new();
    self.cur_userData = self.userData:getUserData();
    if AudioEngine.isMusicPlaying () then
        cc.UserDefault:getInstance():setIntegerForKey("sound",1);
        AudioEngine.stopMusic();
        self.uiHelp:seekChild(self, "voice_bnt"):loadTexture(closeMusicRes,UI_TEX_TYPE_PLIST);
    else
        cc.UserDefault:getInstance():setIntegerForKey("sound",2);
        AudioEngine.playMusic(gameBgMusic,true);
        self.uiHelp:seekChild(self, "voice_bnt"):loadTexture(openMusicRes,UI_TEX_TYPE_PLIST);
    end
end


return MainScene
