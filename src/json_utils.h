#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <json/json.h>

#ifdef __cplusplus
extern "C" {
#endif

Json::Value constructJSON(int source_id, int sink_id);
void saveJSON(const Json::Value& json, const char* filename);

#ifdef __cplusplus
}
#endif

#endif 
