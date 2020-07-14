# Extenal links

1. APIs
    1. [DDS](http://epics-dds.sourceforge.net/annotated.html)
    1. [RTI Connext C](https://community.rti.com/static/documentation/connext-dds/6.0.1/doc/api/connext_dds/api_c/index.html)
    1. [RTI Connext Traditional C++](https://community.rti.com/static/documentation/connext-dds/6.0.1/doc/api/connext_dds/api_cpp/index.html)
    1. [RTI Connext Modern C++](https://community.rti.com/static/documentation/connext-dds/6.0.1/doc/api/connext_dds/api_cpp2/index.html)
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