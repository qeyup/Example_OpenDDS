# Extenal links

1. APIs
    1. [DDS](http://epics-dds.sourceforge.net/annotated.html)
    1. [OpenDDS](http://download.opendds.org/doxygen/index.html)
1. [RTI documentation](https://community.rti.com/documentation)
1. [OpenDDS developer's guide](http://download.objectcomputing.com/OpenDDS/OpenDDS-latest.pdf)


# Build docker image

```bash
cd ./docker/host-compiler && sudo ./DockerBuild.sh
```

# Run subcriber

```bash
./Subscriber -DCPSConfigFile /root/workspace/config/rtps.ini
```

# Run publisher

```bash
./Publisher -DCPSConfigFile /root/workspace/config/rtps.ini
```