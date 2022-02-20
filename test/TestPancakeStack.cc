#include "gtest/gtest.h"

#include "pancake_stack.cc"

using std::vector;

using namespace daily_programmer_20180307;

class TestPancakeStack : public testing::TestWithParam<vector<int>> {
	protected:
		bool isSorted(const vector<int>& arr) {
			for (int i = 0; i < arr.size() - 1; i++) {
				EXPECT_LT(arr[i], arr[i + 1]);
			}
		}
};

TEST(TestPancakeStack, ConstructorInputContainsDuplicatesElementsOfPiAreUnique) {
	// Arrange
	auto input = std::array<int, 2> {0, 0};

	// Act
	auto pancake_stack = PancakeStack(input);
	std::shared_ptr<const vector<int>> pi = pancake_stack.get_pi();

	// Assert
	auto set_of_pi_elements = std::set<int>(pi->begin(), pi->end());
	EXPECT_EQ(set_of_pi_elements.size(), pi->size());
}

TEST(TestPancakeStack, CountAdjacencies1AndNAreAdjacentTheAdjacencyIsCounted) {
	// Arrange
	auto pi = vector<int> {3, 1, 2};
	auto pancake_stack = PancakeStack(pi);

	// Act
	int result = pancake_stack.CountAdjacencies();

	// Assert
	EXPECT_EQ(result, 2);
}

TEST(TestPancakeStack, EndsBlockKAndKPlus1BookendPiKEndsBlock) {
	// Arrange
	auto pi = vector {0, 1};
	auto pancake_stack = PancakeStack(pi);

	// Act
	bool result = pancake_stack.EndsBlock(1);

	// Assert
	EXPECT_TRUE(result);
}

TEST_P(TestPancakeStack, TestSort) {
	// Arrange
	auto pancake_stack = PancakeStack(GetParam());
	
	// Act
	pancake_stack.Sort();

	// Assert
	isSorted(*pancake_stack.get_pi());
}

INSTANTIATE_TEST_SUITE_P(
	DailyProgrammer,
	TestPancakeStack,
	testing::Values(
		vector<int> {3, 1, 2},
		vector<int> {7, 6, 4, 2, 6, 7, 8, 7},
		vector<int> {11, 5, 12, 3, 10, 3, 2, 5},
		vector<int> {3, 12, 8, 12, 4, 7, 10, 3, 8, 10}
	)
);