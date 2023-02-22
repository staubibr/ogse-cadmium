//
// Created by maddmel on 2/10/2023.
//

#ifndef CADMIUM_CBM_GDP_CALCULATOR_HPP
#define CADMIUM_CBM_GDP_CALCULATOR_HPP

#include <iostream>
#include <string>
#include <utility>
#include <json.hpp>
#include <cmath>
#include <iomanip>

#include <cadmium/core/modeling/atomic.hpp>
#include <cadmium/core/modeling/port.hpp>
#include <util.hpp>
#include <models/messages/packet_oil.hpp>

// json file parameters
struct gdp_cell_params {
    std::vector<long> gdp_hist = {0, 0, 0}; // gdp history (date asc t-3, t-2, t-1)
};

// linear model coefficients
struct model_coefficients {
    double intercept = 0.00206;
    double c_dln_gdp_tm1 = -0.174;
    double c_dln_gdp_tm2 = -0.233;
    double c_dln_price_t = 0.266;
    double c_dln_price_tm1 = 0.189;
    double c_dln_price_tm2 = 0.119;
};

// model state
struct gdp_cell_state {
    long gdp = 0; // gdp calculated from lm formula
    long mom = 0; // month over month change
    double mom_gr = 0.0; // month over month growth rate

    int prices_received = 0; // counter for number of oil prices received

    std::vector<long> gdp_hist = {0, 0, 0};  // gdp history (date asc t-3, t-2, t-1)
    std::vector<double> price_hist = {0.0, 0.0, 0.0, 0.0}; // oil price history (date asc t-3, t-2, t-1, t)
};

std::ostream& operator<<(std::ostream &out, const gdp_cell_state& s) {
    return out << s.gdp << ", " << s.mom << ", " << std::fixed << std::setprecision(2) << s.mom_gr;
}

class gdp_cell: public cadmium::Atomic<gdp_cell_state> {
private:
    gdp_cell_params params;
    model_coefficients m;
public:
    cadmium::BigPort<packet_oil> gdp_cell_in;

    gdp_cell(const std::string& id, const nlohmann::json& j_params): cadmium::Atomic<gdp_cell_state>(id, gdp_cell_state()) {

        // create input port
        gdp_cell_in = addInBigPort<packet_oil>("gdp_cell_in");

        // get paramaters from json
        params.gdp_hist = j_params.at("gdp_hist").get<std::vector<long>>();

        // save gdp history to state
        state.gdp_hist = params.gdp_hist;

    }

    void internalTransition(gdp_cell_state& s) const override {

    }

    void externalTransition(gdp_cell_state& s, double e) const override {

        // ln difference values for model
        double dln_gdp_t = 0.0;
        double dln_gdp_tm1 = 0.0;
        double dln_gdp_tm2 = 0.0;
        double dln_price_t = 0.0;
        double dln_price_tm1 = 0.0;
        double dln_price_tm2 = 0.0;

        for (const auto &i : gdp_cell_in->getBag()) {

            // shift price values to left as we receive new prices
            std::rotate(s.price_hist.begin(), s.price_hist.begin() + 1, s.price_hist.end());
            s.price_hist.back() = i->price;

            s.prices_received += 1;

            // start calculating gdp after first 4 prices received
            if (s.prices_received >= 4) {

                // calculate dln oil prices at time t, t-1, t-2
                dln_price_t = log(s.price_hist[3]) - log(s.price_hist[2]); // t
                dln_price_tm1 = log(s.price_hist[2]) - log(s.price_hist[1]); // t-1
                dln_price_tm2 = log(s.price_hist[1]) - log(s.price_hist[0]); // t-2

                // calculate dln gdp values at time t-1, t-2
                dln_gdp_tm1 = log(s.gdp_hist[2]) - log(s.gdp_hist[1]); // t-1
                dln_gdp_tm2 = log(s.gdp_hist[1]) - log(s.gdp_hist[0]); // t-2

                // use linear model formula to calculate dln gdp at time t
                dln_gdp_t = m.intercept + (m.c_dln_gdp_tm1 * dln_gdp_tm1) + (m.c_dln_gdp_tm2 * dln_gdp_tm2) +
                        (m.c_dln_price_t * dln_price_t) + (m.c_dln_price_tm1 * dln_price_tm1) +
                        (m.c_dln_price_tm2 * dln_price_tm2);

                // back calculate gdp from dln gdp
                s.gdp = lround(exp( log(s.gdp_hist[2]) + dln_gdp_t));

                // calculate month over month info
                s.mom = s.gdp - s.gdp_hist[2];
                s.mom_gr = double(s.mom) / s.gdp_hist[2] * 100;

                // shift gdp values to left and save calculated value to gdp history
                std::rotate(s.gdp_hist.begin(), s.gdp_hist.begin() + 1, s.gdp_hist.end());
                s.gdp_hist.back() = s.gdp;

            }

        }

    }

    void output(const gdp_cell_state& s) const override {

        // no outputs, sends value through state

    }

    [[nodiscard]] double timeAdvance(const gdp_cell_state& s) const override {

        // always infinity because this model only responds to receiving oil prices
        return std::numeric_limits<double>::infinity();

    }
};

#endif //CADMIUM_CBM_GDP_CALCULATOR_HPP