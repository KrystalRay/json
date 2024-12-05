#include <iostream>
#include <fstream>
#include <cstring>
#include <stdint.h>
// #include <jsoncpp/json/json.h>
#include <jsonrpccpp/client/connectors/httpclient.h>

#include "rpc_client.h"
#include "service_client.h"

#ifdef __cplusplus
extern "C" {
#endif

    Json::Value constructJSON(int source_id, int sink_id);
    void saveJSON(const Json::Value& json, const char* filename);

    int32_t mod(int32_t a, int32_t b) {
        return b == 0 ? -1 : a % b;
    }

    Json::Value readInputJson(const char *filename) {
        std::ifstream inputFile(filename, std::ifstream::binary);
        Json::Value inputJson;
        inputFile >> inputJson;
        return inputJson;
    }

    Json::Value callProcessInputJson(const Json::Value& params) {
        // URL is fixed internally
        std::string url = "http://localhost:30001";

        jsonrpc::HttpClient httpclient(url);

        // Set timeout to (300000 milliseconds)
        httpclient.SetTimeout(3000000);

        rpc_client client(httpclient, jsonrpc::JSONRPC_CLIENT_V2);
        
        try {
            return client.dealing(params);
        } catch (jsonrpc::JsonRpcException &e) {
            std::cerr << "JsonRpcException: " << e.what() << std::endl;
            throw;
        } catch (std::exception &e) {
            std::cerr << "Standard exception: " << e.what() << std::endl;
        }

        return Json::Value();
    }

    const char* callProcessInputJson_wrapper(const char* serverUrl, const char* inputJsonFilename) {
        Json::Value params = readInputJson(inputJsonFilename);
        Json::Value result = callProcessInputJson(params);
        Json::StreamWriterBuilder writer;
        std::string jsonString = Json::writeString(writer, result);
        char* cstr = new char[jsonString.length() + 1];
        std::strcpy(cstr, jsonString.c_str());
        return cstr;
    }

    const char* wkTaint_wrapper(int32_t source_id, int32_t sink_id) {
        Json::Value tempjson = constructJSON(source_id, sink_id);
        
        Json::Value result = callProcessInputJson(tempjson);

        Json::StreamWriterBuilder writer;
        std::string jsonString = Json::writeString(writer, result);
        char* cstr = new char[jsonString.length() + 1];
        std::strcpy(cstr, jsonString.c_str());
        return cstr;
    }

    int32_t wkTaint_wrapper_2(int32_t source_id, int32_t sink_id) {
        Json::Value tempjson = constructJSON(source_id, sink_id);
        Json::Value result;
        
                // could be packaged into callProcessInputJson()
                // URL is fixed internally
                std::string url = "http://localhost:30001";

                jsonrpc::HttpClient httpclient(url);

                // Set timeout to (300000 milliseconds)
                httpclient.SetTimeout(3000000);

                rpc_client client(httpclient, jsonrpc::JSONRPC_CLIENT_V2);
                
                try {
                    result =  client.wkRpcProcess(tempjson);
                } catch (std::exception &e) {
                    std::cerr << "Standard exception: " << e.what() << std::endl;
                }

                int status = result["status"].asInt();
                std::string description = result["description"].asString();

                std::cout<< description <<std::endl;
                return status;
    }

        int32_t wkTaint(int32_t source_id, int32_t sink_id) {
        // Json::Value tempjson = constructJSON(source_id, sink_id);
        
        return wkTaint_wrapper_2(source_id, sink_id);
    }


#ifdef __cplusplus
}
#endif
