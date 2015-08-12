LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDLIBS += -llog

LOCAL_MODULE    := stimulatepay
LOCAL_SRC_FILES := stimulatepay.c

include $(BUILD_SHARED_LIBRARY)
