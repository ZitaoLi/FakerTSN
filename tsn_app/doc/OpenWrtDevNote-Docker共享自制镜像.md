# Docker共享自制镜像

## Show based infomations

show all images:

```
docker images
➜  ~ docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
ubuntu/openwrt      v2                  93d7f4d9e1a8        7 minutes ago       11.9GB
ubuntu              latest              775349758637        2 months ago        64.2MB
```

show all containers:

```
➜  ~ docker container ls -a
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS                          PORTS               NAMES
8967ab0b6cce        ubuntu              "/bin/bash"         6 weeks ago         Exited (0) About a minute ago                       ubuntu-openwrt
0851f34b65b0        ubuntu              "/bin/bash"         7 weeks ago         Exited (0) 6 weeks ago                              affectionate_kalam
```

## Update image

commit self-made image:

```
docker commit \ 
-m="<messages about your image>" \
-a="author name" \ 
<container ID> \
<target image name>:<version>
```

for instances:

```
➜  ~ docker commit -m="add openwrt" -a="lztao" 8967ab0b6cce ubuntu/openwrt:v2
sha256:93d7f4d9e1a876873e7fc7dfbc7665115e05d571f02be9380932828ea6be1681
```

check your image:

```
➜  ~ docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
ubuntu/openwrt      v2                  93d7f4d9e1a8        13 minutes ago      11.9GB
ubuntu              latest              775349758637        2 months ago        64.2MB
```

## Share your image

Package docker image:

```
docker save -o <target>.tar <docker image name|docker container name>:<TAG>
```

or

```
docker export -o <target>.tar <docker container id>
```

> `save` need more available disk space than `export`

The tar package may be to large and need to be compressed:

```
tar -zcvf <target>.tar.gz <target>.tar
```

Send to remote host:

```
scp ./<target>.tar.gz <remote-user-name>@<remote-ip>:<path-to-store>
```

uncompress tar.gz package:

```
tar -zxvf <target>.tar.gz
```

Unpackage docker image:

```
docker load -i <target>.tar  # this command will create docker images
```

or

```
docker import <target>.tar <docker image name>
```



## References

[1]:https://juejin.im/post/5c13130b6fb9a049b347dcd1 "Docker的架构与自制镜像的发布"
[2]:https://segmentfault.com/q/1010000001732141 "不通过 docker hub 怎样把 docker image 共享给别人"

