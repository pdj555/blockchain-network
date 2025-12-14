#include <gtest/gtest.h>
#include <string>

#include "sha256.h"

TEST(Sha256Test, KnownVectors) {
    EXPECT_EQ(crypto::sha256_hex(""),
              "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    EXPECT_EQ(crypto::sha256_hex("abc"),
              "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
}

