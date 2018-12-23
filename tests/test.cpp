// Copyright 2018 Your Name <your_email>

#include <gtest/gtest.h>

#include <client.hpp>

TEST(Client, Test) {
    talk_to_svr talk("user_login");
    SUCCEED();
}
