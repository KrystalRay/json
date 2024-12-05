#include <iostream>
// #include "service_client.h"
#include "json_utils.h"

int main() {
    // int a = 10;
    // int b = 3;
    // std::cout << "mod(" << a << ", " << b << ") = " << mod(a, b) << std::endl;

    // Json::Value inputJson = readInputJson("input.json");

    // Json::Value result = callProcessInputJson("http://localhost:30001", "/home/chenyz/json-rpc/souffle-client/input.json");
    // std::cout << "Result: " << result.toStyledString() << std::endl;

    int source_id = 1;
    int sink_id = 2;
    Json::Value result = constructJSON(source_id, sink_id);

    return 0;
}
