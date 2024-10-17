// tests/test_lag.cpp

#include <gtest/gtest.h>
#include "screamer/indicators/lag.h"

using namespace screamer::indicators;

// Test Fixture for Lag
class LagTest : public ::testing::Test {
protected:

    LagTest() {
        // Set-up work for each test here.
    }

    ~LagTest() override {
        // Clean-up work that doesn't throw exceptions here.
    }

    void SetUp() override {
        // Called immediately after the constructor (right before each test).
    }

    void TearDown() override {
        // Called immediately after each test (right before the destructor).
    }

};

// Test the constructor and initial state
TEST_F(LagTest, ConstructorInitializesBufferCorrectly) {
    int delay = 3;
    double initial = 5.0;
    Lag lag(delay, initial);

    // Access the internal buffer using a friend class or by modifying the Lag class to provide buffer access for testing
    // For simplicity, we'll test behavior instead of internal state

    // Initial calls should return the initial value
    EXPECT_DOUBLE_EQ(lag(10.0), 5.0);
    EXPECT_DOUBLE_EQ(lag(20.0), 5.0);
    EXPECT_DOUBLE_EQ(lag(30.0), 5.0);
    // After delay, should return the first value
    EXPECT_DOUBLE_EQ(lag(40.0), 10.0);
}

// Test default initial value
TEST_F(LagTest, DefaultInitialValue) {
    int delay = 2;
    Lag lag(delay); // initial defaults to 0.0

    EXPECT_DOUBLE_EQ(lag(10.0), 0.0);
    EXPECT_DOUBLE_EQ(lag(20.0), 0.0);
    EXPECT_DOUBLE_EQ(lag(30.0), 10.0);
    EXPECT_DOUBLE_EQ(lag(40.0), 20.0);
}

// Test multiple sequential calls
TEST_F(LagTest, MultipleSequentialCalls) {
    int delay = 4;
    double initial = 1.0;
    Lag lag(delay, initial);

    std::vector<double> inputs = {100.0, 200.0, 300.0, 400.0, 500.0};
    std::vector<double> expected = {1.0, 1.0, 1.0, 1.0, 100.0};

    for (size_t i = 0; i < inputs.size(); ++i) {
        EXPECT_DOUBLE_EQ(lag(inputs[i]), expected[i]) << "Failed at index " << i;
    }
}

// Test the constructor with valid delay
TEST_F(LagTest, ConstructorValidDelay) {
    EXPECT_NO_THROW(Lag(2, 0.0));
    EXPECT_NO_THROW(Lag(5, 10.0));
}

// Test the constructor with zero delay
TEST_F(LagTest, ConstructorZeroDelay) {
    EXPECT_THROW(Lag(0, 0.0), std::invalid_argument);
}

// Test the constructor with negative delay
TEST_F(LagTest, ConstructorNegativeDelay) {
    EXPECT_THROW(Lag(-1, 0.0), std::invalid_argument);
    EXPECT_THROW(Lag(-5, 10.0), std::invalid_argument);
}