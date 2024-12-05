```
# setup and compile
chmod +x setup.sh
./setup.sh
```
看见完成提示后，`src` 目录下会出现 `libclient.so` 文件，即 `wkTaint` 对应 .so 文件。

如果compile_test.sh出现问题，可能是json.h位置错误导致。检查include路径，修改一下compile_test.sh中的-I即可

```
# before change
g++ -c -fPIC client.cc  -I/usr/local/include -I/usr/include/jsoncpp -o client.o

g++ -c -fPIC json_utils.cc -I/usr/local/include -I/usr/include/jsoncpp -o json_utils.o

# after change 
g++ -c -fPIC client.cc  -I/usr/local/include  -o client.o

g++ -c -fPIC json_utils.cc -I/usr/local/include  -o json_utils.o
```
