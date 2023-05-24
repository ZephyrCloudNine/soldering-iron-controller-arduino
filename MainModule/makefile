MAKE_DIR    	:= $(PWD)
CONFIG_FILE 	:= $(MAKE_DIR)/.arduino/config.json
DATA_PATH		:= $(MAKE_DIR)/.arduino
DOWNLOADS_PATH	:= $(MAKE_DIR)/.arduino/staging
USER_PATH		:= $(MAKE_DIR)/.arduino/Arduino

# ----- Setup for specific board types
FQBN        ?= adafruit:samd:adafruit_feather_m0
# ----- Board URL links for custom boards
ADDITIONAL_URL  :=https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
CORE			:=adafruit:samd
# ----- ---------------------
V          ?= 0
VFLAG      =

ifeq "$(V)" "1"
VFLAG      =-v
endif

ifndef SERIAL_DEV
  ifneq (,$(wildcard /dev/ttyUSB0))
    SERIAL_DEV = /dev/ttyUSB0
  else ifneq (,$(wildcard /dev/ttyUSB1))
    SERIAL_DEV = /dev/ttyUSB1
  else ifneq (,$(wildcard /dev/ttyACM0))
    SERIAL_DEV = /dev/ttyACM0
  else ifneq (,$(wildcard /dev/ttyACM1))
    SERIAL_DEV = /dev/ttyACM1
  else
	SERIAL_DEV = UNKNOWN
  endif
endif

BUILD_DIR  := $(subst :,.,build/$(FQBN))
SRC        := $(wildcard $(MAKE_DIR)/MainModule/*.ino)
HDRS       := $(wildcard $(MAKE_DIR)MainModule/*.h)
BIN        := $(BUILD_DIR)/MainModule.bin
ELF        := $(BUILD_DIR)/MainModule.elf

$(info FQBN       is [${FQBN}])
$(info IOT_NAME   is [${IOT_NAME}])
$(info OTA_PORT   is [${OTA_PORT}])
$(info OTA_PASS   is [${OTA_PASS}])
$(info V          is [${V}])
$(info VFLAG      is [${VFLAG}])
$(info MAKE_DIR   is [${MAKE_DIR}])
$(info BUILD_DIR  is [${BUILD_DIR}])
$(info SRC        is [${SRC}])
$(info HDRS       is [${HDRS}])
$(info BIN        is [${BIN}])
$(info SERIAL_DEV is [${SERIAL_DEV}])

# Compile & Upload  sketch
all: $(ELF) upload
.PHONY: all

#	Compile  sketch
compile: $(ELF)
.PHONY: compile

$(ELF): $(SRC) $(HDRS)
	arduino-cli compile -b $(FQBN) $(VFLAG) $(SRC) --config-file $(CONFIG_FILE) --output-dir $(BUILD_DIR)

#	Upload sketch to the board.
upload:
	@if [ ! -c $(SERIAL_DEV) ] ; \
	then echo "---> ERROR: Serial Device not available, please set the SERIAL_DEV environment variable" ; \
	else echo "---> Uploading sketch\n"; \
	arduino-cli upload -b $(FQBN) -p $(SERIAL_DEV) $(VFLAG) --input-dir $(BUILD_DIR) --config-file $(CONFIG_FILE); \
	fi

#	Clean the build directories. 
clean:
	@echo "---> Cleaning the build directory"
	rm -rf build

#	Install libraries from external-libs.txt
lib_install:
	@if [ -e external-libs.txt ]; \
	then while read -r i ; do echo ; \
	echo "---> Installing " '"'$$i'"' ; \
	arduino-cli lib install "$$i" --config-file $(CONFIG_FILE); \
	done < external-libs.txt ; \
	else echo "---> MISSING external-libs.txt file"; \
	fi

#	Remove all libraries listed in external-libs.txt
lib_remove:
	@if [ -e external-libs.txt ]; \
	then while read -r i ; do echo ; \
	echo "---> Removing " '"'$$i'"' ; \
	arduino-cli lib uninstall "$$i" --config-file $(CONFIG_FILE); \
	done < external-libs.txt ; \
	else echo "---> MISSING external-libs.txt file"; \
	fi

#	View the custom config file
view-config:
	arduino-cli config dump --config-file $(CONFIG_FILE);\

#	Creates a custom config file for the project and update indexes.
init:
	arduino-cli config init --overwrite --dest-file $(CONFIG_FILE); \
	arduino-cli config set directories.downloads $(DOWNLOADS_PATH) --config-file $(CONFIG_FILE); \
	arduino-cli config set directories.data $(DATA_PATH) --config-file $(CONFIG_FILE); \
	arduino-cli config set directories.user $(USER_PATH) --config-file $(CONFIG_FILE); \
	arduino-cli lib update-index --config-file $(CONFIG_FILE); \
	arduino-cli core update-index --config-file $(CONFIG_FILE); \
	arduino-cli core install arduino:avr --config-file $(CONFIG_FILE); \
	

#Install arduino core for other boards other than AVR
boards:
	arduino-cli core install arduino:samd 
	arduino-cli config add board_manager.additional_urls $(ADDITIONAL_URL)  --config-file $(CONFIG_FILE);
	arduino-cli core update-index --config-file $(CONFIG_FILE);
	arduino-cli core install $(CORE) --config-file $(CONFIG_FILE);

# copies over libs folder to working directly-makes testing libs easier
fresh:
	rm -rf src/temp
	mkdir -p src/temp
	cp -r ../libs src/temp;
	# cat src/temp/libs/WifiSettingsTemplate.h > src/temp/libs/WifiSettings.h
	# rm src/temp/libs/WifiSettingsTemplate.h
