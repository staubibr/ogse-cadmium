//
// Created by staub on 2023-02-03.
//

#ifndef CADMIUM_CBM_PACKET_HPP
#define CADMIUM_CBM_PACKET_HPP

#include <string>
#include <iostream>

struct packet_m {
    int data;

    packet_m(): data(0) {}

    explicit packet_m(int data) : data(data) {}
};

std::ostream& operator<<(std::ostream &out, const packet_m& e) {
    out << e.data;

    return out;
}
#endif //CADMIUM_CBM_PACKET_HPP
