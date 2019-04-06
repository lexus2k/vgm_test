#
#    This file is part of I2S demo player for ESP32.
#    Copyright (C) 2019  Alexey Dynda.
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

PROJECT_VER = "v0.1"
PROJECT_NAME := vgm_test
EXTRA_COMPONENT_DIRS := ./components

include $(IDF_PATH)/make/project.mk

