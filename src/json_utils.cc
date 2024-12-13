// #include <json/json.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <stdexcept>

#include "json_utils.h"
// using json = nlohmann::json;
#ifdef __cplusplus
extern "C" {
#endif

    std::vector<std::vector<std::string>> readCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }
        std::vector<std::vector<std::string>> data;
        std::string line;

        while (std::getline(file, line)) {
            std::stringstream lineStream(line);
            std::string cell;
            std::vector<std::string> row;

            while (std::getline(lineStream, cell, ',')) {
                row.push_back(cell);
            }
            if (row.empty()) {
                std::cerr << "Warning: Empty row found in file " << filename << std::endl;
            }
            data.push_back(row);
        }

        return data;
    }

    nlohmann::json constructJSON(int source_id, int sink_id) {
        nlohmann::json result;

        std::string csv_folder = "/tmp/tmp";

        // Load methods.csv & classes_or_interfaces.csv & packages.csv
        std::string methods_file = csv_folder + "/methods.csv";
        std::string classes_file = csv_folder + "/classes_or_interfaces.csv";
        std::string packages_file = csv_folder + "/packages.csv";

        // Exit or handle error appropriately
        // try {
            auto methods_data = readCSV(methods_file);
            auto classes_data = readCSV(classes_file);
            auto packages_data = readCSV(packages_file);
        // } catch (const std::exception& e) {
        //     std::cerr << "Error reading CSV files: " << e.what() << std::endl;
        //     std::exit(EXIT_FAILURE); 
        // }

        // Mapping CSV data to C++ unordered_map
        std::unordered_map<int, std::vector<std::string>> methods_map;
        std::unordered_map<int, std::vector<std::string>> classes_map;
        std::unordered_map<int, std::string> packages_map;

        // populate maps with exception handling for stoi
        for (const auto& row : methods_data) {
            if (!row.empty()) {
                methods_map[std::stoi(row[0])] = row;
            }
        }

        for (const auto& row : classes_data) {
            if (!row.empty()) {
                classes_map[std::stoi(row[0])] = row;
            }
        }

        for (const auto& row : packages_data) {
            if (!row.empty()) {
                packages_map[std::stoi(row[0])] = row[1];
            }
        }

        // Initialize default values for FieldSources and Kills
        result["FieldSources"] = nlohmann::json(nlohmann::json::array());

        nlohmann::json kill;
        kill["ParameterIndex"] = "1";
        kill["Fields"] = "";
        kill["DeclaringClass"] = "Ljavax/xml/parsers/DocumentBuilder";
        kill["MethodName"] = "setEntityResolver";
        kill["ReturnType"] = "V";
        kill["ArgTypes"] = "Lorg/xml/sax/EntityResolver";
        kill["BelongTo"] = "Solr_CVE_2018_1308";
        result["Kills"].push_back(kill);

        // Processing sink_id
        if (methods_map.find(sink_id) != methods_map.end()) {
            const auto& method_row = methods_map[sink_id];
            int parent_id = std::stoi(method_row[4]);
            std::string method_name = method_row[1];

            if (classes_map.find(parent_id) != classes_map.end()) {
                const auto& class_row = classes_map[parent_id];
                int package_id = std::stoi(class_row[2]);
                std::string class_name = class_row[1];

                if (packages_map.find(package_id) != packages_map.end()) {
                    std::string package_name = packages_map[package_id];

                    // Build DeclaringClass
                    std::string declaring_class = "L" + package_name + "." + class_name;
                    std::replace(declaring_class.begin(), declaring_class.end(), '.', '/');
                    
                    nlohmann::json sink;
                    sink["DeclaringClass"] = declaring_class;
                    sink["MethodName"] = method_name;
                    sink["ReturnType"] = "Lorg/w3c/dom/Document"; 
                    sink["ArgTypes"] = "Lorg/xml/sax/InputSource"; 
                    sink["BelongTo"] = "Solr_CVE_2018_1308";
                    sink["AnySource2AnyArg"] = nlohmann::json(nlohmann::json::object());

                    result["Sinks"].push_back(sink);
                }
            }
        }

        // Processing source_id
        if (methods_map.find(source_id) != methods_map.end()) {
            const auto& method_row = methods_map[source_id];
            int parent_id = std::stoi(method_row[4]);
            std::string method_name = method_row[1];

            if (classes_map.find(parent_id) != classes_map.end()) {
                const auto& class_row = classes_map[parent_id];
                int package_id = std::stoi(class_row[2]);
                std::string class_name = class_row[1];

                if (packages_map.find(package_id) != packages_map.end()) {
                    std::string package_name = packages_map[package_id];

                    // Build DeclaringClass
                    std::string declaring_class = "L" + package_name + "." + class_name;
                    std::replace(declaring_class.begin(), declaring_class.end(), '.', '/');

                    nlohmann::json source;
                    source["DeclaringClass"] = declaring_class;
                    source["MethodName"] = method_name;
                    source["ReturnType"] = "Ljava/lang/String"; 
                    source["ArgTypes"] = ""; 
                    source["BelongTo"] = "Solr_CVE_2018_1308"; 
                    source["ParameterIndex"] = "-1"; 
                    source["BugLevel"] = "HIGH"; 

                    result["Sources"].push_back(source);
                }
            }
        }

        // Ensure the structure has "Sinks" key even if no sink is found
        if (result["Sinks"].empty()) {
            result["Sinks"] = nlohmann::json(nlohmann::json::array());
        }

        if (result["Sources"].empty()) {
            result["Sources"] = nlohmann::json(nlohmann::json::array());
        }

        // Test the result
        // std::cout << "Constructed JSON: " << result.toStyledString() << std::endl;

        // Save the result
        std::string debug_filename = "/tmp/Solr_CVE_2018_1308_" + std::to_string(source_id) + "_" + std::to_string(sink_id) + ".json";
        saveJSON(result, debug_filename.c_str());
        // std::cout << "JSON saved to " << debug_filename << std::endl;

        return result;
    }

    void saveJSON(const nlohmann::json& json, const char* filename) {
        std::ofstream file(filename);
        file << json.dump();
        file.close();
    }

#ifdef __cplusplus
}
#endif
