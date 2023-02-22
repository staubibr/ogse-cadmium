//
// Created by maddmel on 2/10/2023.
//

#ifndef CADMIUM_CBM_OIL_PRICE_GENERATOR_HPP
#define CADMIUM_CBM_OIL_PRICE_GENERATOR_HPP

#include <iostream>
#include <string>
#include <utility>
#include <json.hpp>
#include <vector>

#include <cadmium/core/modeling/atomic.hpp>
#include <cadmium/core/modeling/port.hpp>
#include <util.hpp>
#include <models/messages/packet_oil.hpp>

// structure for holding parameters from json file
struct oil_price_generator_params {
    int price_index = 0;                // index to start issuing prices from (0 based)
    std::vector<double> prices = {};    // oil prices (date asc)
};

// structure for holding the current model state
struct oil_price_generator_state {
    double price = 0.0;
    int price_index = 1;
    double time_advance = 1.0; // 1 month
};

std::ostream& operator<<(std::ostream &out, const oil_price_generator_state& s) {
    // no state output required
    return out;
}

class oil_price_generator: public cadmium::Atomic<oil_price_generator_state> {
private:
    oil_price_generator_params params;
public:
    cadmium::BigPort<packet_oil> oil_price_generator_out;

    oil_price_generator(const std::string& id, const nlohmann::json& j_params): cadmium::Atomic<oil_price_generator_state>(id, oil_price_generator_state()) {

        // create output port
        oil_price_generator_out = addOutBigPort<packet_oil>("oil_price_generator_out");

        // get paramaters from json
        params.price_index = j_params.at("price_index").get<int>();
        params.prices = j_params.at("prices").get<std::vector<double>>();

        // save price at current index to state
        state.price_index = params.price_index;
        state.price = params.prices[state.price_index];

    }

    void internalTransition(oil_price_generator_state& s) const override {

        // after output, get next price for state
        s.price_index += 1;
        s.price = params.prices[s.price_index];
        s.time_advance = 1;

    }

    void externalTransition(oil_price_generator_state& s, double e) const override {
        // no input port required
    }

    void output(const oil_price_generator_state& s) const override {

        // send price to output port
        auto p = packet_oil(s.price );

        oil_price_generator_out->addMessage(p);

    }

    [[nodiscard]] double timeAdvance(const oil_price_generator_state& s) const override {

        // if there are no more prices, set time to infinity to stop sending
        return state.price_index < params.prices.size() - 1 ? s.time_advance : std::numeric_limits<double>::infinity();

    }
};

#endif //CADMIUM_CBM_OIL_PRICE_GENERATOR_HPP


