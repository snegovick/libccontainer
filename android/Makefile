SHELL=/bin/bash
NDK_PATH = ~/ext_bin/android-ndk-r10e

LOCAL_PATH = $(shell pwd)

all:
	@echo "Local path: $(LOCAL_PATH)"
	pushd jni; \
	NDK_PATH=$(NDK_PATH) $(NDK_PATH)/ndk-build; \
	popd;

clean:
	rm -rf libs obj
