# 新版package manifest和Makefile

因为工程项目结构的调整，需要重写`package manifest`和`Makefile`。

原`package manifest`和`Makefile`在“[20191219-OpenWrtDevNote-How to develop your own OpenWrt Application - Helloworld as an example](./20191219-OpenWrtDevNote-How to develop your own OpenWrt Application - Helloworld as an example)”。

## 更新`Makefile`文件

工程目录调整为：

```
helloworld/
├── bin
├── doc
├── include
├── lib
├── Makefile
├── src
│   ├── main.cc
│   └── subdir
│       ├── foo.cc
│       └── foo.h
└── test
```

因此需要修改原来的`Makefile`文件，新的`Makefile`文件如下：

```makefile
DIR_INC	= ./include
DIR_SRC	= ./src
DIR_SRC_SUBDIR = $(DIR_SRC)/subdir
DIR_TEST= ./test
DIR_BIN	= ./bin
DIR_LIB	= ./lib

SRCS  	:= $(wildcard *.cc)
SRCS  	+= $(wildcard ${DIR_SRC}/*.cc) 
SRCS  	+= $(wildcard ${DIR_SRC_SUBDIR}/*.cc)
SRCS  	+= $(wildcard ${DIR_TEST}/*.cc)
OBJS    := $(patsubst %.cc,${DIR_BIN}/%.o,$(patsubst %.cc,%.o,$(SRCS)))

CC      := gcc
CXX     := g++
DEBUG 	:= y
RM		:= rm -rf
MAKE = 	:make
LIBS    := -lpcap -lnet
LDFLAGS :=
DEFINES :=
INCLUDES:= -I$(DIR_INC)
CFLAGS  := -Wall $(DEFINES) $(INCLUDES)
ifeq ($(DEBUG), y)
CFLAGS	+= -g
else
CFLAGS	+= -O3
endif
CXXFLAGS:= $(CFLAGS) -std=c++11

PROGRAM	:= helloworld
TARGET  := ${DIR_BIN}/${PROGRAM}

.PHONY 	: all clean cleanall rebuild

all: $(TARGET)

# src
${DIR_BIN}/%.o: $(DIR_SRC)/%.cc
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# test
${DIR_BIN}/%.o: $(DIR_TEST)/%.cc
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# $@ represent all targets (executable files)
# $^ represent all all dependencies (*.o files)
$(TARGET) : $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

rebuild: cleanall

clean:
	rm -rf *.so *.d *.o
	find ${DIR_BIN} . -name '*.o' -exec $(RM) '{}' \;

cleanall: clean
	$(RM) -rf $(TARGET)
```

这是一种集中式写法，其他写法参考：

[多级目录Makefile编写方法]([http://xnzaa.github.io/2015/01/26/Makefile%E5%9F%BA%E7%A1%80%E6%95%99%E7%A8%8B/](http://xnzaa.github.io/2015/01/26/Makefile基础教程/))	

## 更新`package manifest`文件

在`Build/Prepare`阶段，为`cp`命令添加参数`-R`，让它把整个工程都拷贝过去：

```makefile
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp -R $(SOURCE_DIR)/* $(PKG_BUILD_DIR)
	$(Build/Patch)
endef
```

> `Build/Prepare`阶段会把整个工程都拷贝到`$(PKG_BUILD_DIR)`下

新工程将可执行文件放到了`bin`目录下，将`$(PKG_BUILD_DIR)/helloworld`改成`$(PKG_BUILD_DIR)/bin/helloworld`，让`OpenWrt`能够正确安装可执行文件：

```makefile
define Package/helloworld/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin/helloworld $(1)/usr/bin
endef
```

> `$(1)`等价于`$(PKG_BUILD_DIR)/.pkgdir/helloworld`，用于指导`ipk`包的安装

## 

