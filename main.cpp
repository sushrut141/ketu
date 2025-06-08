#include <memory>
#include "world.h"

int main(int argc, char** argv) {

    std::unique_ptr<ketu::world::World> world = std::make_unique<ketu::world::World>();
    ketu::sensing::SensingClient sensing_client(world.get());
    ketu::communication::CommunicationClient communication_client(world.get());

    std::shared_ptr<ketu::objects::Node> sphereNode = std::make_shared<ketu::objects::Node>(
        "sphere",
        &sensing_client,
        &communication_client
    );
    world->addNode(sphereNode);

    world->render();

    return 0;
}