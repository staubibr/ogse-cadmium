//
// Created by maddmel on 2/16/2023.
//

#ifndef CADMIUM_CBM_PACKET_OIL_HPP
#define CADMIUM_CBM_PACKET_OIL_HPP

#include <string>
#include <iostream>

// message structure for oil price generator model
struct packet_oil {

    double price;

    packet_oil(): price(0.0) {}

    explicit packet_oil(double price) : price(price) {}
};

std::ostream& operator<<(std::ostream &out, const packet_oil& e) {
    out << e.price;

    return out;
}

#endif //CADMIUM_CBM_PACKET_OIL_HPP
