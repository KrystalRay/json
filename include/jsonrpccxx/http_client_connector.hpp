// http_client_connector.hpp
#pragma once
#include "iclientconnector.hpp"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>
#include <jsonrpccxx/common.hpp>
namespace jsonrpccxx {

class CppHttpLibClientConnector : public IClientConnector {
public:
    explicit CppHttpLibClientConnector(const std::string& server_url)
        : server_url_(server_url), client_(server_url.c_str()) {}
    void SetTimeout(unsigned int timeout_ms) {
        client_.set_read_timeout(timeout_ms);
        client_.set_write_timeout(timeout_ms);
    }
    std::string Send(const std::string &request) override {
        httplib::Client cli(server_url_.c_str());
        auto res = cli.Post("/jsonrpc", request, "application/json");
        if (!res || res->status != 200) {
            throw jsonrpccxx::JsonRpcException(-32003, "Client connector error: received status != 200");
        }
        return res->body;
    }

private:
    std::string server_url_;
    httplib::Client client_;
};

} // namespace jsonrpccxx