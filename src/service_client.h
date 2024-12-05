#ifndef SERVICE_CLIENT_H
#define SERVICE_CLIENT_H

#include <json/json.h>

#ifdef __cplusplus
extern "C" {
#endif

    int32_t mod(int32_t a, int32_t b);

    Json::Value readInputJson(const char *filename);

    Json::Value callProcessInputJson(const Json::Value& params);

#ifdef __cplusplus
}
#endif

#endif // SERVICE_CLIENT_H
