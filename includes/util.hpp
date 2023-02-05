#ifndef CADMIUM_TOOLS_HPP
#define CADMIUM_TOOLS_HPP

#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include <random>


namespace utility {
    static int random(int min, int max) {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(min, max);

        return int(dist(mt));
    }

    static int round_to_int(double x) {
        // 1.2 >> 1.7 >> 1
        // 1.6 >> 2.1 >> 2
        // -1.2 >> -0.7 >> -1.7 >> -1
        // -1.6 >> -1.1 >> -2.1 >> -2
        return int(x + 0.5 - (x < 0));
    }

    static std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> splits;
        std::string split;
        std::istringstream ss(s);

        while (std::getline(ss, split, delimiter)) splits.push_back(split);

        return splits;
    }
}

#endif //CADMIUM_TOOLS_HPP