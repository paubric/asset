# Makefile - gccDppConsoleInet

ifndef CFG
CFG=Debug
endif
CC=gcc
CFLAGS=-m32 
CXX=g++
CXXFLAGS=$(CFLAGS)
ifeq "$(CFG)" "Debug"
CFLAGS+= -W -O0 -fexceptions -I../gccDppConsoleInet/DeviceIO/ -I../gccDppConsoleInet/ -g -fno-inline -D_DEBUG -D_CONSOLE 
LD=$(CXX) $(CXXFLAGS)
LDFLAGS=
LDFLAGS+= -L../gccDppConsoleInet/DeviceIO/
LIBS+=-lstdc++ -lm -lnsl
# LIBS+=-lstdc++ -lm -static -lnsl -lsocket
# LIBS+=-lstdc++ -lm -static -lws2_32
ifndef TARGET
TARGET=gccDppConsoleInet
endif
ifeq "$(CFG)" "Release"
CFLAGS+= -W -O2 -fexceptions -I../gccDppConsoleInet/DeviceIO/ -I../gccDppConsoleInet/ -g -fno-inline -DNDEBUG -D_CONSOLE 
LD=$(CXX) $(CXXFLAGS)
LDFLAGS=
LDFLAGS+= -L../gccDppConsoleInet/DeviceIO/
LIBS+=-lstdc++ -lm -lnsl
# LIBS+=-lstdc++ -lm -static -lnsl -lsocket
# LIBS+=-lstdc++ -lm -static -lws2_32
ifndef TARGET
TARGET=gccDppConsoleInet
endif
endif
endif
ifndef TARGET
TARGET=gccDppConsoleInet
endif
.PHONY: all
all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

%.res: %.rc
	$(RC) $(CPPFLAGS) -o $@ -i $<

SOURCE_FILES= \
	./ConsoleHelper.cpp \
	./DeviceIO/NetFinder.cpp \
	./DeviceIO/AsciiCmdUtilities.cpp \
	./DeviceIO/DP5Protocol.cpp \
	./DeviceIO/DP5Status.cpp \
	./DeviceIO/DppUtilities.cpp \
	./DeviceIO/ParsePacket.cpp \
	./DeviceIO/SendCommand.cpp \
	./DeviceIO/DppSocket.cpp \
	./stringex.cpp \
	./gccDppConsoleInet.cpp

HEADER_FILES= \
	./ConsoleHelper.h \
	./DeviceIO/NetFinder.h \
	./DeviceIO/AsciiCmdUtilities.h \
	./DeviceIO/DP5Protocol.h \
	./DeviceIO/DP5Status.h \
	./DeviceIO/DppConst.h \
	./DeviceIO/DppUtilities.h \
	./DeviceIO/ParsePacket.h \
	./DeviceIO/SendCommand.h \
	./DeviceIO/DppSocket.h \
	./stringex.h \
	./stringSplit.h

OBJ_FILES= \
	./ConsoleHelper.o \
	./NetFinder.o \
	./AsciiCmdUtilities.o \
	./DP5Protocol.o \
	./DP5Status.o \
	./DppUtilities.o \
	./ParsePacket.o \
	./SendCommand.o \
	./DppSocket.o \
	./stringex.o \
	./gccDppConsoleInet.o 

RESOURCE_FILES= \

SRCS=$(SOURCE_FILES) $(HEADER_FILES) $(RESOURCE_FILES) 

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJ_FILES) $(LIBS)

.PHONY: clean
clean:
	-rm -f -v $(OBJS) $(TARGET) gccDppConsoleInet.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > gccDppConsoleInet.dep

-include gccDppConsoleInet.dep

