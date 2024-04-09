# This Makefile is an edited version of the file found here:

# https://github.com/Steveice10/FBI and has been released under MIT Licence as

# follows:

#

# Copyright � 2015 Steveice10

#

# Permission is hereby granted, free of charge, to any person obtaining a copy

# of this software and associated documentation files (the "Software"), to deal

# in the Software without restriction, including without limitation the rights

# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

# copies of the Software, and to permit persons to whom the Software is

# furnished to do so, subject to the following conditions:

#

# The above copyright notice and this permission notice shall be included in

# all copies or substantial portions of the Software.

#

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

# SOFTWARE.



# TARGET #



TARGET := 3DS

LIBRARY := 0



ifeq ($(TARGET),3DS)

    ifeq ($(strip $(DEVKITPRO)),)

        $(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro")

    endif



    ifeq ($(strip $(DEVKITARM)),)

        $(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")

    endif

endif



# COMMON CONFIGURATION #



NAME := PCT-3DS



BUILD_DIR := build

OUTPUT_DIR := output

INCLUDE_DIRS := source/include extlibs

SOURCE_DIRS := source extlibs



EXTRA_OUTPUT_FILES :=



LIBRARY_DIRS := $(DEVKITPRO)/libctru $(DEVKITPRO)/portlibs/3ds

LIBRARIES := ctru citro2d citro2dd citro3d citro3dd ctrud lzma z m bz2

#LIBRARY_DIRS += $(DEVKITPRO)/libctru
#LIBRARIES += citro3d ctru citro2d

#REMOTE_IP := 192.168.1.222

BUILD_FLAGS := -Wall -Wextra -D__3DS__ -Iextlibs

RUN_FLAGS :=

VERSION_MAJOR := 0

VERSION_MINOR := 1

VERSION_MICRO := 0



# 3DS CONFIGURATION #



TITLE := $(NAME)

DESCRIPTION := Tot PCT a la teva 3DS

AUTHOR := Projecte 'Ce Trencada'

PRODUCT_CODE := CTR-E-CPCT

UNIQUE_ID := 0x437FE


#CPU_SPEED := 804MHz

ROMFS_DIR := romfs

BANNER_AUDIO := resources/audio.wav

BANNER_IMAGE := resources/banner.png

ICON := resources/icon.png



# INTERNAL #



include buildtools/make_base
