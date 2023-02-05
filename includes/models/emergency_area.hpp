#ifndef CADMIUM_EXAMPLE_EMERGENCY_AREA_HPP
#define CADMIUM_EXAMPLE_EMERGENCY_AREA_HPP

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <json.hpp>

#include <cadmium/core/modeling/atomic.hpp>
#include <cadmium/core/modeling/port.hpp>
#include <models/messages/emergency.hpp>
#include <util.hpp>

struct emergency_area_params {
	std::string id; 		// geo area id
	int population = 0;		// population count for geo area
	int emergency_max = 0;	// maximum number of emergency patients in one output
	int n_ports = 0;		// number of connected hospitals

    // TODO: from_json for nlohmann maybe ?
};

struct emergency_area_state {
	int dead = 0;			// number of dead population
	int port_i = 1;			// next hospital to be targeted, by index
	int quantity = 0;
};

std::ostream& operator<<(std::ostream &out, const emergency_area_state& s) {
    out << s.dead;
    return out;
}

class emergency_area: public cadmium::Atomic<emergency_area_state> {
private:
    emergency_area_params params;

public:
    cadmium::BigPort<emergency_t> in_rejected;
    cadmium::BigPort<emergency_t> out_hospital_1;
    cadmium::BigPort<emergency_t> out_hospital_2;
    cadmium::BigPort<emergency_t> out_hospital_3;

    emergency_area(const std::string& id, nlohmann::json j_params): cadmium::Atomic<emergency_area_state>(id, emergency_area_state()) {
        in_rejected = addInBigPort<emergency_t>("rejected_1");
        out_hospital_1 = addOutBigPort<emergency_t>("out_1");
        out_hospital_2 = addOutBigPort<emergency_t>("out_2");
        out_hospital_3 = addOutBigPort<emergency_t>("out_3");

        // 1.9 is a bit, high from surface research, according to Ottawa Hospital rates, it should be more like 1.6
        // But areas generate emergencies with some random factor.
        double rate = 1.9 / 1000;

		params.id = j_params.at("dauid").get<std::string>();
		params.population = j_params.at("DApop_2016").get<int>();
		params.n_ports = 3;

		params.emergency_max = utility::round_to_int(rate * params.population);

		// generate an initial count for the first emergency to output.
    	// It will be output before the first internal transition
		state.quantity = utility::random(1, params.emergency_max);
    }

    void internalTransition(emergency_area_state& s) const override {
    	if (s.quantity == 0 && params.emergency_max > 0) {
    		s.port_i = 1;
    		s.quantity = utility::random(1, params.emergency_max);
    	}

    	else if (s.port_i > params.n_ports) {
			s.dead += s.quantity;
    		s.port_i = 1;
    		s.quantity = 0;
    	}

    	else {
    		s.quantity = 0;
    	}
    }
	
    void externalTransition(emergency_area_state& s, double e) const override {
    	const emergency_t * p_em = nullptr;

    	// One area can be linked to max 3 hospitals through separate ports. However, one hospital may be
    	// linked back to a variable number of areas. Since the number of ports varies and Cadmium doesn't
    	// support variable number of ports, each hospital rejects emergencies to all connected areas.
    	// Therefore, a model can receive multiple rejections simultaneously but, only one should target
    	// the current area at a given time because processing in hospitals is instantaneous.

		for (const auto &i : in_rejected->getBag()) {
			if (i->area_id != params.id) continue;

			// Should only receive one emergency targeted to this area per time unit.
			assert(!p_em);

			p_em = i.get();
		}

		if (!p_em) return;

    	// Emergency was only partially processed, the remaining must be sent to another port if possible.
        s.quantity = p_em->quantity;
    	s.port_i++;
    }
	
    void output(const emergency_area_state& s) const override {
    	// Check that there is an emergency and that there is a hospital left to send it to
    	if (s.quantity == 0 || s.port_i > params.n_ports) return;

    	// This model sends the message to the first port. If it is rejected, then it'll try the second port
    	// To do this, we track the current port in the state.target variable. A value of -1 indicates that
    	// it was rejected by all ports. We set the value to 0 to start the cycle.
    	emergency_t em = emergency_t(params.id, s.port_i, s.quantity);


		if (s.port_i == 1) out_hospital_1->addMessage(em);
		else if (s.port_i == 2) out_hospital_2->addMessage(em);
		else if (s.port_i == 3) out_hospital_3->addMessage(em);
    }
	
    [[nodiscard]] double timeAdvance(const emergency_area_state& s) const override {
    	if (s.quantity > 0) return 0;

    	// In 24h, the model outputs the emergency (1 represents one day)
    	else return 1;
    }
};

#endif // CADMIUM_EXAMPLE_EMERGENCY_AREA_HPP