//
// Created by staub on 2023-02-03.
//

#ifndef CADMIUM_CBM_ACCUMULATOR_HPP
#define CADMIUM_CBM_ACCUMULATOR_HPP

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <json.hpp>

#include <cadmium/core/modeling/atomic.hpp>
#include <cadmium/core/modeling/port.hpp>
#include "models/messages/packet.hpp"

struct accumulator_params {

};

struct accumulator_state {
    int total = 0;
};

std::ostream& operator<<(std::ostream &out, const accumulator_state& s) {
    out << s.total;

    return out;
}

class accumulator: public cadmium::Atomic<accumulator_state> {
private:
    accumulator_params params;
public:
    cadmium::BigPort<packet_m> accumulator_in;

    accumulator(const std::string& id, [[maybe_unused]] const nlohmann::json& j_params): cadmium::Atomic<accumulator_state>(id, accumulator_state()) {
        accumulator_in = addInBigPort<packet_m>("accumulator_in");
    }

    void internalTransition(accumulator_state& s) const override {
        // never does internal transitions
    }

    void externalTransition(accumulator_state& s, double e) const override {
        for (const auto &i : accumulator_in->getBag()) {
            s.total = s.total + i->data;
        }
    }

    void output(const accumulator_state& s) const override {
        // never outputs messages, only through state on each simulation transition
    }

    [[nodiscard]] double timeAdvance(const accumulator_state& s) const override {
        return std::numeric_limits<double>::infinity();
    }
};
#endif //CADMIUM_CBM_ACCUMULATOR_HPP
