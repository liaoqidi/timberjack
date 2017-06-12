local UIHelper = class("UIHelper")
local LastShowRoot    = nil;              --通用框和Loading的节点
local LoadingRes      = "res/loading.csb"; 

--创建一个UIHelper
function UIHelper:ctor()
	self._data = {}
end
--递归查找控件
--@parent node   查找的控件
--@name   string 查找的名称
function UIHelper:seekChild(parent,name)
	local vec = require("MyVector"):new()
    vec:push_back(parent)
    while vec:size() > 0 do
        local Node = vec:at(1)
        vec:erase(Node)
        if Node then
           local children = Node:getChildren()
           for i= 1, #children do
                local child = children[i]
                if child:getName() == name then
                    return child
                else
                    vec:push_back(child)
                end
           end
        end
    end
    return nil
end

-- 设置抖动弹窗
-- @param scene   场景对象
-- @param layer   弹窗对象
-- @param clear   扣费提示是否清晰{0:清晰 1:模糊}
function UIHelper:setTanChuang(scene,layer,clear)
    self:seekChild(scene,"mask_layer"):setVisible(true);
    if GameConfig.clear == 0 then
        self:seekChild(scene,"tishi"):setOpacity(255);
    else
        self:seekChild(scene,"tishi"):setOpacity(76.5);
    end

    local function action()
        layer:setVisible(true);
    end
    local scale  =  cc.ScaleTo:create(0.01, 0);
    local scale1  =  cc.ScaleTo:create(0.2, 1.15);
    local scale2 =  cc.ScaleTo:create(0.1, 1);
    layer:runAction(cc.Sequence:create(scale,cc.CallFunc:create(action),scale1,scale2));
end

-- 二次确认
-- @param scene   场景对象
-- @param layer   弹窗对象
function UIHelper:confirmTanChuang(scene,layer,tag)
    if tag then
        self:seekChild(layer,"confirmSure"):setTag(tag);
    end
    self:seekChild(scene,"mask_layer2"):setVisible(true);
    local function action()
        layer:setVisible(true);
    end
    local scale  =  cc.ScaleTo:create(0.01, 0);
    local scale1  =  cc.ScaleTo:create(0.2, 1.15);
    local scale2 =  cc.ScaleTo:create(0.1, 1);
    layer:runAction(cc.Sequence:create(scale,cc.CallFunc:create(action),scale1,scale2));
end

--显示等待界面
function UIHelper:UIShowLoading()
    if LastShowRoot then
        LastShowRoot:removeFromParent()
    end
    LastShowRoot = cc.CSLoader:createNode(LoadingRes)
    local point  = self:seekChild(LastShowRoot,"loadingPoint")
    local action = cc.RotateBy:create(1,360);
    point:runAction(cc.RepeatForever:create(action));
    cc.Director:getInstance():getRunningScene():addChild(LastShowRoot, 9998, 1000)
end

--关闭等待界面
function UIHelper:UIHideLoading()
     if LastShowRoot then
        LastShowRoot:removeFromParent()
        LastShowRoot = nil
     end
end


return UIHelper;