LOCAL_PATH := $(call my-dir)

#include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

define walk
    $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef
 
ALLFILES = $(call walk, $(LOCAL_PATH)/../../Classes)

FILE_LIST := hellocpp/main.cpp
FILE_LIST += $(filter %.cpp, $(ALLFILES))
FILE_LIST += $(filter %.c, $(ALLFILES))

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../cocos2d/cocos
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../cocos2d/cocos/gui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../cocos2d/cocos/editor-support
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../cocos2d/cocos/editor-support/cocostudio
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/NDKHelper
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/jansson

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_gui_static

#umeng
#LOCAL_SHARED_LIBRARIES := mobclickcpp_shared
#umeng

include $(BUILD_SHARED_LIBRARY)

$(call import-module,2d)
$(call import-module,audio/android)
$(call import-module,Box2D)
$(call import-module,gui)
$(call import-module,editor-support/cocostudio)

#umeng
#$(call import-module,libmobclickcpp)
#umeng
