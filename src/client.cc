#include <iostream>
#include <fstream>
#include <cstring>
#include <stdint.h>
#include <jsonrpccxx/http_client_connector.hpp>

#include "httplib.h"
#include "rpc_client.h"
#include "service_client.h"
#ifdef __cplusplus
extern "C" {
#endif

nlohmann::json constructJSON(int source_id, int sink_id);
void saveJSON(const nlohmann::json& json, const char* filename);

int32_t mod(int32_t a, int32_t b) {
    return b == 0 ? -1 : a % b;
}

nlohmann::json readInputJson(const char *filename) {
    std::ifstream inputFile(filename, std::ifstream::binary);
    nlohmann::json inputJson;
    if (inputFile >> inputJson) {
        return inputJson;
    } else {
        throw std::runtime_error("Failed to read or parse JSON from file");
    }
}

nlohmann::json callProcessInputJson(const nlohmann::json& params) {
    std::string url = "http://localhost:30001";
    jsonrpccxx::CppHttpLibClientConnector client(url); // 使用我们定义的连接器
    jsonrpccxx::JsonRpcClient rpcClient(client, jsonrpccxx::version::v2);

    try {
        return rpcClient.CallMethod<nlohmann::json>("dealing", params);
    } catch (const jsonrpccxx::JsonRpcException &e) {
        std::cerr << "JsonRpcException: " << e.what() << std::endl;
        throw;
    } catch (const std::exception &e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }

    return nlohmann::json();
}

const char* callProcessInputJson_wrapper(const char* inputJsonFilename) {
    nlohmann::json params = readInputJson(inputJsonFilename);
    nlohmann::json result = callProcessInputJson(params);
    std::string jsonString = result.dump(); // 使用 nlohmann::json 的 dump 方法
    char* cstr = new char[jsonString.length() + 1];
    std::strcpy(cstr, jsonString.c_str());
    return cstr;
}

const char* wkTaint_wrapper(int32_t source_id, int32_t sink_id) {
    nlohmann::json tempjson = constructJSON(source_id, sink_id);
    nlohmann::json result = callProcessInputJson(tempjson);
    std::string jsonString = result.dump(); // 使用 nlohmann::json 的 dump 方法
    char* cstr = new char[jsonString.length() + 1];
    std::strcpy(cstr, jsonString.c_str());
    return cstr;
}

int32_t wkTaint_wrapper_2(int32_t source_id, int32_t sink_id) {
    nlohmann::json tempjson = constructJSON(source_id, sink_id);
    nlohmann::json result;

    std::string url = "http://localhost:30001";
    jsonrpccxx::CppHttpLibClientConnector httpclient(url);
    httpclient.SetTimeout(300000); // Set timeout to (300000 milliseconds)

    jsonrpccxx::JsonRpcClient rpcClient(httpclient, jsonrpccxx::version::v2);
    
    try {
        result = rpcClient.CallMethod<nlohmann::json>("wkRpcProcess", tempjson);
    } catch (const std::exception &e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }

    int status = result["status"].get<int>(); // 使用 get<int>() 获取整数值
    std::string description = result["description"].get<std::string>(); // 使用 get<std::string>() 获取字符串值

    std::cout << description << std::endl;
    return status;
}

int32_t wkTaint(int32_t source_id, int32_t sink_id) {
    return wkTaint_wrapper_2(source_id, sink_id);
}


#ifdef __cplusplus
}
#endif

    // const char* callProcessInputJson_wrapper(const char* serverUrl, const char* inputJsonFilename) {
    //     nlohmann::json params = readInputJson(inputJsonFilename);
    //     nlohmann::json result = callProcessInputJson(params);
    //     Json::StreamWriterBuilder writer;
    //     std::string jsonString = Json::writeString(writer, result);
    //     char* cstr = new char[jsonString.length() + 1];
    //     std::strcpy(cstr, jsonString.c_str());
    //     return cstr;
    // }

    // const char* wkTaint_wrapper(int32_t source_id, int32_t sink_id) {
    //     nlohmann::json tempjson = constructJSON(source_id, sink_id);
        
    //     nlohmann::json result = callProcessInputJson(tempjson);

    //     Json::StreamWriterBuilder writer;
    //     std::string jsonString = Json::writeString(writer, result);
    //     char* cstr = new char[jsonString.length() + 1];
    //     std::strcpy(cstr, jsonString.c_str());
    //     return cstr;
    // }

