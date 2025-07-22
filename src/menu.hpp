#include "MUIU8g2.h"
#include "U8g2lib.h"
#include "fonts.h"
#include "lichess.hpp"

constexpr uint8_t WIFI_ENABLED_BIT = 0x01;    // 0b00000001 (бит 0)
constexpr uint8_t BOARD_CONNECTED_BIT = 0x02; // 0b00000010 (бит 1)
constexpr uint8_t BOARD_TYPE_BIT = 0x04;      // 0b00000100 (бит 2)

uint8_t wifi = 0;
uint8_t brightness = 0;
uint8_t statusBar = 0b00000010;

uint16_t lichessChallengeSelection = 0;
uint16_t lichessGameSelection = 0;
uint16_t lichessFriendSelection = 0;

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
    char a = (statusBar & WIFI_ENABLED_BIT) != 0 ? 'A' : 'B';
    char b = (statusBar & BOARD_CONNECTED_BIT) != 0 ? ((statusBar & BOARD_TYPE_BIT) ? 'a' : 'b') : 'c';
    char str[3] = {a, b, '\0'};
    u8g2_DrawUTF8(mui_get_U8g2(ui), mui_get_x(ui), mui_get_y(ui), str);
  }
  return 0;
}
uint8_t mui_action(mui_t *ui, uint8_t msg)
{
  if (msg == MUIF_MSG_DRAW)
  {
    if (strcmp(ui->text, "c") == 0) // challenges
    {
      lichess::fetch_challenges();
      mui_SaveForm(ui);
      return mui_GotoForm(ui, 28, 0);
    }
    else if (strcmp(ui->text, "C") == 0)
    {
      u8g2_t *u8g2 = mui_get_U8g2(ui);
      mui_u8g2_list_t *list = (mui_u8g2_list_t *)muif_get_data(ui->uif);
      uint16_t *selection = mui_u8g2_list_get_selection_ptr(list);
      void *data = mui_u8g2_list_get_data_ptr(list);
      mui_u8g2_get_list_element_cb element_cb = mui_u8g2_list_get_element_cb(list);

      uint16_t pos = ui->arg; // remember the arg value, because it might be overwritten
      pos += ui->form_scroll_top;
      mui_u8g2_draw_button_pi(ui, 128 - mui_get_x(ui) * 2, mui_get_x(ui), element_cb(data, pos) + 1);
    }

    if (strcmp(ui->text, "g") == 0) // challenges
    {
      lichess::fetch_games();
      mui_SaveForm(ui);
      return mui_GotoForm(ui, 29, 0);
    }
    else if (strcmp(ui->text, "G") == 0)
    {
      u8g2_t *u8g2 = mui_get_U8g2(ui);
      mui_u8g2_list_t *list = (mui_u8g2_list_t *)muif_get_data(ui->uif);
      uint16_t *selection = mui_u8g2_list_get_selection_ptr(list);
      void *data = mui_u8g2_list_get_data_ptr(list);
      mui_u8g2_get_list_element_cb element_cb = mui_u8g2_list_get_element_cb(list);

      uint16_t pos = ui->arg; // remember the arg value, because it might be overwritten
      pos += ui->form_scroll_top;
      mui_u8g2_draw_button_pi(ui, 128 - mui_get_x(ui) * 2, mui_get_x(ui), element_cb(data, pos) + 1);
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
    MUIF_BUTTON("GT", mui_u8g2_btn_goto_wm_fi), // goto button
    MUIF_BUTTON("PB", mui_u8g2_btn_goto_wm_pf), // play button (without borders)
    MUIF_BUTTON("BC", mui_u8g2_btn_back_wm_fi), // back button
    MUIF_BUTTON("EX", mui_u8g2_btn_exit_wm_fi), // exit button
    /* action */
    MUIF_RO("AC", mui_action),
    /* lichess challenges */
    MUIF_U8G2_U16_LIST("LC", &lichessChallengeSelection, NULL, lichess::challenges_get_str, lichess::challenges_get_cnt, mui_action),
    MUIF_U8G2_U16_LIST("LG", &lichessGameSelection, NULL, lichess::games_get_str, lichess::games_get_cnt, mui_action),

    /* status bar */
    MUIF_RO("SB", mui_draw_status_bar),
    /* wi-fi select */
    MUIF_VARIABLE("IC", &wifi, mui_u8g2_u8_opt_parent_wm_pi),
    MUIF_VARIABLE("OC", &wifi, mui_u8g2_u8_opt_radio_child_w1_pi),
    /* lichess select */
    MUIF_VARIABLE("IL", &lichess::accountId, mui_u8g2_u8_opt_parent_wm_pi),
    MUIF_VARIABLE("OL", &lichess::accountId, mui_u8g2_u8_opt_radio_child_w1_pi),
    /* brightness bar */
    MUIF_U8G2_U8_MIN_MAX_STEP("B0", &brightness, 0, 16, 2, MUI_MMS_2X_BAR | MUI_MMS_SHOW_VALUE, mui_u8g2_u8_bar_wm_mud_pi),

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
    MUI_XYAT("BC", 64, 59, 1, " Назад ") //

    /* settings */
    MUI_FORM(3)                                              //
    MUI_LABEL(5, 10, "Настройки")                            //
    MUI_XY("HR", 0, 13)                                      //
    MUI_LABEL(5, 24, "Wi-Fi:")                               //
    MUI_XYAT("IC", 64, 24, 12, "Ufanet-55|akim|phone|other") //
    MUI_LABEL(5, 38, "Lichess:")                             //
    MUI_XYAT("IL", 64, 38, 13, "gugich|zaaaz3")              //
    MUI_XYAT("GT", 37, 55, 14, " Доска ")                    //
    MUI_XYAT("BC", 90, 55, 1, " Назад ")                     //

    /* Wi-Fi select */
    MUI_FORM(12)                        //
    MUI_LABEL(5, 10, "Wi-Fi Selection") //
    MUI_XY("HR", 0, 13)                 //
    MUI_XYA("OC", 5, 30, 0)             //
    MUI_XYA("OC", 5, 42, 1)             //
    MUI_XYA("OC", 5, 54, 2)             //

    /* Lichess acc select */
    MUI_FORM(13)                        //
    MUI_LABEL(5, 10, "Lichess account") //
    MUI_XY("HR", 0, 13)                 //
    MUI_XYA("OL", 5, 30, 0)             //
    MUI_XYA("OL", 5, 42, 1)             //
    MUI_XYA("OL", 5, 54, 2)             //

    /* board settings */
    MUI_FORM(14)                         //
    MUI_LABEL(5, 10, "Настройки доски")  //
    MUI_XY("HR", 0, 13)                  //
    MUI_LABEL(5, 24, "Яркость:")         //
    MUI_XY("B0", 64, 24)                 //
    MUI_XYAT("BC", 90, 55, 3, " Назад ") //

    /* play chess */
    MUI_FORM(20)                       //
    MUI_LABEL(5, 10, "Шахматы")        //
    MUI_XY("HR", 0, 13)                //
    MUI_DATA("GP",                     //
             MUI_21 "Вдвоем на доске|" //
             MUI_22 "Играть онлайн|"   //
             MUI_23 "Играть с ботом|"  //
             MUI_2 "Назад|"            //
             )                         //
    MUI_XYA("GC", 5, 25, 0)            //
    MUI_XYA("GC", 5, 37, 1)            //
    MUI_XYA("GC", 5, 49, 2)            //
    MUI_XYA("GC", 5, 61, 3)            //

    /* play offline */
    MUI_FORM(21)                        //
    MUI_LABEL(5, 10, "Настройки доски") //

    /* play lichess */
    MUI_FORM(22)                       //
    MUI_LABEL(5, 10, "Lichess")        //
    MUI_XY("HR", 0, 13)                //
    MUI_DATA("GP",                     //
             MUI_25 "Вызовы|"          //
             MUI_26 "Активные игры|"   //
             MUI_27 "Играть с другом|" //
             MUI_20 "Назад|"           //
             )                         //
    MUI_XYA("GC", 5, 25, 0)            //
    MUI_XYA("GC", 5, 37, 1)            //
    MUI_XYA("GC", 5, 49, 2)            //
    MUI_XYA("GC", 5, 61, 3)            //

    /* lichess challenges */
    MUI_FORM(25)                     //
    MUI_DATA("AC", "c")              //
    MUI_LABEL(64, 32, "Загрузка...") //

    /* lichess games */
    MUI_FORM(26)                     //
    MUI_DATA("AC", "g")              //
    MUI_LABEL(64, 32, "Загрузка...") //

    /* lichess start game */
    MUI_FORM(27)                     //
    MUI_DATA("AC", "s")              //
    MUI_LABEL(64, 32, "Загрузка...") //

    /* lichess challenges */
    MUI_FORM(28)                  //
    MUI_LABEL(5, 10, "Вызовы:")   //
    MUI_XY("HR", 0, 13)           //
    MUI_XYAT("LC", 5, 25, 0, "C") //
    MUI_XYAT("LC", 5, 37, 1, "C") //
    MUI_XYAT("LC", 5, 49, 2, "C") //
    MUI_XYAT("LC", 5, 61, 3, "C") //

    /* lichess active games */
    MUI_FORM(29)                      //
    MUI_LABEL(5, 10, "Текущие игры:") //
    MUI_XY("HR", 0, 13)               //
    MUI_XYAT("LG", 5, 25, 0, "G")     //
    MUI_XYAT("LG", 5, 37, 1, "G")     //
    MUI_XYAT("LG", 5, 49, 2, "G")     //
    MUI_XYAT("LG", 5, 61, 3, "G");    //

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
      std::cout << mui.getCurrentFormId() << std::endl;
      u8g2.firstPage();
      do
      {
        mui.draw();
      } while (u8g2.nextPage());
    }
    else
    {
      std::cout << "no form" << std::endl;
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