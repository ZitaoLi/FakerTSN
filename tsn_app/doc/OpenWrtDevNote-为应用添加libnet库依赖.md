# 为应用添加libnet库依赖

## libnet库的一些基本信息

> **Name**:libnet-1.2.x
> **Version**:1.2-rc3-3
> **Description**:Low-level packet creation library\\ \\
> **Installed size**:27kB
> **Dependencies**:libc, libssp, libpcap
> **Categories**:libraries
> **Repositories**:community-packages
> **Architectures**:aarch64_armv8-a, arc_arc700, arc_archs, arm_arm1176jzf-s_vfp, arm_arm926ej-s, arm_cortex-a15_neon-vfpv4, arm_cortex-a5, arm_cortex-a53_neon-vfpv4, arm_cortex-a7_neon-vfpv4, arm_cortex-a8_vfpv3, arm_cortex-a9, arm_cortex-a9_neon, arm_cortex-a9_vfpv3, arm_fa526, arm_mpcore, arm_mpcore_vfp, arm_xscale, armeb_xscale, i386_geode, i386_i486, i386_pentium4, mips64_octeon, mips_24kc, mips_mips32, mipsel_24kc, mipsel_74kc, mipsel_mips32, powerpc_464fp, powerpc_8540, x86_64
> **LEDE Release**:LEDE-17.01.4
> **File size**:28kB
> **License**:GPL-2.0Maintainer:Mislav Novakovic

数据来源：https://openwrt.org/packages/pkgdata_lede17_1/libnet-1.2.x

## 事先准备

在`menuconfig`中选中`libnet`库，然后保存退出：

```
Libraries -> libnet-1.2.x
```

编译`libnet`库：

```shell
make package/feeds/packages/libnet-1.2-x/compile V=s
```

在`bin`目录下找到`libnet`库的`.ipk`安装包，发送到目标设备上：

```shell
filename=$(find ./bin -name "*.ipk" | grep libnet-1.2.x)
scp $filename <target_username>@<target_ip>:<target_path>
```

安装`.ipk`安装包：

```shell
opkg install libnet-1.2.x_<target_name>.ipk
```

> 注意：`libnet`默认依赖`libpcap`，因此需要先自己编译安装好`libpcap`

## 静态库方式添加

静态库方式添加只需要在自己应用的`Makefile`文件中添加静态链接选项：

```makefile
LIBS	:=-l:libnet.a
```

## 动态库方式添加

动态库方式添加需要在自己应用的`Makefile`文件中添加动态链接选项：

```makefile
LIBS	:=-lnet
```

然后在自己应用的`package manifest Makefile`文件中的`Package/helloworld`条目下添加依赖选项：

```makefile
DEPENDS:=+libnet-1.2.x
```

这里要写`libnet-1.2.x`，不要写`libnet`。

最后编译自己的应用就行啦！

## 如何搜索正确的包名

到`$STAGING_DIR/pkginfo`目录下搜索包名的关键字，例如在我的平台下：

```shell
ls ./staging_dir/target-aarch64_cortex-a72_musl/pkginfo/ | grep <possible_pkt_name>
```

观察打印出来的内容，那就是完整的包名。

## References

[1]:https://openwrt.org/docs/guide-developer/packages "Creating packages"
[2]: http://www.voidcn.com/article/p-bpvvmdsp-bwe.html "makefile – Openwrt：如何在新包中添加特定的库依赖项"

