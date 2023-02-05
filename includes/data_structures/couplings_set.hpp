//
// Created by staub on 2023-01-30.
//

#ifndef CADMIUM_CBM_COUPLINGS_SET_HPP
#define CADMIUM_CBM_COUPLINGS_SET_HPP

#include<string>
#include "json.hpp"

namespace data_structures {
    class couplings_set {

    public:
        couplings_set();

        std::string from_model;
        std::string from_port;
        std::string to_model;
        std::string to_port;
        std::vector<std::vector<std::string>> couplings;
    };

    couplings_set::couplings_set() = default;

    inline void to_json(nlohmann::json &j, const couplings_set &s)
    {
        j["from_model"] = s.from_model;
        j["from_port"] = s.from_port;
        j["to_model"] = s.to_model;
        j["to_port"] = s.to_port;
        j["couplings"] = s.couplings;
    }

    inline void from_json(const nlohmann::json &j, couplings_set &s)
    {
        s.from_model = j.at("from_model").get<std::string>();
        s.from_port = j.at("from_port").get<std::string>();
        s.to_model = j.at("to_model").get<std::string>();
        s.to_port = j.at("to_port").get<std::string>();
        s.couplings = j.at("couplings").get<std::vector<std::vector<std::string>>>();
    }
}


#endif //CADMIUM_CBM_COUPLINGS_SET_HPP
