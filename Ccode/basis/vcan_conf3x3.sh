#!/bin/bash
#Setting up 2 virtuell can nodes
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link add dev vcan1 type vcan
sudo ip link add dev vcan2 type vcan
sudo ip link set up vcan0
sudo ip link set up vcan1
sudo ip link set up vcan2
#Bridge the nodes bidirectional
sudo modprobe can-gw
sudo cangw -A -s vcan0 -d vcan1 -e
sudo cangw -A -s vcan1 -d vcan0 -e

sudo cangw -A -s vcan0 -d vcan2 -e
sudo cangw -A -s vcan2 -d vcan0 -e

sudo cangw -A -s vcan2 -d vcan1 -e
sudo cangw -A -s vcan1 -d vcan2 -e
