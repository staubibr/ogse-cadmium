//
// Created by staub on 2023-01-30.
//

#ifndef CADMIUM_CBM_SCENARIO_HPP
#define CADMIUM_CBM_SCENARIO_HPP

#include "instances_set.hpp"
#include "couplings_set.hpp"

namespace data_structures {
    class scenario {

    public:
        scenario();

        std::vector<instances_set> instances;
        std::vector<couplings_set> couplings;
    };

    scenario::scenario() = default;

    inline void to_json(nlohmann::json &j, const scenario &s)
    {
        j["instances"] = s.instances;
        j["couplings"] = s.couplings;
    }

    inline void from_json(const nlohmann::json &j, scenario &s)
    {
        s.instances = j.at("instances").get<std::vector<instances_set>>();
        s.couplings = j.at("couplings").get<std::vector<couplings_set>>();
    }
}

#endif //CADMIUM_CBM_SCENARIO_HPP
