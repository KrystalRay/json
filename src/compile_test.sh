#!/bin/bash

g++ -c -fPIC client.cc  -I/usr/local/include -I/usr/include/jsoncpp -o client.o

g++ -c -fPIC json_utils.cc -I/usr/local/include -I/usr/include/jsoncpp -o json_utils.o


# g++ -shared -o libclient.so client.o json_utils.o -ljsoncpp -lcurl -ljsonrpccpp-common -ljsonrpccpp-client
# g++ -fPIC -shared -o libclient.so client.o json_utils.o -ljsoncpp -lcurl -ljsonrpccpp-common -ljsonrpccpp-client
# g++ -fPIC -shared -o libclient.so client.o json_utils.o -l:libjsoncpp.a -l:libcurl.a -l:libjsonrpccpp-common.a -l:libjsonrpccpp-client.a  
# g++ -fPIC -shared -o libclient.so client.o json_utils.o -L/usr/local/lib -l:libjsoncpp.a -l:libcurl.a -l:libjsonrpccpp-common.a -l:libjsonrpccpp-client.a
# g++ -shared -o lsibclient.so client.o json_utils.o -L/usr/local/lib -ljsoncpp -lcurl -ljsonrpccpp-common -ljsonrpccpp-client

g++ -shared -o libclient.so client.o json_utils.o -L/usr/local/lib -l:libjsoncpp.so -lcurl -ljsonrpccpp-common -ljsonrpccpp-client

# g++ main.cc -I/usr/include/jsoncpp -L. -lclient -ljsoncpp -lcurl -ljsonrpccpp-common -ljsonrpccpp-client -o sampleclient
