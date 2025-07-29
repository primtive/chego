#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct Account
{
    std::string name;
    std::string authToken;
    std::vector<std::string> friends;
};

const Account accounts[] = {
    {
        "gugich",
        "lip_7Rm30B9zcuMCwoRZ6mrB",
        std::vector<std::string>{"ZaaaZ3", "andysz"},
    },
    {
        "zaaaz3",
        "lip_7Rm30B9zcuMCwoRZ6mrB",
        std::vector<std::string>{"gugich", "andysz"},
    },
};

uint8_t wifi = 0;
uint8_t brightness = 0;
uint8_t statusBar = 0b00000010;

uint16_t accountSelection = 0;
uint16_t lichessChallengeSelection = 0;
uint16_t lichessGameSelection = 0;
uint16_t lichessFriendSelection = 0;


uint16_t accounts_get_cnt(void *data) {
  return 2;
}
const char *accounts_get_str(void *data, uint16_t index) {
  return accounts[index].name.c_str();
}