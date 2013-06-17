LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_ARM_MODE := arm 


LOCAL_CFLAGS += -I$(LOCAL_PATH)/boost/include/
LOCAL_LDLIBS += -L$(LOCAL_PATH)/boost/lib/ -lboost_system -lboost_date_time -lboost_thread_pthread

LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -frtti

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/ClientIoHandler.cpp ../../Classes/ClientPlayer.cpp ../../Classes/SgsClient.cpp \
                   ../../Classes/Card.cpp ../../Classes/Engine.cpp \
                   ../../Classes/General.cpp ../../Classes/Player.cpp ../../Classes/Skill.cpp ../../Classes/Structs.cpp \
                   ../../Classes/ChooseGeneralLayer.cpp ../../Classes/TableAnimationLayer.cpp \
                   ../../Classes/AsioClient.cpp ../../Classes/AsioServer.cpp ../../Classes/Connection.cpp ../../Classes/ConnectionManager.cpp ../../Classes/IoHandler.cpp ../../Classes/IoServicePool.cpp ../../Classes/XmlIoHandler.cpp \
                   ../../Classes/ExpPattern.cpp ../../Classes/Package.cpp ../../Classes/Standard.cpp ../../Classes/StandardCards.cpp ../../Classes/StandardGenerals.cpp \
                   ../../Classes/AboutUsScene.cpp ../../Classes/LoadingScene.cpp ../../Classes/LobbyScene.cpp ../../Classes/MainScene.cpp ../../Classes/MQScene.cpp ../../Classes/TableScene.cpp ../../Classes/WaitingRoomScene.cpp \
                   ../../Classes/CardSelector.cpp ../../Classes/PlayerSelector.cpp ../../Classes/SelectorManager.cpp\
                   ../../Classes/Room.cpp ../../Classes/ServerPlayer.cpp ../../Classes/SgsServer.cpp \
                   ../../Classes/CardItem.cpp ../../Classes/CCTouchNode.cpp ../../Classes/CursorTextField.cpp ../../Classes/PlayerBoard.cpp ../../Classes/PlayerControllerBoard.cpp ../../Classes/PlayerUI.cpp ../../Classes/SgsDialog.cpp \
                   ../../Classes/AsioTimer.cpp ../../Classes/Config.cpp ../../Classes/SgsUtil.cpp ../../Classes/StringResource.cpp  \
                   ../../Classes/pugixml.cpp ../../Classes/Tag.cpp \
                   ../../Classes/AnimationManager.cpp ../../Classes/HelloWorldScene.cpp ../../Classes/PacketFactory.cpp ../../Classes/UIThreadMQ.cpp  

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes           

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static 
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)
