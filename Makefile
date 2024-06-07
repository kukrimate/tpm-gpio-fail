CFLAGS = -std=gnu11 -Wall -Wno-unused-function
LDLIBS =
detect: detect.o platforms/skl_kbl.o platforms/cfl.o
