# Build Package Commands

Compile tools and toolchains:

```shell
make tools/install
make toolchain/install
```

Download package (download the sources of *package_name*)

```shell
make package/[package_name]/download V=s
```

Check package

```shell
make package/[package_name]/check V=s
```

Prepare package (extract the sources, apply patches and download if necessary)（将源代码加载到`build_dir`目录下）

```shell
make package/[package_name]/prepare V=s
```

Update package（打补丁后进行更新，将`build_dir`目录下的内容更新回包的目录下）

```shell
make package/[package_name]/update V=s
```

Compile package (compile *package_name*, prepare and download if necessary)（编译的包放在`bin/packages`目录下）

```shell
make package/[package_name]/compile V=s
```

Clean package (clean the source code)

```shell
make package/[package_name]/clean V=s
```

Index package (build a repository index to make the output directory usable as local *opkg* source)

```shell
make package/index
```

## references

[1]:https://openwrt.org/docs/guide-developer/packages "Creating packages"
[2]:https://openwrt.org/docs/guide-developer/package-policies "OpenWrt packages"

