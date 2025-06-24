#include "scenarios/collision_avoidance.h"

int main(int argc, char** argv)
{

    auto scenario = ketu::scenarios::CollisionAvoidance::create();

    scenario->setup();
    scenario->run();

    return 0;
}
