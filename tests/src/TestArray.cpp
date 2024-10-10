#include "gtest/gtest.h"
#include "array.hh"
#include <vector>
#include <array>
#include <Eigen/Dense> // Include Eigen


class Arr : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};


TEST(Arr, Dot) {
    std::vector<int> a =            {1, 2, 3};
    std::vector<int> b =            {1, 2, 3};
    std::vector<unsigned short> c = {1, 2, 3};
    std::vector<float> d =          {1, 2, 3};
    std::vector<double> e =         {1, 2, 3};
    std::array<int, 3> f =          {1, 2, 3};
    std::array<long, 3> g =         {1, 2, 3};
    std::array<float, 3> h =        {1, 2, 3};
    std::array<double, 3> i =       {1, 2, 3};
    double k[3] =                   {1, 2, 3};
    float l[3] =                    {1, 2, 3};

    Eigen::Vector3d j(1.0, 2.0, 3.0);

    EXPECT_EQ(ums::dot(a, b), 14);
    EXPECT_EQ(ums::dot(a, c), 14);
    EXPECT_EQ(ums::dot(a, d), 14);
    EXPECT_EQ(ums::dot(a, e), 14);
    EXPECT_EQ(ums::dot(a, f), 14);
    EXPECT_EQ(ums::dot(a, g), 14);
    EXPECT_EQ(ums::dot(a, h), 14);
    EXPECT_EQ(ums::dot(a, i), 14);
    EXPECT_EQ(ums::dot(b, c), 14);
    EXPECT_EQ(ums::dot(b, d), 14);
    EXPECT_EQ(ums::dot(b, e), 14);
    EXPECT_EQ(ums::dot(b, f), 14);
    EXPECT_EQ(ums::dot(b, g), 14);
    EXPECT_EQ(ums::dot(b, h), 14);
    EXPECT_EQ(ums::dot(b, i), 14);
    EXPECT_EQ(ums::dot(c, d), 14);
    EXPECT_EQ(ums::dot(c, e), 14);
    EXPECT_EQ(ums::dot(c, f), 14);
    EXPECT_EQ(ums::dot(c, g), 14);
    EXPECT_EQ(ums::dot(c, h), 14);
    EXPECT_EQ(ums::dot(c, i), 14);
    EXPECT_EQ(ums::dot(d, e), 14);
    EXPECT_EQ(ums::dot(d, f), 14);
    EXPECT_EQ(ums::dot(d, g), 14);
    EXPECT_EQ(ums::dot(d, h), 14);
    EXPECT_EQ(ums::dot(d, i), 14);
    EXPECT_EQ(ums::dot(e, f), 14);
    EXPECT_EQ(ums::dot(e, g), 14);
    EXPECT_EQ(ums::dot(e, h), 14);
    EXPECT_EQ(ums::dot(e, i), 14);
    EXPECT_EQ(ums::dot(f, g), 14);
    EXPECT_EQ(ums::dot(f, h), 14);
    EXPECT_EQ(ums::dot(f, i), 14);
    EXPECT_EQ(ums::dot(g, h), 14);
    EXPECT_EQ(ums::dot(g, i), 14);
    EXPECT_EQ(ums::dot(h, i), 14);
    EXPECT_EQ(ums::dot(j, a), 14);
    EXPECT_EQ(ums::dot(j, b), 14);
    EXPECT_EQ(ums::dot(j, c), 14);
    EXPECT_EQ(ums::dot(j, d), 14);
    EXPECT_EQ(ums::dot(j, e), 14);
    EXPECT_EQ(ums::dot(j, f), 14);
    EXPECT_EQ(ums::dot(j, g), 14);
    EXPECT_EQ(ums::dot(j, h), 14);
    EXPECT_EQ(ums::dot(j, i), 14);
    EXPECT_EQ(ums::dot(j, j), 14);
    EXPECT_EQ(ums::dot(j, k), 14);
    EXPECT_EQ(ums::dot(j, l), 14);
    EXPECT_EQ(ums::dot(k, a), 14);
    EXPECT_EQ(ums::dot(k, b), 14);
    EXPECT_EQ(ums::dot(k, c), 14);
    EXPECT_EQ(ums::dot(k, d), 14);
    EXPECT_EQ(ums::dot(k, e), 14);
    EXPECT_EQ(ums::dot(k, f), 14);
    EXPECT_EQ(ums::dot(k, g), 14);
    EXPECT_EQ(ums::dot(k, h), 14);
    EXPECT_EQ(ums::dot(k, i), 14);
    EXPECT_EQ(ums::dot(k, j), 14);
    EXPECT_EQ(ums::dot(k, k), 14);
    EXPECT_EQ(ums::dot(k, l), 14);
    EXPECT_EQ(ums::dot(l, a), 14);
    EXPECT_EQ(ums::dot(l, b), 14);
    EXPECT_EQ(ums::dot(l, c), 14);
    EXPECT_EQ(ums::dot(l, d), 14);
    EXPECT_EQ(ums::dot(l, e), 14);
    EXPECT_EQ(ums::dot(l, f), 14);
    EXPECT_EQ(ums::dot(l, g), 14);
    EXPECT_EQ(ums::dot(l, h), 14);
    EXPECT_EQ(ums::dot(l, i), 14);
    EXPECT_EQ(ums::dot(l, j), 14);
    EXPECT_EQ(ums::dot(l, k), 14);
}

