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

class Controller;
std::shared_ptr<Controller> controller_ptr;

bool is_menu_closed = false;
bool is_game_active = false;
bool is_offline_bot = false;
uint8_t wifi = 0;
uint8_t brightness = 10;
uint8_t status_bar = 0b00000010;

uint16_t account_selection = 0;
uint16_t challenge_selection = 0;
uint16_t game_selection = 0;
uint16_t friend_selection = 0;

uint8_t bot_level = 1;

uint16_t accounts_get_cnt(void *data)
{
  return 2;
}
const char *accounts_get_str(void *data, uint16_t index)
{
  return accounts[index].name.c_str();
}