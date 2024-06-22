CFLAGS = -std=gnu11 -Wall -Wno-unused-function
LDLIBS =
OBJ = detect.o platforms/skl_kbl.o platforms/cfl.o

.PHONY: clean

detect: $(OBJ)

clean:
	rm -f detect $(OBJ)
