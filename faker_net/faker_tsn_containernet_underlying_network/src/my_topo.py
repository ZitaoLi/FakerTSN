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
h1 = net.addDocker('h1', dimage='containernet_example:ubuntu-my_topo',
                   volumes=["/home/openwrt-dev/workspaces/openwrt/FakerTSN/tsn_app/:/mnt/vol1:rw"])
s2 = net.addDocker('s2', dimage='containernet_example:ubuntu-my_topo',
                   volumes=["/home/openwrt-dev/workspaces/openwrt/FakerTSN/tsn_app/:/mnt/vol1:rw"])
h3 = net.addDocker('h3', dimage='containernet_example:ubuntu-my_topo',
                   volumes=["/home/openwrt-dev/workspaces/openwrt/FakerTSN/tsn_app/:/mnt/vol1:rw"])
h4 = net.addDocker('h4', dimage='containernet_example:ubuntu-my_topo',
                   volumes=["/home/openwrt-dev/workspaces/openwrt/FakerTSN/tsn_app/:/mnt/vol1:rw"])
c1 = net.addDocker('c1', dimage='containernet_example:ubuntu-my_topo',
                   volumes=["/home/openwrt-dev/workspaces/openwrt/FakerTSN/tsn_app/:/mnt/vol1:rw"])

info('*** Creating links\n')
net.addLink(h1, s2)
net.addLink(s2, h3)
net.addLink(s2, h4)
net.addLink(c1, h1)
net.addLink(c1, s2)
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
