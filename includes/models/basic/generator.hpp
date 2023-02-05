//
// Created by staub on 2023-02-03.
//

#ifndef CADMIUM_CBM_GENERATOR_HPP
#define CADMIUM_CBM_GENERATOR_HPP

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <json.hpp>

#include <cadmium/core/modeling/atomic.hpp>
#include <cadmium/core/modeling/port.hpp>
#include <util.hpp>
#include <models/messages/packet.hpp>

struct generator_params {
    int rate = 1;
    int min = 0;
    int max = 0;
};

struct generator_state {
    int value;
};

std::ostream& operator<<(std::ostream &out, const generator_state& s) {
    return out;
}

class generator: public cadmium::Atomic<generator_state> {
private:
    generator_params params;
public:
    cadmium::BigPort<packet_m> generator_out;

    generator(const std::string& id, const nlohmann::json& j_params): cadmium::Atomic<generator_state>(id, generator_state()) {
        generator_out = addOutBigPort<packet_m>("generator_out");

        params.rate = j_params.at("rate").get<int>();
        params.min = j_params.at("min").get<int>();
        params.max = j_params.at("max").get<int>();

        state.value = utility::random(params.min, params.max);
    }

    // TODO: Why prevent user from modifying this->state here?
    void internalTransition(generator_state& s) const override {
        s.value = utility::random(params.min, params.max);
    }

    void externalTransition(generator_state& s, double e) const override {
        // never receives anything
    }

    void output(const generator_state& s) const override {
        auto p = packet_m(state.value);

        // TODO: why can't ports use messages with a common base class instead of templates?
        generator_out->addMessage(p);
    }

    [[nodiscard]] double timeAdvance(const generator_state& s) const override {
        return 24.0 / params.rate;
    }
};

#endif //CADMIUM_CBM_GENERATOR_HPP
