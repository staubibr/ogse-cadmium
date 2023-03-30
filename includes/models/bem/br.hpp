#ifndef CADMIUM_EXAMPLE_BEM_BR_HPP_
#define CADMIUM_EXAMPLE_BEM_BR_HPP_

#include <cadmium/core/modeling/atomic.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <json.hpp>

namespace cadmium::example::bem {

    struct BrParams {
        double area = 0;
        std::string bt;		//building type for factors: Single Detached, Single Attached, Apartments, Mobile Homes
        double eid = 0;
    };

    struct BrState {
		double energy = 0;
		
		BrState() = default;
	};

    std::ostream& operator<<(std::ostream& out, const BrState& s) {
        out << s.energy;

        return out;
    }
	
	class Br : public Atomic<BrState> {
    private:
        BrParams params{};

    public:
		Port<double> in1;
		Port<double> out2;
		
		Br(const std::string& id, nlohmann::json j_params): Atomic<BrState>(id, BrState()) {
            in1 = addInPort<double>("in1");
            out2 = addOutPort<double>("out2");

            params.area = j_params.at("area").get<double>();
            params.bt = j_params.at("bt").get<std::string>();

            if (params.bt == "Single Detached") params.eid = 0.71;
            else if (params.bt == "Single Attached") params.eid = 0.64;
            else if (params.bt == "Apartments") params.eid = 0.58;
            else if (params.bt == "Mobile Homes") params.eid = 0.96;
		}

		void internalTransition(BrState& s) const override {
			// s.energy = -1 ;
		}

		void externalTransition(BrState& s, double e) const override {
            for (auto &ftr: in1->getBag()) {
                s.energy = params.area * params.eid * ftr;
            }
        }

		void output(const BrState& s) const override {
			// out2->addMessage(s.energy);
		}

		[[nodiscard]] double timeAdvance(const BrState& s) const override {
           // if (s.energy > 0) return 0;

            // else

                return std::numeric_limits<double>::infinity();
		}
	};
}  

#endif //CADMIUM_EXAMPLE_BEM_BR_HPP_
