#ifndef SERVICE_CLIENT_H
#define SERVICE_CLIENT_H

// #include <json/json.h>
#include <nlohmann/json.hpp>

using Json = nlohmann::json;
#ifdef __cplusplus
extern "C" {
#endif

    int32_t mod(int32_t a, int32_t b);

    nlohmann::json readInputJson(const char *filename);

    nlohmann::json callProcessInputJson(const nlohmann::json& params);

#ifdef __cplusplus
}
#endif

#endif // SERVICE_CLIENT_H
