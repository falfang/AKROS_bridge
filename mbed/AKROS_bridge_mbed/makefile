PROJECT         := AKROS_bridge_mbed
DEVICES         := NUCLEO_F446RE 	NUCLEO_L476RG
GCC4MBED_DIR    := $(GCC4MBED_DIR)
USER_LIBS       := !../ros_lib ../ros_lib/BufferedSerial\
					../include

NO_FLOAT_SCANF  := 1
NO_FLOAT_PRINTF := 1

include $(GCC4MBED_DIR)/build/gcc4mbed.mk
