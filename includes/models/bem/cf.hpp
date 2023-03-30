#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-misleading-indentation"
#ifndef CADMIUM_EXAMPLE_BEM_CF_HPP_
#define CADMIUM_EXAMPLE_BEM_CF_HPP_

#include <cadmium/core/modeling/atomic.hpp>
#include <iostream>
#include <string>
#include <utility>
#include <json.hpp>


namespace cadmium::example::bem {

    struct CfDate {
        int day = 1;
        std::string weekday = "tuesday";
        std::string month = "january";
        int year = 2019;
    };

	struct CfState{
        CfDate date;
		double ftr = 0;
		
		CfState() = default;
	};

    std::ostream& operator<<(std::ostream& out, const CfState& s) {
        out << s.date.day << ","<< s.date.weekday << "," <<s.date.month << "," << s.date.year << "," << s.ftr;
        return out;
    }

	class Cf : public Atomic<CfState> {
	                   
    public:
        Port<double> out1;

        Cf(const std::string& id,nlohmann::json j_params): Atomic<CfState>(id, CfState()){
            out1 = addOutPort<double>("out1");

            state.date.day = j_params.at("day").get<int>();
            state.date.weekday = j_params.at("weekday").get<std::string>();
            state.date.month = j_params.at("month").get<std::string>();
            state.date.year = j_params.at("year").get<int>();
        }

        void internalTransition(CfState& s) const override {
                double dmf;
                double yf;

                s.date.day += 1;

                if (s.date.month == "january" && s.date.day > 31) {
                    s.date.month = "february";
                    s.date.day = 1;
                }

                else if (s.date.month == "february") {
                    bool is_leap = (s.date.year % 4 == 0 && s.date.year % 100 != 0) || s.date.year % 400 == 0;

                    if (s.date.day > (is_leap ? 29 : 28)) {
                        s.date.month = "march";
                        s.date.day = 1;
                    }
                }

                else if (s.date.month == "march" && s.date.day > 31) {
                    s.date.month = "april";
                    s.date.day = 1;
                }

                else if (s.date.month == "april" && s.date.day > 30) {
                    s.date.month = "may";
                    s.date.day = 1;
                }

                else if (s.date.month == "may" && s.date.day > 31) {
                    s.date.month = "june";
                    s.date.day = 1;
                }

                else if (s.date.month == "june" && s.date.day > 30) {
                    s.date.month = "july";
                    s.date.day = 1;
                }

                else if (s.date.month == "july" && s.date.day > 31) {
                    s.date.month = "august";
                    s.date.day = 1;
                }

                else if (s.date.month == "august" && s.date.day > 31) {
                    s.date.month = "september";
                    s.date.day = 1;
                }

                else if (s.date.month == "september" && s.date.day > 30) {
                    s.date.month = "october";
                    s.date.day = 1;
                }

                else if (s.date.month == "october" && s.date.day > 31) {
                    s.date.month = "november";
                    s.date.day = 1;
                }

                else if (s.date.month == "november" && s.date.day > 30) {
                    s.date.month = "december";
                    s.date.day = 1;
                }

                else if (s.date.month == "december" && s.date.day > 31) {
                    s.date.month = "january";
                    s.date.day = 1;
                    s.date.year += 1;
                }

                if (s.date.weekday == "sunday") s.date.weekday = "monday";
                else if (s.date.weekday == "monday") s.date.weekday = "tuesday";
                else if (s.date.weekday == "tuesday") s.date.weekday = "wednesday";
                else if (s.date.weekday == "wednesday") s.date.weekday = "thursday";
                else if (s.date.weekday == "thursday") s.date.weekday = "friday";
                else if (s.date.weekday == "friday") s.date.weekday = "saturday";
                else if (s.date.weekday == "saturday") s.date.weekday = "sunday";

                if (s.date.year == 2019) yf = 1.000;
                else if (s.date.year == 2020) yf = 0.870;
                else if (s.date.year == 2021) yf = 0.909;
                else if (s.date.year == 2022) yf = 0.961;
                else if (s.date.year == 2023) yf = 0.968;
                else if (s.date.year == 2024) yf = 0.971;
                else if (s.date.year == 2025) yf = 0.970;
                else if (s.date.year == 2026) yf = 0.969;
                else if (s.date.year == 2027) yf = 0.964;
                else if (s.date.year == 2028) yf = 0.957;
                else if (s.date.year == 2029) yf = 0.952;
                else if (s.date.year == 2030) yf = 0.943;
                else if (s.date.year == 2031) yf = 0.933;
                else if (s.date.year == 2032) yf = 0.922;
                else if (s.date.year == 2033) yf = 0.912;
                else if (s.date.year == 2034) yf = 0.900;
                else if (s.date.year == 2035) yf = 0.888;
                else if (s.date.year == 2036) yf = 0.875;
                else if (s.date.year == 2037) yf = 0.861;
                else if (s.date.year == 2038) yf = 0.846;
                else if (s.date.year == 2039) yf = 0.832;
                else if (s.date.year == 2040) yf = 0.817;
                else if (s.date.year == 2041) yf = 0.804;
                else if (s.date.year == 2042) yf = 0.790;
                else if (s.date.year == 2043) yf = 0.777;
                else if (s.date.year == 2044) yf = 0.765;
                else if (s.date.year == 2045) yf = 0.753;
                else if (s.date.year == 2046) yf = 0.741;
                else if (s.date.year == 2047) yf = 0.731;
                else if (s.date.year == 2048) yf = 0.721;
                else if (s.date.year == 2049) yf = 0.713;
                else if (s.date.year == 2050) yf = 0.704;

            if (s.date.weekday == "sunday") {
                if (s.date.month == "january") dmf = 1.0889;
                else if (s.date.month == "february") dmf = 0.9412;
                else if (s.date.month == "march") dmf = 0.9954;
                else if (s.date.month == "april") dmf = 0.8475;
                else if (s.date.month == "may") dmf = 0.8341;
                else if (s.date.month == "june") dmf = 0.8441;
                else if (s.date.month == "july") dmf = 1.0717;
                else if (s.date.month == "august") dmf = 0.9779;
                else if (s.date.month == "september") dmf = 0.8600;
                else if (s.date.month == "october") dmf = 0.8526;
                else if (s.date.month == "november") dmf = 0.9280;
                else if (s.date.month == "december") dmf = 1.0099;
            }

            else if (s.date.weekday == "monday") {
                if (s.date.month == "january") dmf = 1.1935;
                else if (s.date.month == "february") dmf = 0.9845;
                else if (s.date.month == "march") dmf = 1.0689;
                else if (s.date.month == "april") dmf = 0.9358;
                else if (s.date.month == "may") dmf = 0.9102;
                else if (s.date.month == "june") dmf = 0.9210;
                else if (s.date.month == "july") dmf = 1.1015;
                else if (s.date.month == "august") dmf = 1.0848;
                else if (s.date.month == "september") dmf = 0.9273;
                else if (s.date.month == "october") dmf = 0.9097;
                else if (s.date.month == "november") dmf = 1.0122;
                else if (s.date.month == "december") dmf = 1.0565;
            }

            else if (s.date.weekday == "tuesday") {
                if (s.date.month == "january") dmf = 1.1157;
                else if (s.date.month == "february") dmf = 1.0300;
                else if (s.date.month == "march") dmf = 1.0683;
                else if (s.date.month == "april") dmf = 0.9362;
                else if (s.date.month == "may") dmf = 0.9338;
                else if (s.date.month == "june") dmf = 0.9421;
                else if (s.date.month == "july") dmf = 1.1458;
                else if (s.date.month == "august") dmf = 1.1235;
                else if (s.date.month == "september") dmf = 0.9481;
                else if (s.date.month == "october") dmf = 0.9529;
                else if (s.date.month == "november") dmf = 1.0143;
                else if (s.date.month == "december") dmf = 1.0589;
            }

            else if (s.date.weekday == "wednesday") {
                if (s.date.month == "january") dmf = 1.1379;
                else if (s.date.month == "february") dmf = 1.0507;
                else if (s.date.month == "march") dmf = 1.0645;
                else if (s.date.month == "april") dmf = 0.9290;
                else if (s.date.month == "may") dmf = 0.9383;
                else if (s.date.month == "june") dmf = 0.9730;
                else if (s.date.month == "july") dmf = 1.1521;
                else if (s.date.month == "august") dmf = 1.1332;
                else if (s.date.month == "september") dmf = 0.9742;
                else if (s.date.month == "october") dmf = 0.9512;
                else if (s.date.month == "november") dmf = 1.0271;
                else if (s.date.month == "december") dmf = 1.0878;
            }

            else if (s.date.weekday == "thursday") {
                if (s.date.month == "january") dmf = 1.1531;
                else if (s.date.month == "february") dmf = 1.0147;
                else if (s.date.month == "march") dmf = 1.0564;
                else if (s.date.month == "april") dmf = 0.9364;
                else if (s.date.month == "may") dmf = 0.9407;
                else if (s.date.month == "june") dmf = 0.9729;
                else if (s.date.month == "july") dmf = 1.1812;
                else if (s.date.month == "august") dmf = 1.0603;
                else if (s.date.month == "september") dmf = 0.9362;
                else if (s.date.month == "october") dmf = 0.9509;
                else if (s.date.month == "november") dmf = 1.0359;
                else if (s.date.month == "december") dmf = 1.1089;
            }

            else if (s.date.weekday == "friday") {
                if (s.date.month == "january") dmf = 1.1213;
                else if (s.date.month == "february") dmf = 1.0181;
                else if (s.date.month == "march") dmf = 1.0459;
                else if (s.date.month == "april") dmf = 0.9186;
                else if (s.date.month == "may") dmf = 0.9318;
                else if (s.date.month == "june") dmf = 0.9449;
                else if (s.date.month == "july") dmf = 1.1925;
                else if (s.date.month == "august") dmf = 1.0347;
                else if (s.date.month == "september") dmf = 0.9281;
                else if (s.date.month == "october") dmf = 0.9374;
                else if (s.date.month == "november") dmf = 1.0188;
                else if (s.date.month == "december") dmf = 1.0805;
            }

            else if (s.date.weekday == "saturday") {
                if (s.date.month == "january") dmf = 1.0954;
                else if (s.date.month == "february") dmf = 0.9608;
                else if (s.date.month == "march") dmf = 1.0002;
                else if (s.date.month == "april") dmf = 0.8536;
                else if (s.date.month == "may") dmf = 0.8609;
                else if (s.date.month == "june") dmf = 0.8727;
                else if (s.date.month == "july") dmf = 1.1403;
                else if (s.date.month == "august") dmf = 0.9595;
                else if (s.date.month == "september") dmf = 0.8794;
                else if (s.date.month == "october") dmf = 0.8615;
                else if (s.date.month == "november") dmf = 0.9672;
                else if (s.date.month == "december") dmf = 1.0311;
            }

            s.ftr = yf * dmf;
        }

        void externalTransition(CfState& s, double e) const override {
			
		}

		void output(const CfState& s) const override {
			out1->addMessage(s.ftr);
		}

		[[nodiscard]] double timeAdvance(const CfState& s) const override {
			return 1;
		}
	};
}

#endif //CADMIUM_EXAMPLE_BEM_CF_HPP_