#ifndef CADMIUM_EXAMPLE_HOSPITAL_HPP
#define CADMIUM_EXAMPLE_HOSPITAL_HPP

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <json.hpp>

#include <cadmium/core/modeling/atomic.hpp>
#include <cadmium/core/modeling/port.hpp>
#include <models/messages/emergency.hpp>

// model parameters
struct hospital_params {
	std::string id;
    std::string name;
	int rate = 0;
	int capacity = 0;
};

struct hospital_state {
    int active = 0;
    int total = 0;
    int released = 0;
    int rejected = 0;

    std::vector<emergency_t> emergencies;
};

std::ostream& operator<<(std::ostream &out, const hospital_state& s) {
	out << s.active << "," << s.total << "," << s.released << "," << s.rejected;

	return out;
}

class hospital: public cadmium::Atomic<hospital_state> {
private:
    hospital_params params;
public:
    cadmium::BigPort<emergency_t> processor_in;
    cadmium::BigPort<emergency_t> processor_out;

    hospital(const std::string& id, nlohmann::json j_params): cadmium::Atomic<hospital_state>(id, hospital_state()) {
        processor_in = addInBigPort<emergency_t>("processor_in");
        processor_out = addOutBigPort<emergency_t>("processor_out");

		params.id = j_params.at("index").get<std::string>();
		params.name = j_params.at("facility_name").get<std::string>();
		params.rate = j_params.at("rate").get<int>();
		params.capacity = j_params.at("capacity").get<int>();
    }

    void internalTransition(hospital_state& s) const override {
    	if (!s.emergencies.empty()) s.emergencies.clear();

    	else {
            int released = params.rate > s.active ? s.active : params.rate;

            s.active -= released;
            s.released += released;
    	}
    }

    void externalTransition(hospital_state& s, double e) const override {


        for (const auto &i : processor_in->getBag()) {
            s.active += i->quantity;
            s.total += i->quantity;

            int rejected = s.active > params.capacity ? s.active - params.capacity : 0;

            s.rejected += rejected;
            s.active -= rejected;

            if (rejected > 0) {
                s.emergencies.emplace_back(i->area_id, i->port_i, rejected);
            }
        }
    }

    void output(const hospital_state& s) const override {
		for(const auto &em : s.emergencies){
            processor_out->addMessage(em);
		}
    }

    [[nodiscard]] double timeAdvance(const hospital_state& s) const override {
        if (!s.emergencies.empty()) return 0;

        if (s.active > 0) return 1;

        else return std::numeric_limits<double>::infinity();
    }
};

#endif //CADMIUM_EXAMPLE_HOSPITAL_HPP