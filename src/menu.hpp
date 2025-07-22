#include "MUIU8g2.h"
#include "U8g2lib.h"
#include "fonts.h"

uint8_t wifi = 0;
uint8_t lichess = 0;

class U8G2_SDL : public U8G2
{
public:
  U8G2_SDL(const u8g2_cb_t *rotation) : U8G2()
  {
    u8g2_SetupBuffer_SDL_128x64_4(&u8g2, rotation);
  }
};

muif_t muif_list[] = {
    /* latin + cyrillic text style */
    MUIF_U8G2_FONT_STYLE(0, u8g2_font_6x12_t_cyrillic),
    /* lichess font */
    MUIF_U8G2_FONT_STYLE(2, custom_font),
    /* test fonts */
    MUIF_U8G2_FONT_STYLE(3, u8g2_font_open_iconic_check_4x_t),
    MUIF_U8G2_FONT_STYLE(4, u8g2_font_iconquadpix_m_all),
    /* label */
    MUIF_U8G2_LABEL(),
    /* horizontal line */
    MUIF_RO("HR", mui_hline),
    /* main menu */
    MUIF_RO("GP", mui_u8g2_goto_data),
    MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pi),
    /* goto button */
    MUIF_BUTTON("G1", mui_u8g2_btn_goto_wm_fi),
    /* wi-fi select */
    MUIF_VARIABLE("IC", &wifi, mui_u8g2_u8_opt_parent_wm_pi),
    MUIF_VARIABLE("OC", &wifi, mui_u8g2_u8_opt_radio_child_w1_pi),
    /* lichess select */
    MUIF_VARIABLE("IL", &lichess, mui_u8g2_u8_opt_parent_wm_pi),
    MUIF_VARIABLE("OL", &lichess, mui_u8g2_u8_opt_radio_child_w1_pi),
};

#define STATUS_PANEL MUI_STYLE(4) MUI_LABEL(96, 12, "AB") MUI_STYLE(0)

fds_t fds_data[] =
    /* main menu */
    MUI_FORM(1)                 //
    STATUS_PANEL                //
        MUI_STYLE(0)            //
    MUI_LABEL(5, 10, "Меню")    //
    MUI_XY("HR", 0, 13)         //
    MUI_DATA("GP",              //
             MUI_2 "Играть|"    //
             MUI_3 "Настройки|" //
             )                  //
    MUI_XYA("GC", 5, 25, 0)     //
    MUI_XYA("GC", 5, 37, 1)     //

    /* chessboard play menu */
    MUI_FORM(2)                          //
    MUI_STYLE(0)                         //
    MUI_LABEL(50, 8, "Играть")           //
    MUI_STYLE(3)                         //
    MUI_XYAT("G1", 22, 46, 13, "A")      //
    MUI_XYAT("G1", 64, 46, 13, "B")      //
    MUI_XYAT("G1", 106, 46, 13, "C")     //
    MUI_STYLE(0)                         //
    MUI_XYAT("G1", 64, 59, 1, " Назад ") //

    /* settings menu */
    MUI_FORM(3)                                              //
    MUI_LABEL(5, 10, "Настройки")                            //
    MUI_XY("HR", 0, 13)                                      //
    MUI_LABEL(5, 24, "Wi-Fi:")                               //
    MUI_XYAT("IC", 64, 24, 12, "Ufanet-55|akim|phone|other") //
    MUI_LABEL(5, 38, "Lichess:")                             //
    MUI_XYAT("IL", 64, 38, 12, "gugich|zaaaz3")              //
    MUI_XYAT("G1", 37, 55, 13, " Доска ")                    //
    MUI_XYAT("G1", 90, 55, 1, " Назад ")                     //

    /* Wi-Fi select */
    MUI_FORM(12)                        //
    MUI_LABEL(5, 10, "Wi-Fi Selection") //
    MUI_XY("HR", 0, 13)                 //
    MUI_XYA("OC", 5, 30, 0)             //
    MUI_XYA("OC", 5, 42, 1)             //
    MUI_XYA("OC", 5, 54, 2)             //

    /* Lichess select */
    MUI_FORM(13)                        //
    MUI_LABEL(5, 10, "Lichess account") //
    MUI_XY("HR", 0, 13)                 //
    MUI_XYA("OL", 5, 30, 0)             //
    MUI_XYA("OL", 5, 42, 1)             //
    MUI_XYA("OL", 5, 54, 2)             //
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