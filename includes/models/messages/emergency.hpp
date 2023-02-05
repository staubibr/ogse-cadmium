//
// Created by staub on 2023-01-31.
//

#ifndef CADMIUM_CBM_EMERGENCY_HPP
#define CADMIUM_CBM_EMERGENCY_HPP

#include <string>
#include <iostream>

struct emergency_t {
    std::string area_id;
    int port_i;
    int quantity;

    emergency_t(): port_i(0), quantity(0) {}

    explicit emergency_t(std::string area_id, int port_i, int quantity) : area_id(std::move(area_id)), port_i(port_i), quantity(quantity) {}
};

std::ostream& operator<<(std::ostream &out, const emergency_t& e) {
    out << e.area_id << "," << e.port_i << "," << e.quantity;

    return out;
}

#endif //CADMIUM_CBM_EMERGENCY_HPP
