#ifndef JSON_UTILS_H
#define JSON_UTILS_H

// #include <json/json.h>
#include <nlohmann/json.hpp>

using Json = nlohmann::json; 
#ifdef __cplusplus
extern "C" {
#endif

nlohmann::json constructJSON(int source_id, int sink_id);
void saveJSON(const nlohmann::json& json, const char* filename);

#ifdef __cplusplus
}
#endif

#endif 
