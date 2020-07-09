# CENG435 TERM PROJECT PART 2

Group 26 <br />
Furkan Doğan / 2098937 <br />
Sinan Talha Koşar / 2099190 <br />

In this part of project we are sending data from source to destination with RDT over UDP and TCP.We are sending 5MB file as a maximum 1000bytes of packets when we send first packet from source we are getting time with NTP server and write it to a file and at the Destination when we get last packet we are also getting time with the NTP and we are also getting time from file and print their difference to get end-to-end delay.

## PIP Installation

To install pip,

```bash
sudo apt-get update
```
Then run the following:

```bash
sudo apt-get install python3-pip
```

If you get Python locale error: unsupported locale setting,run the following commands

```bash
export LC_ALL="en_US.UTF-8"
export LC_CTYPE="en_US.UTF-8"
sudo dpkg-reconfigure locales
```


## NTP Installation
Use the package manager [pip](https://pip.pypa.io/en/stable/) to install NTP.

```bash
sudo pip3 install NTPlib
```


## Usage of NTP

```python
import NTPlib

c = ntplib.NTPClient()   # returns NTPClient
response = c.request('time.google.com') # returns response from time.google.com
time = response.tx_time #return current time as an timestamp
```

## Source.py

Source.py gets time with NTP and write its to time.txt file and then send all packets to Broker with TCP.

## Broker.py

Broker.py gets packets from Source with TCP and sends them to the Destination with RDT over UDP.

## Destination.py

Destination.py gets packets from Broker with RDT over UDP and writes them to output2.txt file,also gets time with NTP and take difference between times and prints end-to-end delay between Source and Destination.

## How to Run

These commands need to apply one after another.

```bash
python destination.py [Port1 to Receive] [Port2 to Receive]

```

```bash
python broker.py [Port to Receive] [IP1 to Send] [Port1 to Send] [IP2 to Send] [Port2 to Send]

```

```bash
python source.py [IP to Send] [Port to Send] [Input File]

```

## To add Packet Loss Percentage

```bash
sudo tc qdisc change dev eth0 root netem loss 0.5% corrupt 0% duplicate 0% delay 3 reorder 0% 0%
```

This causes the added packet loss with 0.5 percentage and added delay 3ms to the interface named eth0.

```bash
sudo tc qdisc change dev eth0 root netem loss 10% corrupt 0% duplicate 0% delay 3 reorder 0% 0%
```

This causes the added packet loss with 10 percentage and added delay 3ms to the interface named eth0.

```bash
sudo tc qdisc change dev eth0 root netem loss 20% corrupt 0% duplicate 0% delay 3 reorder 0% 0%
```

This causes the added packet loss with 20 percentage and added delay 3ms to the interface named eth0.

## To add Corruption Percentage

```bash
sudo tc qdisc change dev eth0 root netem loss 0% corrupt 0.2% duplicate 0% delay 3 reorder 0% 0%
```

This causes the added corruption with 0.2 percentage and added delay 3ms to the interface named eth0.

```bash
sudo tc qdisc change dev eth0 root netem loss 0% corrupt 10% duplicate 0% delay 3 reorder 0% 0%
```

This causes the added corruption with 10 percentage and added delay 3ms to the interface named eth0.

```bash
sudo tc qdisc change dev eth0 root netem loss 0% corrupt 20% duplicate 0% delay 3 reorder 0% 0%
```

This causes the added corruption with 20 percentage and added delay 3ms to the interface named eth0.

## To add Reordering Percentage

```bash
sudo tc qdisc change dev eth0 root netem loss 0% corrupt 0% duplicate 0% delay 3 reorder 1% 50%
```

This causes the added reordering with 1 percentage and added delay 3ms to the interface named eth0.

```bash
sudo tc qdisc change dev eth0 root netem loss 0% corrupt 0% duplicate 0% delay 3 reorder 10% 50%
```

This causes the added reordering with 10 percentage and added delay 3ms to the interface named eth0.

```bash
sudo tc qdisc change dev eth0 root netem loss 0% corrupt 0% duplicate 0% delay 3 reorder 35% 50%
```

This causes the added reordering with 35 percentage and added delay 3ms to the interface named eth0.

## To change Routing Table

```bash
sudo route add -net 192.168.1.0 netmask 255.255.255.0 gw 192.168.1.1 dev eth0
```

This causes route the packets coming to 192.168.1.0 to the 192.168.1.1

```bash
sudo route del -net 192.168.1.0 netmask 255.255.255.0 gw 192.168.1.1 dev eth0
```

The above command will delete the route to 192.168.1.0 to 192.168.1.1 from the routing table.