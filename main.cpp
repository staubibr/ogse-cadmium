#include <iostream>
#include <json.hpp>
#include <fstream>
#include <data_structures/scenario.hpp>
#include <model_factory.hpp>
#include <filesystem>
#include <cadmium/core/modeling/coupled.hpp>
#include <cadmium/core/simulation/root_coordinator.hpp>
#include <cadmium/core/logger/csv.hpp>

int main(int argc, char *argv[]) {
    std::cout << "Checking arguments... \n";
    if (argc < 4) {
        std::cerr << "ERROR: not enough arguments" << std::endl;
        std::cerr << "    Usage:" << std::endl;
        std::cerr << "    > main_ogse SCENARIO_PATH OUTPUT_PATH DURATION" << std::endl;
        std::cerr << "        (DURATION must be greater than or equal to 0)" << std::endl;
        return -1;
    }

    std::cout << "Reading scenario file... \n";
    // Open scenario file and read content into scenario object
    std::ifstream stream(argv[1]);
    if (!stream.is_open() || stream.fail()) throw std::runtime_error("unable to open scenario file as stream.");
    data_structures::scenario s = nlohmann::json::parse(stream);
    stream.close();

    // Build coupled model from scenario using model factory
    ogse::model_factory::initialize();

    auto top = std::make_shared<cadmium::Coupled>("top");

    // TODO: error handling if model type doesnt exist
    std::cout << "Building model instances... \n";
    for (auto& i_set : s.instances) {
        for (auto& m : i_set.models) {
            nlohmann::json json = {};
            auto id = m.at(0).get<std::string>();
            for (int i = 1; i < m.size(); i++) json[i_set.properties[i]] = m.at(i);
            top->addComponent(ogse::model_factory::build(i_set.type, id, json));
        }
    }

    std::cout << "Coupling atomic models... \n";
    for (auto& c_set : s.couplings) {
        for (auto& c : c_set.couplings) {
            auto p_from = top->getComponent(c[0])->getOutPort(c_set.from_port);
            auto p_to = top->getComponent(c[1])->getInPort(c_set.to_port);
            top->addCoupling(p_from, p_to);
        }
    }

    // prepare output file and logger
    std::cout << "Preparing output file... \n";
    std::filesystem::path dir (argv[2]);
    std::filesystem::path file ("log.csv");
    std::filesystem::path output = dir / file;

    auto logger = std::make_shared<cadmium::CSVLogger>(output.u8string(), ";");

    // prepare and launch simulation
    std::cout << "Launching simulation... \n";
    double duration = std::stoi(argv[3]);
    auto rootCoordinator = cadmium::RootCoordinator(top);
    rootCoordinator.setLogger(logger);
    rootCoordinator.start();
    rootCoordinator.simulate(duration);
    rootCoordinator.stop();

    // TODO: Why doesn't id match name in the log file? Looks like model_id is internal to the simulator
    // TODO: (built recursively). If so, then it doesn't need to be in the log file. It's useless to the user.

    std::cout << "Done! \n";
    return 0;
}
