#ifndef CADMIUM_EXAMPLE_BEM_BC_HPP_
#define CADMIUM_EXAMPLE_BEM_BC_HPP_

#include <cadmium/core/modeling/atomic.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <json.hpp>

namespace cadmium::example::bem {

    struct BcParams {
        double area = 0;		//total area of the building
        double eid = 0;			//energy intensity per meter square in kilowatt-hour per day
        std::string bt;		    //building type for factors: Office buildings (non-medical), Medical office buildings, Elementary and/or secondary schools, Assisted daily/residential care facilities, Warehouses, Hotels, motels or lodges, Hospitals, Food and beverage stores, Non-food retail stores, Other activity or function
        int nof = 0;			//number of floor
        int yoc = 0;			//year of construction
    };

	struct BcState {
		double energy = 0;	//energy consumption
		
		BcState(): energy() {}
	};

    std::ostream& operator<<(std::ostream& out, const BcState& s) {
        out << s.energy;

        return out;
    }

	class Bc : public Atomic<BcState> {
    private:
        BcParams params;
	                   
	 public:
		Port<double> in1;
		Port<double> out2;
		
		Bc(const std::string& id, nlohmann::json j_params): Atomic<BcState>(id, BcState()){
            in1 = addInPort<double>("in1");
            out2 = addOutPort<double>("out2");

            params.area = j_params.at("area").get<double>();
			params.bt = j_params.at("bt").get<std::string>();
            params.nof = j_params.at("nof").get<int>();
            params.yoc = j_params.at("yoc").get<int>();

            if (params.bt == "Educational Services") params.eid = 1.52;
            else if (params.bt == "Health Care and Social Assistance") params.eid = 2.57;
            else if (params.bt == "Information and Cultural Industries") params.eid = 1.41;
            else if (params.bt == "Offices") params.eid = 1.36;
            else if (params.bt == "Retail Trade") params.eid = 1.49;
            else if (params.bt == "Accommodation and Food Services") params.eid = 2.17;
            else if (params.bt == "Arts, Entertainment and Recreation") params.eid = 1.59;
            else if (params.bt == "Transportation and Warehousing") params.eid = 1.35;
            else if (params.bt == "Wholesale Trade") params.eid = 1.38;
            else if (params.bt == "Other Services") params.eid = 1.37;
		}

		void internalTransition(BcState& s) const override {
			// s.energy = -1 ;
		}

		void externalTransition(BcState& s, double e) const override {
            double nff;			//number of floor factor
            double ycf;			//year of construction factor

            for (auto &ftr: in1->getBag()) {
                // if (params.bt == "Office buildings (non-medical)") {
                if (params.bt == "Offices") {
                    if (params.nof == 1) nff = 1.10;
                    else if (params.nof == 2) nff = 0.89;
                    else if (params.nof == 3) nff = 1.00;
                    else if (params.nof >= 4 && params.nof <= 9) nff = 0.89;
                    else if (params.nof >= 10) nff = 0.97;

                    if (params.yoc < 1920) ycf = 1.12;
                    else if (params.yoc >= 1920 && params.yoc <= 1959) ycf = 1.16;
                    else if (params.yoc >= 1960 && params.yoc <= 1969) ycf = 0.96;
                    else if (params.yoc >= 1970 && params.yoc <= 1979) ycf = 1.09;
                    else if (params.yoc >= 1980 && params.yoc <= 1989) ycf = 0.86;
                    else if (params.yoc >= 1990 && params.yoc <= 1999) ycf = 1.00;
                    else if (params.yoc >= 2000 && params.yoc <= 2009) ycf = 1.01;
                    else if (params.yoc >= 2010) ycf = 0.80;
                }

                // else if (params.bt == "Medical office buildings") {
                else if (params.bt == "Health Care and Social Assistance") {
                    if (params.nof == 1) nff = 0.83;
                    else if (params.nof == 2) nff = 0.85;
                    else if (params.nof == 3) nff = 1.10;
                    else if (params.nof >= 4 && params.nof <= 9) nff = 0.93;
                    else if (params.nof >= 10) nff = 1.29;

                    if (params.yoc < 1920) ycf = 0.79;
                    else if (params.yoc >= 1920 && params.yoc <= 1959) ycf = 1.05;
                    else if (params.yoc >= 1960 && params.yoc <= 1969) ycf = 1.35;
                    else if (params.yoc >= 1970 && params.yoc <= 1979) ycf = 1.09;
                    else if (params.yoc >= 1980 && params.yoc <= 1989) ycf = 0.77;
                    else if (params.yoc >= 1990 && params.yoc <= 1999) ycf = 1.15;
                    else if (params.yoc >= 2000 && params.yoc <= 2009) ycf = 0.82;
                    else if (params.yoc >= 2010) ycf = 0.98;
                }

                // else if (params.bt == "Elementary and/or secondary schools") {
                else if (params.bt == "Educational Services") {
                    if (params.nof == 1) nff = 1.10;
                    else if (params.nof == 2) nff = 0.92;
                    else if (params.nof == 3) nff = 1.03;
                    else if (params.nof >= 4 && params.nof <= 9) nff = 0.96;
                    else if (params.nof >= 10) nff = 1.00;

                    if (params.yoc < 1920) ycf = 0.90;
                    else if (params.yoc >= 1920 && params.yoc <= 1959) ycf = 0.94;
                    else if (params.yoc >= 1960 && params.yoc <= 1969) ycf = 1.13;
                    else if (params.yoc >= 1970 && params.yoc <= 1979) ycf = 0.88;
                    else if (params.yoc >= 1980 && params.yoc <= 1989) ycf = 1.01;
                    else if (params.yoc >= 1990 && params.yoc <= 1999) ycf = 0.98;
                    else if (params.yoc >= 2000 && params.yoc <= 2009) ycf = 0.93;
                    else if (params.yoc >= 2010) ycf = 1.23;
                }

                else if (params.bt == "Assisted daily/residential care facilities") {
                    if (params.nof == 1) nff = 1.41;
                    else if (params.nof == 2) nff = 0.96;
                    else if (params.nof == 3) nff = 0.87;
                    else if (params.nof >= 4 && params.nof <= 9) nff = 1.08;
                    else if (params.nof >= 10) nff = 0.68;

                    if (params.yoc < 1920) ycf = 1.05;
                    else if (params.yoc >= 1920 && params.yoc <= 1959) ycf = 1.01;
                    else if (params.yoc >= 1960 && params.yoc <= 1969) ycf = 1.07;
                    else if (params.yoc >= 1970 && params.yoc <= 1979) ycf = 1.18;
                    else if (params.yoc >= 1980 && params.yoc <= 1989) ycf = 1.14;
                    else if (params.yoc >= 1990 && params.yoc <= 1999) ycf = 0.96;
                    else if (params.yoc >= 2000 && params.yoc <= 2009) ycf = 1.02;
                    else if (params.yoc >= 2010) ycf = 0.58;
                }

                // else if (params.bt == "Warehouses") {
                else if (params.bt == "Transportation and Warehousing") {
                    if (params.nof == 1) nff = 1.12;
                    else if (params.nof == 2) nff = 1.11;
                    else if (params.nof == 3) nff = 1.06;
                    else if (params.nof >= 4 && params.nof <= 9) nff = 0.81;
                    else if (params.nof >= 10) nff = 0.89;

                    if (params.yoc < 1920) ycf = 0.58;
                    else if (params.yoc >= 1920 && params.yoc <= 1959) ycf = 1.08;
                    else if (params.yoc >= 1960 && params.yoc <= 1969) ycf = 0.99;
                    else if (params.yoc >= 1970 && params.yoc <= 1979) ycf = 1.17;
                    else if (params.yoc >= 1980 && params.yoc <= 1989) ycf = 1.10;
                    else if (params.yoc >= 1990 && params.yoc <= 1999) ycf = 1.19;
                    else if (params.yoc >= 2000 && params.yoc <= 2009) ycf = 0.89;
                    else if (params.yoc >= 2010) ycf = 1.00;
                }

                else if (params.bt == "Accommodation and Food Services") {
                // else if (params.bt == "Hotels, motels or lodges") {
                    if (params.nof == 1) nff = 0.89;
                    else if (params.nof == 2) nff = 1.10;
                    else if (params.nof == 3) nff = 0.77;
                    else if (params.nof >= 4 && params.nof <= 9) nff = 1.09;
                    else if (params.nof >= 10) nff = 1.15;

                    if (params.yoc < 1920) ycf = 1.35;
                    else if (params.yoc >= 1920 && params.yoc <= 1959) ycf = 0.85;
                    else if (params.yoc >= 1960 && params.yoc <= 1969) ycf = 1.17;
                    else if (params.yoc >= 1970 && params.yoc <= 1979) ycf = 1.27;
                    else if (params.yoc >= 1980 && params.yoc <= 1989) ycf = 1.13;
                    else if (params.yoc >= 1990 && params.yoc <= 1999) ycf = 0.60;
                    else if (params.yoc >= 2000 && params.yoc <= 2009) ycf = 0.83;
                    else if (params.yoc >= 2010) ycf = 0.80;
                }

//                else if (params.bt == "Hospitals") {
//                    if (params.nof == 1) nff = 0.89;
//                    else if (params.nof == 2) nff = 1.06;
//                    else if (params.nof == 3) nff = 1.03;
//                    else if (params.nof >= 4 && params.nof <= 9) nff = 0.95;
//                    else if (params.nof >= 10) nff = 1.07;
//
//                    if (params.yoc < 1920) ycf = 0.83;
//                    else if (params.yoc >= 1920 && params.yoc <= 1959) ycf = 1.12;
//                    else if (params.yoc >= 1960 && params.yoc <= 1969) ycf = 1.01;
//                    else if (params.yoc >= 1970 && params.yoc <= 1979) ycf = 1.25;
//                    else if (params.yoc >= 1980 && params.yoc <= 1989) ycf = 0.87;
//                    else if (params.yoc >= 1990 && params.yoc <= 1999) ycf = 0.94;
//                    else if (params.yoc >= 2000 && params.yoc <= 2009) ycf = 0.99;
//                    else if (params.yoc >= 2010) ycf = 1.00;
//                }

//                else if (params.bt == "Food and beverage stores") {
//                    if (params.nof == 1) nff = 1.01;
//                    else if (params.nof == 2) nff = 1.43;
//                    else if (params.nof == 3) nff = 1.06;
//                    else if (params.nof >= 4 && params.nof <= 9) nff = 1.23;
//                    else if (params.nof >= 10) nff = 0.28;
//
//                    if (params.yoc < 1920) ycf = 0.57;
//                    else if (params.yoc >= 1920 && params.yoc <= 1959) ycf = 0.68;
//                    else if (params.yoc >= 1960 && params.yoc <= 1969) ycf = 1.58;
//                    else if (params.yoc >= 1970 && params.yoc <= 1979) ycf = 1.44;
//                    else if (params.yoc >= 1980 && params.yoc <= 1989) ycf = 0.79;
//                    else if (params.yoc >= 1990 && params.yoc <= 1999) ycf = 1.32;
//                    else if (params.yoc >= 2000 && params.yoc <= 2009) ycf = 1.12;
//                    else if (params.yoc >= 2010) ycf = 0.50;
//                }

               //  else if (params.bt == "Non-food retail stores") {
                else if (params.bt == "Retail Trade") {
                    if (params.nof == 1) nff = 1.16;
                    else if (params.nof == 2) nff = 0.78;
                    else if (params.nof == 3) nff = 0.82;
                    else if (params.nof >= 4 && params.nof <= 9) nff = 1.23;
                    else if (params.nof >= 10) nff = 1.02;

                    if (params.yoc < 1920) ycf = 1.40;
                    else if (params.yoc >= 1920 && params.yoc <= 1959) ycf = 0.94;
                    else if (params.yoc >= 1960 && params.yoc <= 1969) ycf = 1.07;
                    else if (params.yoc >= 1970 && params.yoc <= 1979) ycf = 0.72;
                    else if (params.yoc >= 1980 && params.yoc <= 1989) ycf = 0.99;
                    else if (params.yoc >= 1990 && params.yoc <= 1999) ycf = 1.11;
                    else if (params.yoc >= 2000 && params.yoc <= 2009) ycf = 1.08;
                    else if (params.yoc >= 2010) ycf = 0.68;
                }

                // else if (params.bt == "Other activity or function") {
                else {
                    if (params.nof == 1) nff = 1.15;
                    else if (params.nof == 2) nff = 1.09;
                    else if (params.nof == 3) nff = 0.98;
                    else if (params.nof >= 4 && params.nof <= 9) nff = 0.92;
                    else if (params.nof >= 10) nff = 0.87;

                    if (params.yoc < 1920) ycf = 0.87;
                    else if (params.yoc >= 1920 && params.yoc <= 1959) ycf = 1.13;
                    else if (params.yoc >= 1960 && params.yoc <= 1969) ycf = 0.88;
                    else if (params.yoc >= 1970 && params.yoc <= 1979) ycf = 1.34;
                    else if (params.yoc >= 1980 && params.yoc <= 1989) ycf = 0.76;
                    else if (params.yoc >= 1990 && params.yoc <= 1999) ycf = 0.89;
                    else if (params.yoc >= 2000 && params.yoc <= 2009) ycf = 1.19;
                    else if (params.yoc >= 2010) ycf = 0.92;
                }

                s.energy = (params.area * params.eid * nff * ycf) * ftr;
            }
        }

		void output(const BcState& s) const override {
			// out2->addMessage(s.energy);
		}

		[[nodiscard]] double timeAdvance(const BcState& s) const override {
            // if (s.energy >0) return 0;

            // else
            return std::numeric_limits<double>::infinity();
		}
	};
}  

#endif //CADMIUM_EXAMPLE_BEM_BC_HPP_
