## Premises

1. ubuntu 18 (required)
   2. Raspberry Pie 4Bs
3. USB-RJ45 adapters

## Getting Started

### Option 1: Run on Virtual Environment

#### Installing Dependencies

```
sudo apt-get update
sudo apt-get install -y gcc g++ binutils gdb make gdb libpcap-dev
```

#### Installing Containernet

Install `docker` and `python3` before installing containernet.

How to install containernet is referenced by: https://containernet.github.io/#installation

The tutorial of `Mininet`: http://mininet.org/walkthrough/

#### Getting Project

```
git clone https://github.com/ZitaoLi/FakerTSN.git
```

#### Building Project

```
cd FakerTSN
sh ./build -t 0 -c 0
```

#### Building Docker Image

```
cd FakerTSN/faker_net/faker_tsn_containernet_underlying_network/containers/
sh ./build.sh
```

#### Running Project

##### step 1 enable containernet

Run containernet:

```
cd FakerTSN/faker_net/faker_tsn_containernet_underlying_network/src/
sudo python3 my_topo2.py
```

Check network:

```
containernet> net
```

Open  another terminal and view running docker container:

```
openwrt-dev@ubuntu:~$ sudo docker ps -a
[sudo] password for openwrt-dev:
CONTAINER ID        IMAGE                                 COMMAND             CREATED             STATUS                    PORTS               NAMES
dfeffd55245c        containernet_example:ubuntu-my_topo   "/bin/bash"         6 minutes ago       Up 6 minutes                                  mn.h4
12afd4f67ffd        containernet_example:ubuntu-my_topo   "/bin/bash"         6 minutes ago       Up 6 minutes                                  mn.h3
7d88e86fb2d1        containernet_example:ubuntu-my_topo   "/bin/bash"         6 minutes ago       Up 6 minutes                                  mn.s2
3a281ac3e978        containernet_example:ubuntu-my_topo   "/bin/bash"         7 minutes ago       Up 6 minutes                                  mn.h1
```

Attach into docker container:

```
sudo docker attach <container-id>
```

Enter project directory:

```
cd /mnt/vol1
```

If you want to exit docker container without stopping it, enter `Ctrl+P+Q`

If you want to exit docker container only, just enter `exit`

Do never forget to clean containernet context:

```
sudo mn -c
```

##### step 2 run FakerTSN app with specified mode

FakerTSN support three mode:

- `host` mode: run as TSN host
- `switch` mode:  run as TSN switch
- `configurator` mode: run as TSN configurator 

```
./bin/tsn_app -f <config_file>
```

**Example 1: run as host**

```
./bin/tsn_app -f ../config/simulation_2/config_h1.ini
```

```ini
# runtime
log.level=info # log level
log.path=./log/h1.log # path to storage log files
log.reset=yes # wether reset log files or not
log.mode=std # std/file
# data spot
data.path=./data/h1.txt
# tsn context
profile.path=./config/simulation_2
workMode=host # switch/host
deviceName=host1 # device name
enabledTSN=yes # whether enable TSN functions or not
timePrecision=1000000 # time precison, unit is ns
switch.port.class=FlowApp # port class
switch.port.infs=h1-eth0 # physical interfaces
switch.port.cons=h1-eth1
switch.port.per=0.01
## time context
timeUnit=ns # ns us ms s
sizeUnit=B # B K M G
enhancedGCL=ture
```

**Example 2: run as switch**

```
./bin/tsn_app -f ../config/simulation_2/config_s1.ini
```

```ini
# runtime
log.level=info # log level
log.path=./log/s1.log # path to storage log files
log.reset=yes # wether reset log files or not
log.mode=std # std/file
# data spot
data.path=./data/s1.txt
# tsn context
profile.path=./config/simulation_2
workMode=switch # switch/host/configurator
deviceName=switch1 # device name
enabledTSN=yes # whether enable TSN functions or not
timePrecision=1000000 # time precison, unit is ns
switch.port.class=DataPort # port class
switch.port.infs=s1-eth0 s1-eth1 s1-eth2 # physical interfaces
switch.port.cons=s1-eth3
switch.port.per=0.01
## queue context
switch.queue.number=8 # number of queues
switch.queue.class=IEEE8021QbvQueue # queue class
switch.queue.capacity=1024000 # queue size 1M
switch.queue.transmissionSelectionAlgorithm=ErrorToleranceTransmissionSelectionAlgorithm
switch.gcl=EnhancedGateControlList
## time context
timeUnit=ns # ns us ms s
sizeUnit=B # B K M G
enhancedGCL=ture
```

**Example 3: run as configurator**

```
./bin/tsn_app -f ../config/simulation_2/config_h1.ini
```

```ini

# runtime
log.level=info # log level
log.path=./log/c1.log # path to storage log files
log.reset=yes # wether reset log files or not
log.mode=std # std/file
# tsn context
profile.path=./config/simulation_2
workMode=configurator # switch/host
deviceName=c1 # device name
enabledTSN=yes # whether enable TSN functions or not
timePrecision=1000000 # time precison, unit is ns
switch.port.class=TSNController # port class
switch.port.infs=c1-eth0 c1-eth1 c1-eth2 c1-eth3 c1-eth4 c1-eth5 c1-eth6 c1-eth7 # physical interfaces
switch.port.cons=stdin
## time context
timeUnit=ns # ns us ms s
sizeUnit=B # B K M G
enhancedGCL=ture
```



