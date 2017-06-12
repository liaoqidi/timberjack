local SelectSceneRes = "res/selectGame.csb";      -- 选择关卡场景
local StagePointRes  = "res/stageItem.csb";       -- 关卡模板
local onePageRes     = "levelLayer-o1.png";       -- 页面选中样式
local twoPageRes     = "levelLayer-o2.png";       -- 页面未选中样式
local lockRes        = "levelLayer-y.png";        -- 未开锁
local openRes        = "levelLayer-open.png";     -- 已开锁
local okRes          = "levelLayer-ok.png";       -- 已通关
local openMusicRes   = "publicLayer- volume1.png"; -- 打开声音图标
local closeMusicRes  = "publicLayer- volume2.png"; -- 关闭声音图标

SelectScene = class("SelectScene", function()
    return cc.Layer:create()
end)

-- 创建场景
function SelectScene:createScene()
    local scene = cc.Scene:create()
    scene:addChild(self)
    return scene
end

-- 构造函数
function SelectScene:ctor()
    -- 注册enter与exit事件
    self:registerScriptHandler(function(state)
        if state == "enter" then
            self:_onEnter()
        elseif state == "exit" then
            self:_onExit()
        end
    end)
    self:addChild(cc.CSLoader:createNode(SelectSceneRes));
    self.uiHelp   = require("UIHelper"):new();
    self.userData = require("userData"):new();
    self.cur_userData = self.userData:getUserData();
    --设置声音图标
    if tonumber(self.cur_userData.sound) ~= 2 then
        self.uiHelp:seekChild(self, "voice_bnt"):loadTexture(closeMusicRes,UI_TEX_TYPE_PLIST);
    end

    --每页关卡的数量
    self.stage_num = 35;
    --总的关卡数量
    self.allStage_num = 50;
    --当前关卡页数
    self.cur_page = 1;
    --最后页数
    self.end_page = math.ceil(self.allStage_num/self.stage_num);
    self.stage_panel = self.uiHelp:seekChild(self,"Panel_stage");
    --每次解锁关卡数
    self.open_num = 10;
    --点击事件
    self.uiHelp:seekChild(self, "left_bnt"):addClickEventListener(handler(self,self.leftPage));
    self.uiHelp:seekChild(self, "right_bnt"):addClickEventListener(handler(self,self.rightPage));
    self.uiHelp:seekChild(self, "openLock_bnt"):addClickEventListener(handler(self,self.openLock));
    self.uiHelp:seekChild(self, "openLock_close"):addClickEventListener(handler(self,self.openLock_close));
    self.uiHelp:seekChild(self, "voice_bnt"):addClickEventListener(handler(self,self.openOrCloseVoice));   
    self.uiHelp:seekChild(self,"confirmSure"):addClickEventListener(handler(self,self.confirmSure));
    self.uiHelp:seekChild(self,"confirmClose"):addClickEventListener(handler(self,self.confirmClose));
    --设置滑动翻页
    local listener = cc.EventListenerTouchOneByOne:create();
    listener:setSwallowTouches(false);
    listener:registerScriptHandler(handler(self,self.onTouchBegan),cc.Handler.EVENT_TOUCH_BEGAN );
    listener:registerScriptHandler(handler(self,self.onTouchEnded),cc.Handler.EVENT_TOUCH_ENDED );
    local eventDispatcher = self:getEventDispatcher();
    eventDispatcher:addEventListenerWithSceneGraphPriority(listener,self);
    --默认设置第一页是开始关卡
    self:toSetStagePos();
end

-- 进入场景
function SelectScene:_onEnter()
end
-- 退出场景
function SelectScene:_onExit()  
end

--关卡左翻
function SelectScene:leftPage()
    self.cur_page = self.cur_page -1;
    if self.cur_page < 1 then
        self.cur_page = 1;
    end
    self:showStage(self.cur_page);
end

--关卡右翻
function SelectScene:rightPage()
    self.cur_page = self.cur_page + 1;
    if self.cur_page > self.end_page then
        self.cur_page = self.end_page;
    end
    self:showStage(self.cur_page);
end

--解锁
function SelectScene:toOpenLock()
    local endStage_num = 0;
    --当前最后解锁的关卡
    local curStage = self:getOpenStageNum();
    if (self.open_num + curStage) > self.allStage_num then
        endStage_num = self.allStage_num;
    else
        endStage_num = self.open_num + curStage;    
    end

    cc.UserDefault:getInstance():setIntegerForKey("curStage",endStage_num);
    for i = curStage,endStage_num do
        cc.UserDefault:getInstance():setIntegerForKey(string.format("stage_%d",i),2);
    end
    self.uiHelp:seekChild(self,"openLock"):setVisible(false);
    self.uiHelp:seekChild(self,"mask_layer"):setVisible(false);
    self:confirmClose();
    self:showStage(self.cur_page);
end


--解锁关卡
function SelectScene:openLock()
    if GameConfig.confirm ==  0 then
        SDKManager.pay("2000","4",function(result)
            self:toOpenLock();
        end);
    else 
        local layer = self.uiHelp:seekChild(self,"confirm_bnt");
        self.uiHelp:confirmTanChuang(self,layer);
    end
end

--确认购买
function SelectScene:confirmSure()
    SDKManager.pay("2000","4",function(result)
        self:toOpenLock();
    end);
end

--二次确认取消
function SelectScene:confirmClose()
    self.uiHelp:seekChild(self,"mask_layer2"):setVisible(false);
    self.uiHelp:seekChild(self,"confirm_bnt"):setVisible(false);
end



--关闭解锁关卡
function  SelectScene:openLock_close()
     self.uiHelp:seekChild(self,"openLock"):setVisible(false);
     self.uiHelp:seekChild(self,"mask_layer"):setVisible(false);
end

--进入游戏
function  SelectScene:enterGame(sender)
    self.cur_userData = self.userData:getUserData();
    if self.cur_userData.isPass[sender:getTag()] ~= 1 then
        cc.UserDefault:getInstance():setIntegerForKey("curStage", sender:getTag());        
        local newScene = require("views.MainScene"):new():createScene();
        local transition = cc.TransitionFade:create(0.5,newScene);
        cc.Director:getInstance():replaceScene(transition);
    else
        local openLock = self.uiHelp:seekChild(self,"openLock");
        self.uiHelp:setTanChuang(self,openLock);
    end
end

--开关声音
function SelectScene:openOrCloseVoice(sender)
    self.userData = require("userData"):new();
    self.cur_userData = self.userData:getUserData();
    if AudioEngine.isMusicPlaying () then
        cc.UserDefault:getInstance():setIntegerForKey("sound",1);
        AudioEngine.stopMusic();
        self.uiHelp:seekChild(self, "voice_bnt"):loadTexture(closeMusicRes,UI_TEX_TYPE_PLIST);
    else
        cc.UserDefault:getInstance():setIntegerForKey("sound",2);
        AudioEngine.playMusic(bgMusic,true);
        self.uiHelp:seekChild(self, "voice_bnt"):loadTexture(openMusicRes,UI_TEX_TYPE_PLIST);
    end
end

--滑动开始
function SelectScene:onTouchBegan(touch,event)
    self.firstX = touch:getLocation().x;
    return true;  
end

--滑动结束
function SelectScene:onTouchEnded(touch,event)
    local endX = self.firstX - touch:getLocation().x;
    --大于零是从右向左
    if math.abs(endX) > 100 then
        if endX > 0 then
            self:rightPage();
        else
            self:leftPage();
        end
    end
end

--设置关卡位置
function SelectScene:toSetStagePos()
    local stage = cc.CSLoader:createNode(StagePointRes);
    local item = self.uiHelp:seekChild(stage,"stage");
    --初始Y坐标
    local posY = self.stage_panel:getPositionY() + item:getContentSize().height;
    --当前第几个  
    local curPosX = 1;
    local curPosY = 1;
    --偏移量
    local moveX = item:getContentSize().width;
    local moveY = item:getContentSize().height * 1.2;
    for i = 1,self.stage_num do
        local temp_item = cc.CSLoader:createNode(StagePointRes);
        temp_item:setPosition(cc.p((curPosX - 1)* moveX,(posY - (curPosY-1) * moveY)));
        --给关卡增加点击事件
        self.uiHelp:seekChild(temp_item,"stage_bg"):addClickEventListener(handler(self,self.enterGame));
        self.stage_panel:addChild(temp_item);
        curPosX = curPosX + 1;
        if i%5 == 0 then
            curPosY = curPosY +1;
            curPosX = 1;
        end
    end
    --默认显示第一页
    self:showStage(1);
end

--显示关卡
--@param pageIndex 第几页
function SelectScene:showStage(pageIndex)
    self.cur_userData = self.userData:getUserData();
    --当前页面关卡数量
    local num = 0;
    if pageIndex == self.end_page then
        num = self.allStage_num%self.stage_num;
    else
        num = self.stage_num;
    end
    --当前页面开始关卡
    local curItems = self.stage_num * (pageIndex-1);   
    local all_stages = self.stage_panel:getChildren();
    for i = 1,self.stage_num do
        if pageIndex == self.end_page then
            if i > num then
                all_stages[i]:setVisible(false);
                self.uiHelp:seekChild(self,"onePage"):loadTexture(onePageRes,UI_TEX_TYPE_PLIST);
                self.uiHelp:seekChild(self,"twoPage"):loadTexture(twoPageRes,UI_TEX_TYPE_PLIST);
            end
        else
            all_stages[i]:setVisible(true);
            self.uiHelp:seekChild(self,"onePage"):loadTexture(twoPageRes,UI_TEX_TYPE_PLIST);
            self.uiHelp:seekChild(self,"twoPage"):loadTexture(onePageRes,UI_TEX_TYPE_PLIST);            
        end
        curItems = curItems + 1;
        local stage_bg = self.uiHelp:seekChild(all_stages[i],"stage_bg");
        stage_bg:setTag(curItems);
        if self.cur_userData.isPass[curItems] == 1 then
            self.uiHelp:seekChild(all_stages[i],"stage_font"):setVisible(false);
            stage_bg:loadTexture(lockRes,UI_TEX_TYPE_PLIST);
        else
            self.uiHelp:seekChild(all_stages[i],"stage_font"):setVisible(true);
            self.uiHelp:seekChild(all_stages[i],"stage_font"):setString(curItems);
            if self.cur_userData.isPass[curItems] == 2 then
                stage_bg:loadTexture(openRes,UI_TEX_TYPE_PLIST);
            elseif self.cur_userData.isPass[curItems] == 3 then
                stage_bg:loadTexture(okRes,UI_TEX_TYPE_PLIST);
            end
        end
    end
end

--获取当前已经解锁的关卡
function SelectScene:getOpenStageNum()
     self.cur_userData = self.userData:getUserData();
     local curStage_end = 0;
     for i=1,#self.cur_userData.isPass do
        if self.cur_userData.isPass[i] ~= 1 then
            curStage_end = curStage_end + 1;
        end
     end
     return curStage_end;
end

return SelectScene
