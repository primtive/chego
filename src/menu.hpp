#pragma once

#include "MUIU8g2.h"
#include "U8g2lib.h"
#include "fonts.h"
#include "lichess.hpp"
#include "globals.hpp"

constexpr static uint8_t WIFI_ENABLED_BIT = 0x01;    // 0b00000001 (бит 0)
constexpr static uint8_t BOARD_CONNECTED_BIT = 0x02; // 0b00000010 (бит 1)
constexpr static uint8_t BOARD_TYPE_BIT = 0x04;      // 0b00000100 (бит 2)

using namespace lichess;

class U8G2_SDL : public U8G2
{
public:
  U8G2_SDL(const u8g2_cb_t *rotation) : U8G2()
  {
    u8g2_SetupBuffer_SDL_128x64_4(&u8g2, rotation);
  }
};

uint8_t mui_u8g2_btn_goto_wm_pf(mui_t *ui, uint8_t msg)
{
  switch (msg)
  {
  case MUIF_MSG_DRAW:
    mui_u8g2_draw_button_utf(ui, U8G2_BTN_HCENTER | mui_u8g2_get_pf_flags(ui), 0, 1, 1, ui->text);
    break;
  case MUIF_MSG_FORM_START:
    break;
  case MUIF_MSG_FORM_END:
    break;
  case MUIF_MSG_CURSOR_ENTER:
    break;
  case MUIF_MSG_CURSOR_SELECT:
  case MUIF_MSG_VALUE_INCREMENT:
  case MUIF_MSG_VALUE_DECREMENT:
    // return mui_GotoForm(ui, ui->arg, 0);
    mui_SaveForm(ui);
    return mui_GotoFormAutoCursorPosition(ui, ui->arg);
  case MUIF_MSG_CURSOR_LEAVE:
    break;
  case MUIF_MSG_TOUCH_DOWN:
    break;
  case MUIF_MSG_TOUCH_UP:
    break;
  }
  return 0;
}
uint8_t mui_draw_status_bar(mui_t *ui, uint8_t msg)
{
  if (msg == MUIF_MSG_DRAW)
  {
    char a = (status_bar & WIFI_ENABLED_BIT) != 0 ? 'A' : 'B';
    char b = (status_bar & BOARD_CONNECTED_BIT) != 0 ? ((status_bar & BOARD_TYPE_BIT) ? 'a' : 'b') : 'c';
    char str[3] = {a, b, '\0'};
    u8g2_DrawUTF8(mui_get_U8g2(ui), mui_get_x(ui), mui_get_y(ui), str);
  }
  return 0;
}
uint8_t mui_brightness(mui_t *ui, uint8_t msg) {
  uint8_t val = mui_u8g2_u8_bar_wm_mud_pi(ui, msg);
  if (val) {
    controller_ptr->matrix->display();
  }
  return val;
}


enum class Action : uint8_t
{
  Challenges = 0,
  Games = 1,
  PlayOffline = 2,
  Highlight = 3,
  StartGame = 4,
  Friends = 5,
  BotGame = 6,
  QuitGame = 7
};
enum class ActionButton : char
{
  Challenges = 'c',
  Games = 'g',
  Friends = 'f'
};

uint8_t mui_action_button(mui_t *ui, uint8_t msg)
{
  switch (msg)
  {
  case MUIF_MSG_DRAW:
    mui_u8g2_draw_button_pi(ui, 128 - mui_get_x(ui) * 2, mui_get_x(ui), ui->text);
    // mui_u8g2_draw_button_utf(ui, U8G2_BTN_HCENTER | mui_u8g2_get_fi_flags(ui), 0, 1, 1, ui->text);
    break;
  case MUIF_MSG_FORM_START:
    break;
  case MUIF_MSG_FORM_END:
    break;
  case MUIF_MSG_CURSOR_ENTER:
    break;
  case MUIF_MSG_CURSOR_SELECT:
  case MUIF_MSG_VALUE_INCREMENT:
  case MUIF_MSG_VALUE_DECREMENT:
    switch (static_cast<Action>(ui->arg))
    {
    case Action::Challenges:
      fetch_challenges();
      return mui_GotoForm(ui, 28, 0);
      break;
    case Action::Games:
      fetch_games();
      return mui_GotoForm(ui, 29, 0);
      break;
    case Action::Friends:
      return mui_GotoForm(ui, 30, 0);
    case Action::PlayOffline:
      controller_ptr->initChess();
      if (!(controller_ptr->game->getGameStarted()))
      {
        return mui_GotoForm(ui, 40, 0);
      }
      is_game_active = true;
      return mui_GotoForm(ui, 50, 0);
    case Action::Highlight:
      controller_ptr->game->highlightPieces();
      break;
    case Action::StartGame:
      if (controller_ptr->game->checkPosition())
      {
        controller_ptr->startChess();
        is_game_active = true;
        return mui_GotoForm(ui, 50, 0);
      }
      break;
    case Action::QuitGame:
      controller_ptr->resetChess();
      return mui_GotoForm(ui, 1, 0);
    case Action::BotGame:
      if (is_offline_bot)
      {
        controller_ptr->initOfflineBot();
      }
      else
      {
        json game = apiPost("challenge/ai", cpr::Payload{{"level", std::to_string(bot_level)}});
        controller_ptr->initLichess(Game{
            game["id"],
            "Stockfish",
            game["fen"],
            game["player"].get<std::string>() != "white"});
      }

      if (!(controller_ptr->game->getGameStarted()))
      {
        return mui_GotoForm(ui, 40, 0);
      }
      is_game_active = true;
      return mui_GotoForm(ui, 50, 0);
    }
  case MUIF_MSG_CURSOR_LEAVE:
    break;
  case MUIF_MSG_TOUCH_DOWN:
    break;
  case MUIF_MSG_TOUCH_UP:
    break;
  }
  return 0;
}
uint8_t mui_action_list_button(mui_t *ui, uint8_t msg)
{
  mui_u8g2_list_t *list = (mui_u8g2_list_t *)muif_get_data(ui->uif);
  uint16_t *selection = mui_u8g2_list_get_selection_ptr(list);
  void *data = mui_u8g2_list_get_data_ptr(list);
  mui_u8g2_get_list_element_cb element_cb = mui_u8g2_list_get_element_cb(list);

  uint16_t pos = ui->arg; // remember the arg value, because it might be overwritten
  pos += ui->form_scroll_top;
  auto target = element_cb(data, pos);
  switch (msg)
  {
  case MUIF_MSG_DRAW:
    mui_u8g2_draw_button_pi(ui, 128 - mui_get_x(ui) * 2, mui_get_x(ui), target);
    break;
  case MUIF_MSG_FORM_START:
    break;
  case MUIF_MSG_FORM_END:
    break;
  case MUIF_MSG_CURSOR_ENTER:
    break;
  case MUIF_MSG_CURSOR_LEAVE:
    break;
  case MUIF_MSG_TOUCH_DOWN:
    break;
  case MUIF_MSG_TOUCH_UP:
    break;
  case MUIF_MSG_CURSOR_SELECT:
  case MUIF_MSG_VALUE_INCREMENT:
  case MUIF_MSG_VALUE_DECREMENT:
    // Поиск элемента, где первый элемент внутреннего вектора равен target
    switch (static_cast<ActionButton>(ui->text[0]))
    {
    case ActionButton::Challenges:
      apiPost("challenge/" + challenges[*selection].id + "/accept");
      return mui_GotoForm(ui, 22, 0);
      break;
    case ActionButton::Friends:
      apiPost("challenge/" + accounts[account_selection].friends[*selection]);
      return mui_GotoForm(ui, 22, 0);
      break;
    case ActionButton::Games:
      auto it = std::find_if(games.begin(),
                             games.end(),
                             [&target](const Game &game)
                             {
                               return game.name == target;
                             });
      if (it != games.end())
      {
        controller_ptr->initLichess(*it);
        if (!(controller_ptr->game->getGameStarted()))
        {
          return mui_GotoForm(ui, 40, 0);
        }
      }
      return mui_GotoForm(ui, ui->arg, 0);
      break;
    }
  }
  return 0;
}

muif_t muif_list[] = {
    /* latin + cyrillic text style */
    MUIF_U8G2_FONT_STYLE(0, u8g2_font_6x12_t_cyrillic),
    /* lichess font */
    MUIF_U8G2_FONT_STYLE(2, custom_font),
    /* test fonts */
    MUIF_U8G2_FONT_STYLE(3, u8g2_font_open_iconic_check_4x_t),
    // MUIF_U8G2_FONT_STYLE(4, u8g2_font_iconquadpix_m_all),
    MUIF_U8G2_FONT_STYLE(4, u8g2_font_6x12_t_cyrillic),
    /* label */
    MUIF_U8G2_LABEL(),
    /* horizontal line */
    MUIF_RO("HR", mui_hline),
    /* main menu */
    MUIF_RO("GP", mui_u8g2_goto_data),
    MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pi),
    /* buttons */
    MUIF_BUTTON("GL", mui_u8g2_btn_goto_w1_pi), // goto button line
    MUIF_BUTTON("GT", mui_u8g2_btn_goto_wm_fi), // goto button
    MUIF_BUTTON("PB", mui_u8g2_btn_goto_wm_pf), // play button (without borders)
    MUIF_BUTTON("EX", mui_u8g2_btn_exit_wm_fi), // exit button
    MUIF_BUTTON("AB", mui_action_button),       // action button
    /* lichess */
    MUIF_U8G2_U16_LIST("LC", &challenge_selection, NULL, challenges_get_str, challenges_get_cnt, mui_action_list_button),
    MUIF_U8G2_U16_LIST("LG", &game_selection, NULL, games_get_str, games_get_cnt, mui_action_list_button),
    MUIF_U8G2_U16_LIST("LF", &friend_selection, NULL, friends_get_str, friends_get_cnt, mui_action_list_button),
    /* lichess bot */
    MUIF_VARIABLE("CB", &is_offline_bot, mui_u8g2_u8_chkbox_wm_pi),
    MUIF_U8G2_U8_MIN_MAX_STEP("LB", &bot_level, 1, 8, 1, MUI_MMS_4X_BAR | MUI_MMS_SHOW_VALUE, mui_u8g2_u8_bar_wm_mud_pi),
    /* status bar */
    MUIF_RO("SB", mui_draw_status_bar),
    /* wi-fi select */
    MUIF_VARIABLE("IC", &wifi, mui_u8g2_u8_opt_parent_wm_pi),
    MUIF_VARIABLE("OC", &wifi, mui_u8g2_u8_opt_radio_child_w1_pi),
    /* lichess acc select */
    MUIF_U8G2_U16_LIST("AL", &account_selection, NULL, accounts_get_str, accounts_get_cnt, mui_u8g2_u16_list_line_wa_mud_pi),
    /* brightness bar */
    MUIF_U8G2_U8_MIN_MAX_STEP("B0", &brightness, 0, 10, 2, MUI_MMS_4X_BAR | MUI_MMS_SHOW_VALUE, mui_brightness),

};

#define STATUS_PANEL MUI_STYLE(4) MUI_XY("SB", 96, 12) MUI_STYLE(0)

fds_t fds_data[] =

    MUI_FORM(0)

    /* main menu */
    MUI_FORM(1)                          //
    STATUS_PANEL                         //
        MUI_STYLE(0)                     //
    MUI_LABEL(5, 10, "Меню")             //
    MUI_XY("HR", 0, 13)                  //
    MUI_DATA("GP",                       //
             MUI_2 "Играть|"             //
             MUI_3 "Настройки|"          //
             )                           //
    MUI_XYA("GC", 5, 25, 0)              //
    MUI_XYA("GC", 5, 37, 1)              //
    MUI_XYAT("EX", 64, 55, 1, "Закрыть") //
    // MUI_XYA("GC", 5, 49, 2)     //

    /* chessboard play */
    MUI_FORM(2)                          //
    MUI_STYLE(0)                         //
    MUI_LABEL(50, 8, "Играть")           //
    MUI_STYLE(3)                         //
    MUI_XYAT("PB", 22, 46, 20, "A")      //
    MUI_XYAT("PB", 64, 46, 30, "B")      //
    MUI_XYAT("PB", 106, 46, 40, "C")     //
    MUI_STYLE(0)                         //
    MUI_XYAT("GT", 64, 59, 1, " Назад ") //

    /* settings */
    MUI_FORM(3)                                              //
    MUI_LABEL(5, 10, "Настройки")                            //
    MUI_XY("HR", 0, 13)                                      //
    MUI_LABEL(5, 24, "Wi-Fi:")                               //
    MUI_XYAT("IC", 64, 24, 12, "Ufanet-55|akim|phone|other") //
    MUI_LABEL(5, 38, "Lichess:")                             //
    MUI_XYA("AL", 64, 38, 50)                                //
    MUI_XYAT("GT", 37, 55, 14, " Доска ")                    //
    MUI_XYAT("GT", 90, 55, 1, " Назад ")                     //

    /* Wi-Fi select */
    MUI_FORM(12)                        //
    MUI_LABEL(5, 10, "Wi-Fi Selection") //
    MUI_XY("HR", 0, 13)                 //
    MUI_XYA("OC", 5, 30, 0)             //
    MUI_XYA("OC", 5, 42, 1)             //
    MUI_XYA("OC", 5, 54, 2)             //

    /* board settings */
    MUI_FORM(14)                         //
    MUI_LABEL(5, 10, "Настройки доски")  //
    MUI_XY("HR", 0, 13)                  //
    MUI_LABEL(5, 24, "Яркость:")         //
    MUI_XY("B0", 64, 24)                 //
    MUI_XYAT("GT", 90, 55, 3, " Назад ") //

    /* play chess */
    MUI_FORM(20)                                //
    MUI_LABEL(5, 10, "Шахматы")                 //
    MUI_XY("HR", 0, 13)                         //
    MUI_XYAT("AB", 5, 25, 2, "Вдвоем на доске") // Action::PlayOffline
    MUI_XYAT("GL", 5, 37, 22, "Играть онлайн")  // online
    MUI_XYAT("GL", 5, 49, 23, "Играть с ботом") // bot
    MUI_XYAT("GL", 5, 61, 2, "Назад")          // go back

    /* play offline */
    /*
     *   Перед игрой:
     * - Use clock + время
     * - Allow back
     * - Disable move highlighting
     */
    MUI_FORM(21)                       //
    MUI_LABEL(5, 10, "Играть оффлайн") //

    /* play lichess */
    MUI_FORM(22)                                //
    MUI_LABEL(5, 10, "Lichess")                 //
    MUI_XY("HR", 0, 13)                         //
    MUI_XYAT("AB", 5, 25, 0, "Вызовы")          // Action::Challenges
    MUI_XYAT("AB", 5, 37, 1, "Активные игры")   // Action::Games
    MUI_XYAT("AB", 5, 49, 5, "Играть с другом") // Action::Friends
    MUI_XYAT("GL", 5, 61, 20, "Назад")          // go back

    /* play bot */
    MUI_FORM(23)                       //
    MUI_LABEL(5, 10, "Игра с ботом:")  //
    MUI_XY("HR", 0, 13)                //
    MUI_LABEL(5, 24, "Сложность:")     //
    MUI_XY("LB", 70, 25)               //
    MUI_LABEL(5, 36, "Оффлайн: ")      //
    MUI_XY("CB", 70, 37)               //
    MUI_XYAT("AB", 5, 48, 6, "Начать") // Action::BotGame
    MUI_XYAT("GL", 5, 60, 20, "Назад") // go back

    /* lichess challenges */
    MUI_FORM(28)                       //
    MUI_LABEL(5, 10, "Вызовы:")        //
    MUI_XY("HR", 0, 13)                //
    MUI_XYAT("LC", 5, 25, 0, "c")      //
    MUI_XYAT("LC", 5, 37, 1, "c")      //
    MUI_XYAT("LC", 5, 49, 2, "c")      //
    MUI_XYAT("GL", 5, 61, 22, "Назад") // go back

    /* lichess active games */
    MUI_FORM(29)                       //
    MUI_LABEL(5, 10, "Текущие игры:")  //
    MUI_XY("HR", 0, 13)                //
    MUI_XYAT("LG", 5, 25, 0, "g")      //
    MUI_XYAT("LG", 5, 37, 1, "g")      //
    MUI_XYAT("LG", 5, 49, 2, "g")      //
    MUI_XYAT("GL", 5, 61, 22, "Назад") // go back

    /* lichess active games */
    MUI_FORM(30)                       //
    MUI_LABEL(5, 10, "Друзья:")        //
    MUI_XY("HR", 0, 13)                //
    MUI_XYAT("LF", 5, 25, 0, "f")      //
    MUI_XYAT("LF", 5, 37, 1, "f")      //
    MUI_XYAT("LF", 5, 49, 2, "f")      //
    MUI_XYAT("GL", 5, 61, 22, "Назад") // go back

    /* prepare game */
    MUI_FORM(40)                            //
    MUI_LABEL(5, 10, "Подготовка игры:")    //
    MUI_XY("HR", 0, 13)                     //
    MUI_XYAT("AB", 5, 25, 3, "Показать")    // Action::Highlight
    MUI_XYAT("AB", 5, 37, 4, "Начать игру") // Action::StartGame
    // MUI_XYAT("GL", 5, 49, 2, "d")        //
    MUI_XYAT("GL", 5, 61, 22, "Назад") // go back

    /* ingame menu */
    MUI_FORM(50)              //
    MUI_LABEL(5, 10, "Меню:") //
    MUI_XY("HR", 0, 13)       //

    MUI_XYAT("AB", 5, 61, 7, "Выйти из игры") // Action::QuitGame
    ;

class Menu
{
private:
  U8G2 u8g2 = U8G2_SDL(U8G2_R0);
  MUIU8G2 mui;

public:
  Menu()
  {
    u8g2.begin();
    mui.begin(u8g2, fds_data, muif_list, sizeof(muif_list) / sizeof(muif_t));
    mui.gotoForm(1, 0);
    draw();
  }
  void draw()
  {
    if (mui.isFormActive())
    {
      u8g2.firstPage();
      do
      {
        mui.draw();
      } while (u8g2.nextPage());
    }
    else
    {
      if (is_menu_closed)
      {
        if (is_game_active)
        {
          mui.gotoForm(50, 1);
        }
        else
        {
          mui.gotoForm(1, 1);
        }
        draw();
        is_menu_closed = false;
      }
      else
      {
        u8g2.clearDisplay();
        is_menu_closed = true;
      }
    }
  }
  void select()
  {
    mui.sendSelect();
    draw();
  }
  void next()
  {
    mui.nextField();
    draw();
  }
  void prev()
  {
    mui.prevField();
    draw();
  }
};