#!/usr/bin/python

"""
This is a simple example that demonstrates multiple links
between nodes.
"""

from mininet.net import Containernet
from mininet.node import Controller
from mininet.cli import CLI
from mininet.log import setLogLevel
from mininet.topo import Topo

def runMultiLink():
    "Create and run multiple link network"
    net = Containernet(controller=Controller )
    net.addController('c0')

    d1 = net.addDocker('d1', dimage="ubuntu:trusty")
    d2 = net.addDocker('d2', dimage="ubuntu:trusty")
    s1 = net.addSwitch('s1')

    net.addLink( s1, d1 )
    net.addLink( s1, d1 )
    net.addLink( s1, d2 )
    net.addLink( s1, d2 )

    net.start()

    for key in d1.intfs.keys():
        print(key)

    CLI( net )

    net.stop()

if __name__ == '__main__':
    setLogLevel( 'info' )
    runMultiLink()
