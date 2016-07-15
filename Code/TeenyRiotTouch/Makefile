# Parameters for compiling
#
#	 add tap
#	$ brew tap sudar/arduino-mk
#	 to install the last stable release
#
#	$ brew install arduino-mk
#	 to install the development version
#
#	$ brew install --HEAD arduino-mk
#	

#Arduino makefile info
ARDUINO_DIR     = /Applications/Development/Arduino.app/Contents/Java
ARDMK_DIR       = /usr/local/opt/arduino-mk
OBJDIR          = ../build
ISP_PROG     	= usbasp
AVRDUDE         = /Users/xcorex/Documents/Arduino/hardware/teenyriot/avr/tools/avrdude/macosx/avrdude

# Board Information
ARCHITECTURE 			= avr
ALTERNATE_CORE			= teenyriot
BOARD_TAG			= teenyriotusbtiny
ARDUINO_PORT			= 

CPPFLAGS        += -DARDUINO_AVR_TEENYRIOT

# User Library Path
USER_LIB_PATH   = ../libraries
#ARDUINO_PLATFORM_LIB_PATH =
#ARDUINO_LIBS =TeenyMidi TeenyTouch TeenySerial Filter TeenyKeyboard
ARDUINO_LIBS =TeenyTouch TeenySerial Filter TeenyKeyboard
ARDUINO_QUIET = 1

# Auto-detected from boards.txt
# MCU             = atmega328p
# F_CPU           = 16000000
# AVRDUDE_ARD_PROGRAMMER  = stk500v2
# AVRDUDE_ARD_BAUDRATE    = 115200


# Arduino makefile
include $(ARDMK_DIR)/Arduino.mk
