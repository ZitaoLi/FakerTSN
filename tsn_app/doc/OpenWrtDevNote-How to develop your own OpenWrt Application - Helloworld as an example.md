# How to develop your own OpenWrt Application - "Hello World" as an example

## Preparing your environment

Install the necessary dependencies using

```
sudo apt-get install gcc g++ binutils patch bzip2 flex bison make autoconf gettext texinfo unzip sharutils libncurses5-dev ncurses-term zlib1g-dev gawk asciidoc libz-dev git-core uuid-dev libacl1-dev liblzo2-dev pkg-config libc6-dev curl libxml-parser-perl ocaml-nox wget
```

Download OpenWrt source code using

```
git clone https://git.openwrt.org/openwrt/openwrt.git source
```

Enter source directory, checkout version and clean make using

```
cd /home/buildbot/source
git checkout openwrt-19.07 // don't execute this commad beacause it doesn't include subtarget bcm2711 if you need it
make distclean
```

> Note that `make disclean` will remove all  compiled parts or settings, and delete the package and source code you have downloaded.

Update and install "feeds packages" using

```
./scripts/feeds update -a
./scripts/feeds install -a
```

Configure your own OpenWrt using

```
make defconfig
make menuconfig
```

Configure your own OpenWrt Kernel using

```
make kernel_menuconfig
```

Build OpenWrt image using

```
make V=99
```

If you only want to install toolchain and plan to build image later, then using

```
make toolchain/install
```

Add toolchain to PATH variable using

```
export PATH=/home/buildbot/source/staging_dir/host/bin:$PATH
```

## Creating "Hello World" application

Create application source code:

```
cd /home/buildbot
mkdir helloworld
cd helloworld
touch helloworld.c
```

The following is the content of `helloworld.c`:

```c
#include <stdio.h>
 
int main(void)
{
    printf("\nHello, world!\n\n");
    return 0;
}
```

Compile, link and test the application:

```
gcc -c -o helloworld.o helloworld.c -Wall
gcc -o helloworld helloworld.o
./helloworld
```

Then you will see the text "Hello World" on Terminal.

## Creating package from your application

Create a "package feed" for you application:

```
cd /home/buildbot
mkdir -p mypackages/examples/helloworld
```

Create the "package manifest" file:

```
cd home/buildbot/mypackages/examples/helloworld
touch Makefile
```

> Each **package** in the OpenWrt build system is described by a package **manifest file**. The **manifest file** is responsible for describing the package, what it does, and must at least provide instructions on where to obtain the source code, how to build it and which files should be contained in the final installable package. A **package manifest** may additionally contain options for optional configuration scripts, specify dependencies between packages and so on.

The following is the content of `Makefile`:

```makefile
include $(TOPDIR)/rules.mk

# Name, version and release number
# The name and version of your package are used to define the variable to point to the build directory of your package: $(PKG_BUILD_DIR)
PKG_NAME:=helloworld
PKG_VERSION:=1.0
PKG_RELEASE:=1

# Source settings (i.e. where to find the source codes)
# This is a custom variable, used below
SOURCE_DIR:=/home/buildbot/helloworld

include $(INCLUDE_DIR)/package.mk

# Package definition; instructs on how and where our package will appear in the overall configuration menu ('make menuconfig')
define Package/helloworld
	SECTION:=examples
	CATEGORY:=Examples
	TITLE:=Hello, World!
endef

# Package description; a more verbose description on what our package does
define Package/helloworld/description
  A simple "Hello, world!" -application.
endef

# Package preparation instructions; create the build directory and copy the source code. 
# The last command is necessary to ensure our preparation instructions remain compatible with the patching system.
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp $(SOURCE_DIR)/* $(PKG_BUILD_DIR)
	$(Build/Patch)
endef

# Package build instructions; invoke the target-specific compiler to first compile the source file, and then to link the file into the final executable
define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/helloworld.o -c $(PKG_BUILD_DIR)/helloworld.c
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/$1 $(PKG_BUILD_DIR)/helloworld.o
endef

# Package install instructions; create a directory inside the package to hold our executable, and then copy the executable we built previously into the folder
define Package/helloworld/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/helloworld $(1)/usr/bin
endef

# This command is always the last, it uses the definitions and variables we give above in order to get the job done
$(eval $(call BuildPackage,helloworld))
```

## Include your feed into OpenWrt build system:

```
cd /home/buildbot/source
touch feeds.conf
```

And attach following content into `feeds.conf` file:

```
src-link mypackages /home/buildbot/mypackages
```

Update and install your package feed:

```
cd /home/buildbot/source
./scripts/feeds update mypackages
./scripts/feeds install -a -p mypackages
```

## Building, deploying and testing your application

Build the package:

```
cd /home/buildbot/source
make package/helloworld/compile
```

If everything went successfully, we are presented with a brand new package named `helloworld_1.0-1_<arch>.ipk` in `bin/packages/<arch>/mypackages` folder. You can use following command to find it:

 ```
 find . -name "*.ipk" | grep "helloworld"
 ```

Send the`.ipk` file to target device:

```
scp /home/buildbot/source/bin/packages/<arch>/mypackages/helloworld_1.0-1_<arch>.ipk root@target_ip:/tmp
```

Deploy your application:

```
root@OpenWrt:/# opkg install /tmp/helloworld_1.0-1_<arch>.ipk
Installing helloworld (1.0-1) to root...
Configuring helloworld.
```

Test your application:

```
root@OpenWrt:/# helloworld

Hello, world!
```

If you want to remove the installation of your application, use the following command:

```
root@OpenWrt:/# opkg remove helloworld
Removing package helloworld from root...
```

## Migrating to use GNU make in your application

Modify the package manifest under directory `home/buildbot/mypackages/examples/helloworld`:

```makefile
include $(TOPDIR)/rules.mk

# Name, version and release number
# The name and version of your package are used to define the variable to point to the build directory of your package: $(PKG_BUILD_DIR)
PKG_NAME:=helloworld
PKG_VERSION:=1.0
PKG_RELEASE:=1

# Source settings (i.e. where to find the source codes)
# This is a custom variable, used below
SOURCE_DIR:=/home/openwrt/helloworld

include $(INCLUDE_DIR)/package.mk

# Package definition; instructs on how and where our package will appear in the overall configuration menu ('make menuconfig')
define Package/helloworld
	SECTION:=examples
	CATEGORY:=Examples
    TITLE:=Hello, World!
endef

# Package description; a more verbose description on what our package does
define Package/helloworld/description
	A simple "Hello, world!" -application.
endef

# Package preparation instructions; create the build directory and copy the source code. 
# The last command is necessary to ensure our preparation instructions remain compatible with the patching system.
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp $(SOURCE_DIR)/* $(PKG_BUILD_DIR)
	$(Build/Patch)
endef

# Package build instructions; invoke the GNU make tool to build our package
define Build/Compile
	 $(MAKE) -C $(PKG_BUILD_DIR)  CC="$(TARGET_CC)" CFLAGS="$(TARGET_CFLAGS)" LDFLAGS="$(TARGET_LDFLAGS)"
endef

# Package install instructions; create a directory inside the package to hold our executable, and then copy the executable we built previously into the folder
define Package/helloworld/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/helloworld $(1)/usr/bin
endef

# This command is always the last, it uses the definitions and variables we give above in order to get the job done
$(eval $(call BuildPackage,helloworld))

```

Create `Makefile` under directory `/home/buildbot/helloworld`:

```
cd /home/buildbot/helloworld
touch Makefile
```

Attach the following content into `Makefile`:

```makefile
SOURCE  := $(wildcard *.c) $(wildcard *.cpp)
OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

TARGET  := helloworld

CC      := gcc
LIBS    :=
LDFLAGS :=
DEFINES :=
INCLUDE := -I.
CFLAGS  := -g -Wall -O3 $(DEFINES) $(INCLUDE)
CXXFLAGS:= $(CFLAGS) -DHAVE_CONFIG_H

.PHONY : everything objs clean veryclean rebuild

everything : $(TARGET)

all : $(TARGET)

objs : $(OBJS)

rebuild: veryclean everything

clean :
	rm -fr *.so
	rm -fr *.o

veryclean : clean
	rm -fr $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

```

> The official document writes like this:
>
> ```makefile
> # Global target; when 'make' is run without arguments, this is what it should do
> all: helloworld
>  
> # These variables hold the name of the compilation tool, the compilation flags and the link flags
> # We make use of these variables in the package manifest
> CC = gcc
> CFLAGS = -Wall
> LDFLAGS = 
>  
> # This variable identifies all header files in the directory; we use it to create a dependency chain between the object files and the source files
> # This approach will re-build your application whenever any header file changes. In a more complex application, such behavior is often undesirable
> DEPS = $(wildcard *.h)
>  
> # This variable holds all source files to consider for the build; we use a wildcard to pick all files
> SRC = $(wildcard *.c)
>  
> # This variable holds all object file names, constructed from the source file names using pattern substitution
> OBJ = $(patsubst %.c, %.o, $(SRC))
>  
> # This rule builds individual object files, and depends on the corresponding C source files and the header files
> %.o: %.c $(DEPS)
>         $(CC) -c -o $@ $< $(CFLAGS)
>  
> # To build 'helloworld', we depend on the object files, and link them all into a single executable using the compilation tool
> # We use automatic variables to specify the final executable name 'helloworld', using '$@' and the '$^' will hold the names of all the
> # dependencies of this rule
> helloworld: $(OBJ)
>         $(CC) -o $@ $^ $(LDFLAGS)
>  
> # To clean build artifacts, we specify a 'clean' rule, and use PHONY to indicate that this rule never matches with a potential file in the directory
> .PHONY: clean
>  
> clean:
>         rm -f helloworld *.o   
> ```

Clean and recompile your package:

```
cd /home/buildbot/source
make package/helloworld/clean V=s
make package/helloworld/compile V=s
```

> If you encounter errors when building it using GNU make, it is sometimes necessary to manually update and install the feeds:
>
> ```
> cd /home/buildbot/source
> ./scripts/feeds update mypackages
> ./scripts/feeds install -a -p mypackages
> ```

## References

[1]:  https://openwrt.org/docs/guide-developer/helloworld/start  ""Hello, world!" for OpenWrt"
[2]:  https://openwrt.org/docs/guide-developer/packages?s%5B%5D=pkg&s%5B%5D=fixup  "Creating packages"
[3]:  https://openwrt.org/zh-cn/doc/devel/packages?s%5B%5D=build&s%5B%5D=prepare  "创建软件包"
