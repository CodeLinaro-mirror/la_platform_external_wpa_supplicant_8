LIBRARY=libqti_aidl_sock.so.0

CUR_DIR = $(shell pwd)
INC_DIR = $(CUR_DIR)/include
SRC_DIR = $(CUR_DIR)/src

CFLAGS += -c -fPIC
CFLAGS += -Wall -Werror -Wno-unused-function -Wno-ignored-attributes
CPPFLAGS += -I$(INC_DIR)
CPPFLAGS += -DENABLE_LOG
LDFLAGS += -lqti_log_util
LDFLAGS += -shared -Wl,-soname,$(LIBRARY)

SRCS += $(SRC_DIR)/aidl_sock.c

COBJECTS := $(SRCS:.c=.o)

$(LIBRARY): $(COBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(COBJECTS): %.o: %.c
	$(CC) $< -o $@  $(CPPFLAGS) $(CFLAGS)

clean:
	rm -rf *.o *.so*