LOCAL_PATH := $(call my-dir)
#========================================
include $(CLEAR_VARS)

OpenCV_INSTALL_MODULES := on
OPENCV_CAMERA_MODULES := on
OPENCV_LIB_TYPE := STATIC
include $(LOCAL_PATH)/../opencv/jni/OpenCV.mk

LOCAL_MODULE    := dlib4Android

LOCAL_C_INCLUDES :=  \
    $(LOCAL_PATH)/../opencv/jni/include \
    $(LOCAL_PATH)

LOCAL_SRC_FILES := dlib/entropy_decoder/entropy_decoder_kernel_2.cpp \
				   dlib/base64/base64_kernel_1.cpp \
				   dlib4Android.cpp 

LOCAL_LDLIBS := -lm -llog -ldl -lz

LOCAL_CPPFLAGS += -fexceptions -frtti -std=c++11   

LOCAL_ARM_MODE := arm
ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
LOCAL_ARM_NEON := true
endif

LOCAL_CFLAGS += -pie -fPIE
#LOCAL_LDFLAGS += -pie -fPIE

include $(BUILD_SHARED_LIBRARY)

#------------------------------
