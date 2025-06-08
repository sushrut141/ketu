#include "scenarios/random_nodes.h"

int main(int argc, char** argv) {

    auto scenario = ketu::scenarios::RandomNodes::create();

    scenario->setup();
    scenario->run();

    return 0;
}
