//
// Created by staub on 2023-01-31.
//

#ifndef CADMIUM_CBM_MODEL_FACTORY_HPP
#define CADMIUM_CBM_MODEL_FACTORY_HPP

#include <string>
#include <memory>
#include <map>
#include <functional>
#include <json.hpp>
#include <cadmium/core/modeling/component.hpp>

#include <models/hospital.hpp>
#include <models/emergency_area.hpp>
#include <models/basic/generator.hpp>
#include <models/basic/processor.hpp>
#include <models/basic/accumulator.hpp>
#include <models/oil_gdp/oil_price_generator.hpp>
#include <models/oil_gdp/gdp_cell.hpp>

namespace ogse {
    class model_factory {

    private:
        // NOTE: map of string and pointer to function which returns a component pointer.
        inline static std::map<std::string, std::function<std::shared_ptr<cadmium::Component>(std::string id, nlohmann::json json)>> map;

        template<typename T>
        static std::shared_ptr<T> createT(const std::string& id, const nlohmann::json& json) {
            return std::make_shared<T>(id, json);
        }

    public:
        template<typename T>
        static void register_model(const std::string& s) {
            map.insert(std::make_pair(s, &createT<T>));
        }

        static void initialize() {
            model_factory::register_model<hospital>("89b31233-5f69-4bed-9aa4-3d0051fdfab8");
            model_factory::register_model<emergency_area>("71b7ac8b-b30b-4e67-b306-2332e27995d0");
            model_factory::register_model<generator>("uuid_test_1");
            model_factory::register_model<processor>("uuid_test_2");
            model_factory::register_model<accumulator>("uuid_test_3");
            model_factory::register_model<oil_price_generator>("26066ec1-8150-498e-9396-cf9af98a45d8");
            model_factory::register_model<gdp_cell>("21567c86-34f0-4f13-a6f0-ddc05ca45fe5");
        }

        static bool contains(const std::string& s) {
            return map.find(s) != map.end();
        }

        static std::shared_ptr<cadmium::Component> build(const std::string& s, const std::string& id, const nlohmann::json& json) {
            auto it = map.find(s);

            if (it == map.end()) return nullptr;

            return it->second(id, json);
        }
    };
};


#endif //CADMIUM_CBM_MODEL_FACTORY_HPP
