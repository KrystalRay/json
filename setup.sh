#!/bin/bash

echo "[INSTALLING] Dependency for libjsonrpc-cpp"
sudo apt install -y libjsoncpp-dev libargtable2-dev libhiredis-dev libmicrohttpd-dev build-essential libcurl4-openssl-dev curl cmake 
if [ $? -ne 0 ]; then
    echo "[ERROR] Failed to install dependencies for libjsonrpc-cpp"
    exit 1
fi
echo "[DONE] Dependency for libjsonrpc-cpp"

echo "[INSTALLING] clone and build libjsonrpc-cpp"
git clone https://github.com/cinemast/libjson-rpc-cpp.git
if [ $? -ne 0 ]; then
    echo "[ERROR] Failed to clone libjson-rpc-cpp"
    exit 1
fi

# echo "[INSTALLING] clone and build libjsonrpc-cpp"
# git clone https://github.com/cinemast/libjson-rpc-cpp.git
# if [ $? -ne 0 ]; then
#     echo "[ERROR] Failed to clone libjson-rpc-cpp"
#     exit 1
# fi

mkdir -p libjson-rpc-cpp/build
cd libjson-rpc-cpp/build
cmake .. && make
if [ $? -ne 0 ]; then
    echo "[ERROR] Failed to build libjson-rpc-cpp"
    exit 1
fi
sudo make install
sudo ldconfig
if [ $? -ne 0 ]; then
    echo "[ERROR] Failed to install libjson-rpc-cpp"
    exit 1
fi
echo "[DONE] installation of libjson-rpc-cpp"

cd ../../

echo "[INSTALLING] clone and compile repository"
cd src
chmod +x compile_test.sh
./compile_test.sh
if [ $? -ne 0 ]; then
    echo "[ERROR] Failed to compile rpc-client-libjsonrpc-cpp"
    exit 1
fi

echo "Deployment completed successfully."
