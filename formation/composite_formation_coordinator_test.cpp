#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "formation/composite_formation_coordinator.h"
#include "formation/formation_coordinator.h" // For direct use of NodeMessages, NodePositions
#include <vector>
#include <string>
#include <stdexcept>
#include <memory> // For std::make_unique

// Define ketu::communication::MessageType and ketu::telemetry::Position if not fully defined
// For simplicity, using placeholder types if actual definitions are complex and not needed for mock compilation
namespace ketu::communication {
    // Assuming MessageType is a simple enum or struct that can be default constructed or easily mocked.
    // If it's complex, a proper mock or definition would be needed.
    enum class MessageType {
        DEFAULT_MSG
    };
}

namespace ketu::telemetry {
    // Assuming Position is a struct that can be default constructed or easily mocked.
    struct Position {
        double x = 0.0, y = 0.0, z = 0.0;
        bool operator==(const Position& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
    };
}


namespace ketu::formation::testing
{

    using ::testing::_;
    using ::testing::Return;
    using ::testing::Throw;
    using ::testing::ElementsAre; // For comparing vector contents
    using ::testing::Eq;
    using ::testing::Field; // For checking members of an object, useful for NodePositions if complex

    class MockFormationCoordinator : public FormationCoordinator
    {
    public:
        MOCK_METHOD(int, getMaxNeighborCount, (const std::string &nodeId), (override));
        MOCK_METHOD(bool, isNodeLocallyFormed, (const std::string &nodeId), (override));
        MOCK_METHOD(std::vector<std::string>, getLocalNeighbors, (const std::string &nodeId), (override));
        MOCK_METHOD(void, setLocalNeighbors, (const std::string &nodeId, const std::vector<std::string> neighbors), (override));
        MOCK_METHOD(bool, isNodeAssigned, (const std::string &nodeId), (override));
        MOCK_METHOD(bool, isNodeInPosition, (const std::string &sourceNodeId, const std::string &targetNodeId), (override));
        MOCK_METHOD(bool, isFormationComplete, (), (override));
        MOCK_METHOD(const NodeMessages, align, (const std::string &nodeId, const NodePositions &relativeNodePositions), (override));
        // Adding a virtual destructor to satisfy GMock's requirements for mockable classes
        ~MockFormationCoordinator() override = default;
    };

    class CompositeFormationCoordinatorTest : public ::testing::Test
    {
    protected:
        // Using raw pointers as CompositeFormationCoordinator takes a vector of raw pointers.
        // Ownership will be managed by std::unique_ptr in the tests for safety,
        // but raw pointers will be passed to the CompositeFormationCoordinator.
        std::vector<std::unique_ptr<MockFormationCoordinator>> mock_coordinators_owner_;
        std::vector<FormationCoordinator *> mock_coordinator_ptrs_;

        void CreateCoordinators(int count) {
            mock_coordinators_owner_.clear();
            mock_coordinator_ptrs_.clear();
            for (int i = 0; i < count; ++i) {
                auto mock_coord = std::make_unique<MockFormationCoordinator>();
                mock_coordinator_ptrs_.push_back(mock_coord.get());
                mock_coordinators_owner_.push_back(std::move(mock_coord));
            }
        }
    };

    TEST_F(CompositeFormationCoordinatorTest, ConstructorSuccess)
    {
        CreateCoordinators(2);
        ASSERT_NO_THROW({
            CompositeFormationCoordinator cfc(mock_coordinator_ptrs_);
        });
        // active_coordinator_idx_ is private, tested implicitly via delegation tests
    }

    TEST_F(CompositeFormationCoordinatorTest, ConstructorThrowsOnEmpty)
    {
        CreateCoordinators(0); // Creates an empty vector
        ASSERT_THROW(CompositeFormationCoordinator cfc(mock_coordinator_ptrs_), std::invalid_argument);
    }

    TEST_F(CompositeFormationCoordinatorTest, SwitchCoordinatorSuccess)
    {
        CreateCoordinators(3);
        CompositeFormationCoordinator cfc(mock_coordinator_ptrs_);
        ASSERT_NO_THROW(cfc.switchCoordinator(1));
        // Further testing of active_coordinator_idx_ is done via delegation
    }

    TEST_F(CompositeFormationCoordinatorTest, SwitchCoordinatorThrowsOutOfRange)
    {
        CreateCoordinators(2);
        CompositeFormationCoordinator cfc(mock_coordinator_ptrs_);
        ASSERT_THROW(cfc.switchCoordinator(-1), std::out_of_range);
        ASSERT_THROW(cfc.switchCoordinator(2), std::out_of_range);
        ASSERT_THROW(cfc.switchCoordinator(3), std::out_of_range);
    }

    TEST_F(CompositeFormationCoordinatorTest, DelegatesGetMaxNeighborCount)
    {
        CreateCoordinators(2);
        CompositeFormationCoordinator cfc(mock_coordinator_ptrs_);
        const std::string node_id = "node1";
        const int expected_count = 5;

        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[0]), getMaxNeighborCount(node_id))
            .WillOnce(Return(expected_count));
        EXPECT_EQ(cfc.getMaxNeighborCount(node_id), expected_count);

        cfc.switchCoordinator(1);
        const int expected_count_switched = 10;
        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[1]), getMaxNeighborCount(node_id))
            .WillOnce(Return(expected_count_switched));
        EXPECT_EQ(cfc.getMaxNeighborCount(node_id), expected_count_switched);
    }

    TEST_F(CompositeFormationCoordinatorTest, DelegatesIsNodeLocallyFormed)
    {
        CreateCoordinators(2);
        CompositeFormationCoordinator cfc(mock_coordinator_ptrs_);
        const std::string node_id = "nodeA";

        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[0]), isNodeLocallyFormed(node_id))
            .WillOnce(Return(true));
        EXPECT_TRUE(cfc.isNodeLocallyFormed(node_id));

        cfc.switchCoordinator(1);
        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[1]), isNodeLocallyFormed(node_id))
            .WillOnce(Return(false));
        EXPECT_FALSE(cfc.isNodeLocallyFormed(node_id));
    }

    TEST_F(CompositeFormationCoordinatorTest, DelegatesGetLocalNeighbors)
    {
        CreateCoordinators(2);
        CompositeFormationCoordinator cfc(mock_coordinator_ptrs_);
        const std::string node_id = "nodeX";
        const std::vector<std::string> expected_neighbors = {"n1", "n2"};

        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[0]), getLocalNeighbors(node_id))
            .WillOnce(Return(expected_neighbors));
        EXPECT_THAT(cfc.getLocalNeighbors(node_id), ElementsAre("n1", "n2"));

        cfc.switchCoordinator(1);
        const std::vector<std::string> expected_neighbors_switched = {"n3", "n4"};
        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[1]), getLocalNeighbors(node_id))
            .WillOnce(Return(expected_neighbors_switched));
        EXPECT_THAT(cfc.getLocalNeighbors(node_id), ElementsAre("n3", "n4"));
    }

    TEST_F(CompositeFormationCoordinatorTest, DelegatesSetLocalNeighbors)
    {
        CreateCoordinators(2);
        CompositeFormationCoordinator cfc(mock_coordinator_ptrs_);
        const std::string node_id = "nodeY";
        const std::vector<std::string> neighbors_to_set = {"nbA", "nbB"};

        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[0]), setLocalNeighbors(node_id, neighbors_to_set))
            .Times(1);
        cfc.setLocalNeighbors(node_id, neighbors_to_set);

        cfc.switchCoordinator(1);
        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[1]), setLocalNeighbors(node_id, neighbors_to_set))
            .Times(1);
        cfc.setLocalNeighbors(node_id, neighbors_to_set);
    }

    TEST_F(CompositeFormationCoordinatorTest, DelegatesIsNodeAssigned)
    {
        CreateCoordinators(2);
        CompositeFormationCoordinator cfc(mock_coordinator_ptrs_);
        const std::string node_id = "nodeB";

        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[0]), isNodeAssigned(node_id))
            .WillOnce(Return(false));
        EXPECT_FALSE(cfc.isNodeAssigned(node_id));

        cfc.switchCoordinator(1);
        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[1]), isNodeAssigned(node_id))
            .WillOnce(Return(true));
        EXPECT_TRUE(cfc.isNodeAssigned(node_id));
    }

    TEST_F(CompositeFormationCoordinatorTest, DelegatesIsNodeInPosition)
    {
        CreateCoordinators(2);
        CompositeFormationCoordinator cfc(mock_coordinator_ptrs_);
        const std::string source_node = "src";
        const std::string target_node = "tgt";

        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[0]), isNodeInPosition(source_node, target_node))
            .WillOnce(Return(true));
        EXPECT_TRUE(cfc.isNodeInPosition(source_node, target_node));

        cfc.switchCoordinator(1);
        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[1]), isNodeInPosition(source_node, target_node))
            .WillOnce(Return(false));
        EXPECT_FALSE(cfc.isNodeInPosition(source_node, target_node));
    }

    TEST_F(CompositeFormationCoordinatorTest, DelegatesIsFormationComplete)
    {
        CreateCoordinators(2);
        CompositeFormationCoordinator cfc(mock_coordinator_ptrs_);

        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[0]), isFormationComplete())
            .WillOnce(Return(false));
        EXPECT_FALSE(cfc.isFormationComplete());

        cfc.switchCoordinator(1);
        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[1]), isFormationComplete())
            .WillOnce(Return(true));
        EXPECT_TRUE(cfc.isFormationComplete());
    }

    // Helper for comparing NodePositions if it becomes complex
    MATCHER_P(NodePositionsEq, expected, "") {
        if (arg.size() != expected.size()) return false;
        for (const auto& [key, val] : expected) {
            auto it = arg.find(key);
            if (it == arg.end() || !(it->second == val)) return false;
        }
        return true;
    }

    TEST_F(CompositeFormationCoordinatorTest, DelegatesAlign)
    {
        CreateCoordinators(2);
        CompositeFormationCoordinator cfc(mock_coordinator_ptrs_);
        const std::string node_id = "nodeAlign";
        const NodePositions input_positions = {
            {"neighbor1", {1.0, 0.0, 0.0}},
            {"neighbor2", {0.0, 1.0, 0.0}}
        };
        const NodeMessages expected_messages = {
            {"neighbor1", ketu::communication::MessageType::DEFAULT_MSG}
        };

        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[0]), align(node_id, NodePositionsEq(input_positions)))
            .WillOnce(Return(expected_messages));
        // Need to compare NodeMessages map. For simplicity, using direct equality if operator== is defined or it's simple.
        // If NodeMessages comparison is complex, a custom matcher would be needed.
        // Assuming NodeMessages can be compared directly for this example.
        EXPECT_EQ(cfc.align(node_id, input_positions).size(), expected_messages.size());


        cfc.switchCoordinator(1);
        const NodeMessages expected_messages_switched = {
            {"neighbor3", ketu::communication::MessageType::DEFAULT_MSG}
        };
        EXPECT_CALL(*static_cast<MockFormationCoordinator*>(mock_coordinator_ptrs_[1]), align(node_id, NodePositionsEq(input_positions)))
            .WillOnce(Return(expected_messages_switched));
        EXPECT_EQ(cfc.align(node_id, input_positions).size(), expected_messages_switched.size());
    }

} // namespace ketu::formation::testing

// It's common for test files not to have a main if the build system (e.g., CMake with GTest)
// links gtest_main. This provides a generic main.
// int main(int argc, char **argv) {
//   ::testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }
