CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -framework CoreGraphics
TARGET_BIN := togglemon

$(TARGET_BIN): %: %.o

.PHONY: clean
clean: 
	-rm -f $(TARGET_BIN) *.o
