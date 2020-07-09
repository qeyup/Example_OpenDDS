cmake_minimum_required(VERSION 3.1)

project(OpenDDS_Subscriber_Example)


# Build data typesupport source
set(IDL_FILE "src/msg.idl")
set(IDL_OUT_DIR "idl")
get_filename_component(IDL_FILE_NAME ${IDL_FILE} NAME_WE)
get_filename_component(IDL_FILE_FULL_NAME ${IDL_FILE} NAME)
get_filename_component(IDL_FILE_PATH ${IDL_FILE} ABSOLUTE)
configure_file(${IDL_FILE_PATH} ${CMAKE_CURRENT_BINARY_DIR}/${IDL_OUT_DIR}/${IDL_FILE_FULL_NAME} COPYONLY)
execute_process(COMMAND opendds_idl -o . msg.idl WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${IDL_OUT_DIR})
execute_process(COMMAND tao_idl -o . -I$ENV{DDS_ROOT} -I/usr/include ${IDL_FILE_FULL_NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${IDL_OUT_DIR})
execute_process(COMMAND tao_idl -o . -I$ENV{DDS_ROOT} -I/usr/include ${IDL_FILE_NAME}TypeSupport.idl WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${IDL_OUT_DIR})
file(GLOB_RECURSE GEN_SOURCE_FILES "${CMAKE_CURRENT_BINARY_DIR}/${IDL_OUT_DIR}/*.cpp")
include_directories(${CMAKE_CURRENT_BINARY_DIR}/${IDL_OUT_DIR}/)


# Gen exec
add_executable(
    Subscriber 
    ${GEN_SOURCE_FILES}
    "src/main.cpp"
)


# link dds
target_link_libraries(
    Subscriber

    # OpenDDS
    OpenDDS_Rtps
    OpenDDS_Rtps_Udp
    OpenDDS_Dcps
    OpenDDS_Tcp
    OpenDDS_InfoRepoDiscovery

    # TAO
    TAO_AnyTypeCode
    TAO_PortableServer
    TAO

    # ACE
    ACE

    # Others
    dl
    nsl
    m
    pthread
    rt
)
