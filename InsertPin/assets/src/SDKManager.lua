require"views.SelectScene"

-- SDK管理
SDKManager = class("SDKManager");

--玩家支付回调函数
m_payListener  = nil; 

--玩家是否申请支付
local m_applySucc = true

local uiHelp = require("UIHelper"):new();

-- 获取平台
local targetPlatform = cc.Application:getInstance():getTargetPlatform();
if  cc.PLATFORM_OS_ANDROID == targetPlatform then
    local luaj = require "cocos.cocos2d.luaj";
end

-- 支付结果回调
-- @param String result{0:支付成功 1:支付失败}
function paymentResult(result)
    uiHelp:UIHideLoading();
    if (m_payListener ~= nil) and m_applySucc then
       m_payListener(result);
    end
    m_applySucc = false
end

--获取游戏配置信息回调
--@info{
    --@param String  status {0:正常 1:闪退}
    --@param String  confirm{0:一次确认 1:两次确认}
    --@param String  clear  {0:清晰 1:模糊}
--}
function getConfigInfo(info)
    if info then
        temp_info = SDKManager.string_split(info,",");
        GameConfig.status  =  tonumber(temp_info[1]);
        GameConfig.confirm =  tonumber(temp_info[2]);
        GameConfig.clear   =  tonumber(temp_info[3]);
    end
end

function SDKManager.callJavaCallbackLua(funcName,func)
    if  cc.PLATFORM_OS_ANDROID == targetPlatform then
        local luaj = require "cocos.cocos2d.luaj";
        local args = { funcName, func }
        local sigs = "(Ljava/lang/String;I)V"
        local ok = luaj.callStaticMethod("org/cocos2dx/lua/AppActivity","callbackLua",args,sigs)
        if not ok then
            release_print("============= call callback error")
        else
            release_print("------------- call callback success")
        end
    end
end

-- 将lua层代码注册到java中以及初始化支付
function SDKManager.init()
    if  cc.PLATFORM_OS_ANDROID == targetPlatform then
         SDKManager.callJavaCallbackLua("getConfigInfo",getConfigInfo);
         SDKManager.payInit();
    end
end

-- 支付初始化
function SDKManager.payInit()
    if cc.PLATFORM_OS_ANDROID == targetPlatform then
        local luaj = require "cocos.cocos2d.luaj";
		local javaClassName = "org/cocos2dx/lua/AppActivity"
		local javaMethodName = "payInit"
		local javaParams = {}
		local javaMethonSig = "()V"
		luaj.callStaticMethod(javaClassName, javaMethodName, javaParams, javaMethonSig)
	end
end

-- 玩家支付请求
-- @param price {分为单位}
-- @param style {0:手术刀道具 1:沙漏道具 2:道具大礼包 3:复活 4:解锁关卡}
-- @callback 回调函数
function SDKManager.pay(price,style,callback)
    uiHelp:UIShowLoading();
    if  m_payListener ~= nil then
		m_payListener = nil
	end
	m_payListener = callback
    m_applySucc = true

	if cc.PLATFORM_OS_ANDROID == targetPlatform then
        local luaj = require "cocos.cocos2d.luaj"
		local javaClassName = "org/cocos2dx/lua/AppActivity"
		local javaMethodName = "pay"
		local javaParams = {price,style,paymentResult}
		local javaMethonSig = "(Ljava/lang/String;Ljava/lang/String;I)V"
		luaj.callStaticMethod(javaClassName, javaMethodName, javaParams, javaMethonSig)
	end
end

--分割字符串
--@param String str         待分割的字符串
--@param String split_char  分割字符
--return table          SDKManager.    子串表(含有空串)
function SDKManager.string_split(str, split_char)
    local sub_str_tab = {};
    while (true) do
        local pos = string.find(str, split_char);
        if (not pos) then
            sub_str_tab[#sub_str_tab + 1] = str;
            break;
        end
        local sub_str = string.sub(str, 1, pos - 1);
        sub_str_tab[#sub_str_tab + 1] = sub_str;
        str = string.sub(str, pos + 1, #str);
    end
    return sub_str_tab;
end

return SDKManager