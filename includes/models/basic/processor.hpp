//
// Created by staub on 2023-02-03.
//

#ifndef CADMIUM_CBM_PROCESSOR_HPP
#define CADMIUM_CBM_PROCESSOR_HPP

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <json.hpp>

#include <cadmium/core/modeling/atomic.hpp>
#include <cadmium/core/modeling/port.hpp>
#include "models/messages/packet.hpp"

struct processor_params {
    int multiplier = 0;
    double delay = 0.0;
};

struct processor_state {
    int value = 0;
    double time_advance = 0.0;
};

std::ostream& operator<<(std::ostream &out, const processor_state& s) {
    return out;
}

class processor: public cadmium::Atomic<processor_state> {
private:
    processor_params params;
public:
    cadmium::BigPort<packet_m> processor_in;
    cadmium::BigPort<packet_m> processor_out;

    processor(const std::string& id, const nlohmann::json& j_params): cadmium::Atomic<processor_state>(id, processor_state()) {
        // TODO: Why do we need small and big port? Why not use the big port approach for all?
        processor_in = addInBigPort<packet_m>("processor_in");
        processor_out = addOutBigPort<packet_m>("processor_out");

        params.multiplier = j_params.at("multiplier").get<int>();
        params.delay = j_params.at("delay").get<double>();

        state.time_advance = params.delay;
    }

    void internalTransition(processor_state& s) const override {
        s.value = 0;
        s.time_advance = params.delay;
    }

    // Not sure about this but, why not trigger one external transition per message received ?
    void externalTransition(processor_state& s, double e) const override {
        for (const auto &i : processor_in->getBag()) {
            s.value = s.value + params.multiplier * i->data;
        }

        s.time_advance = e;
    }

    void output(const processor_state& s) const override {
        auto p = packet_m(state.value);

        processor_out->addMessage(p);
    }

    [[nodiscard]] double timeAdvance(const processor_state& s) const override {
        return s.value > 0 ? s.time_advance : std::numeric_limits<double>::infinity();
    }
};

#endif //CADMIUM_CBM_PROCESSOR_HPP
