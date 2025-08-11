#include <gtest/gtest.h>
#include "../src/packer.h"

class PackerTest : public ::testing::Test {
protected:
    Packer packer;

    void SetUp() override {
        // Code to set up test environment, if needed
    }

    void TearDown() override {
        // Code to clean up after tests, if needed
    }
};

TEST_F(PackerTest, PackFilesTest) {
    // Add test code for packFiles method
    ASSERT_TRUE(packer.packFiles("input.txt", "output.packed"));
}

TEST_F(PackerTest, UnpackFilesTest) {
    // Add test code for unpackFiles method
    ASSERT_TRUE(packer.unpackFiles("output.packed", "output.txt"));
}