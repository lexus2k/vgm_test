#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

# OVERRIDE_IDF_VER ?= "v3.1.2"
# OVERRIDE_IDF_VER ?= "v3.2-beta1"

ifeq ($(OVERRIDE_IDF_VER),"v3.1.2")
    export IDF_PATH:=~/esp/esp-idf-v3.1.2
    CPPFLAGS += -DIDF_VERSION=030102
else ifeq ($(OVERRIDE_IDF_VER),"v3.3-beta1")
    export IDF_PATH:=~/esp/esp-idf-v3.3-beta1
    CPPFLAGS += -DIDF_VERSION=030300
else ifeq ($(OVERRIDE_IDF_VER),"v3.2-beta1")
    export IDF_PATH:=~/esp/esp-idf-v3.2-beta1
    CPPFLAGS += -DIDF_VERSION=030200
else
    CPPFLAGS += -DIDF_VERSION=030200
endif

PROJECT_VER = "v0.1"
PROJECT_NAME := vgm_test
EXTRA_COMPONENT_DIRS := ./components
# COMPONENTS=freertos esp32 newlib esptool_py nvs_flash spi_flash log tcpip_adapter lwip main xtensa-debug-module driver bt

include $(IDF_PATH)/make/project.mk

