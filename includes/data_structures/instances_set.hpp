//
// Created by staub on 2023-01-30.
//

#ifndef CADMIUM_CBM_INSTANCES_SET_HPP
#define CADMIUM_CBM_INSTANCES_SET_HPP

#include<string>
#include "json.hpp"

namespace data_structures {
    class instances_set {

    public:
        instances_set();

        std::string id;
        std::string type;
        std::vector<std::string> properties;
        std::vector<nlohmann::json> models;
    };

    instances_set::instances_set() = default;

    inline void to_json(nlohmann::json &j, const instances_set &s)
    {
        j["id"] = s.id;
        j["type"] = s.type;
        j["properties"] = s.properties;
        j["models"] = s.models;
    }

    inline void from_json(const nlohmann::json &j, instances_set &s)
    {
        s.id = j.at("id").get<std::string>();
        s.type = j.at("type").get<std::string>();
        s.properties = j.at("properties").get<std::vector<std::string>>();
        s.models = j.at("models").get<std::vector<nlohmann::json>>();
    }
}


#endif //CADMIUM_CBM_INSTANCES_SET_HPP
