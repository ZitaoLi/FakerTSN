#!/usr/bin/python

"This example doesn't use OpenFlow, but attempts to run sshd in a namespace."

import sys
from mininet.cli import CLI
from mininet.net import Containernet
from mininet.node import Controller
from mininet.node import Host
from mininet.util import ensureRoot, waitListening
from mininet.node import Docker
from mininet.log import info, setLogLevel

ensureRoot()
setLogLevel('info')

net = Containernet(controller=Controller)
net.autoSetMacs = True  # macs can be set automaticaly

info('*** Adding docker containers\n')
s1 = net.addDocker('s1', dimage='containernet_example:ubuntu-my_topo',
                   volumes=["/home/openwrt-dev/workspaces/openwrt/FakerTSN/tsn_app/:/mnt/vol1:rw"])
s2 = net.addDocker('s2', dimage='containernet_example:ubuntu-my_topo',
                   volumes=["/home/openwrt-dev/workspaces/openwrt/FakerTSN/tsn_app/:/mnt/vol1:rw"])
s3 = net.addDocker('s3', dimage='containernet_example:ubuntu-my_topo',
                   volumes=["/home/openwrt-dev/workspaces/openwrt/FakerTSN/tsn_app/:/mnt/vol1:rw"])
s4 = net.addDocker('s4', dimage='containernet_example:ubuntu-my_topo',
                   volumes=["/home/openwrt-dev/workspaces/openwrt/FakerTSN/tsn_app/:/mnt/vol1:rw"])
h1 = net.addDocker('h1', dimage='containernet_example:ubuntu-my_topo',
                   volumes=["/home/openwrt-dev/workspaces/openwrt/FakerTSN/tsn_app/:/mnt/vol1:rw"])
h2 = net.addDocker('h2', dimage='containernet_example:ubuntu-my_topo',
                   volumes=["/home/openwrt-dev/workspaces/openwrt/FakerTSN/tsn_app/:/mnt/vol1:rw"])
h3 = net.addDocker('h3', dimage='containernet_example:ubuntu-my_topo',
                   volumes=["/home/openwrt-dev/workspaces/openwrt/FakerTSN/tsn_app/:/mnt/vol1:rw"])
h4 = net.addDocker('h4', dimage='containernet_example:ubuntu-my_topo',
                   volumes=["/home/openwrt-dev/workspaces/openwrt/FakerTSN/tsn_app/:/mnt/vol1:rw"])
c1 = net.addDocker('c1', dimage='containernet_example:ubuntu-my_topo',
                   volumes=["/home/openwrt-dev/workspaces/openwrt/FakerTSN/tsn_app/:/mnt/vol1:rw"])

info('*** Creating links\n')

net.addLink(h1, s1) # h1-eth0 - s1-eth0
net.addLink(s1, s2) # s1-eth1 - s2-eth0
net.addLink(s1, s4) # s1-eth2 - s4-eth0
net.addLink(h2, s2) # h2-eth0 - s2-eth1
net.addLink(s2, s3) # s2-eth2 - s3-eth0
net.addLink(h3, s3) # h3-eth0 - s3-eth1
net.addLink(h4, s3) # h4-eth0 - s3-eth2
net.addLink(s2, s4) # s2-eth3 - s4-eth1
net.addLink(s3, s4) # s3-eth3 - s4-eth2

net.addLink(c1, s1)
net.addLink(c1, s2)
net.addLink(c1, s3)
net.addLink(c1, s4)
net.addLink(c1, h1)
net.addLink(c1, h2)
net.addLink(c1, h3)
net.addLink(c1, h4)

info('*** Starting network\n')
net.start()

for key, value in s2.intfs.items():
    print(key)
    print(value)

info('*** Running CLI\n')
CLI(net)

info('*** Stopping network')
net.stop()
