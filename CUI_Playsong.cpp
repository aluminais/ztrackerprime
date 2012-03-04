#include "zt.h"



CUI_Playsong::CUI_Playsong(void) {

    PatternDisplay *p;
    VUPlay *vu;
//    LCDDisplay *lcd;
    //UI = new UserInterface;

    UI_PatternDisplay = new UserInterface;
    UI_VUMeters = new UserInterface;

    p = new PatternDisplay;
    p->x = 1;
    p->y = 20 ;
    p->ysize = 30;

    UI_PatternDisplay->add_element(p,0);
    //delete p;
/*
    lcd = new LCDDisplay;
    lcd->str = "Big Time!";
    lcd->length = strlen(lcd->str);
    lcd->x = col(2);
    lcd->y = row(12);

//  UI->add_gfx(lcd,0);
    delete lcd;
*/
    vu = new VUPlay;
    vu->cur_order = vu->cur_pattern = vu->cur_row = 0;
    vu->num_channels = 32;
    UI_VUMeters->add_element(vu,0);

    UI = UI_PatternDisplay;
}

CUI_Playsong::~CUI_Playsong(void) {
 //   if (UI) delete UI; UI = NULL;
  if (UI_PatternDisplay) delete UI_PatternDisplay; UI_PatternDisplay = NULL;
  if (UI_VUMeters) delete UI_VUMeters; UI_VUMeters = NULL;
  UI = NULL;
}

void CUI_Playsong::enter(void) {
    need_refresh = 1;
    cur_state = STATE_PLAY;
}

void CUI_Playsong::leave(void) {

}

void CUI_Playsong::update() {
    int key=0,act=0;
    KBMod kstate = Keys.getstate();
    UI->update();
    if (Keys.size()) {
        key = Keys.getkey();
        switch(key) {
            case DIK_RIGHT: 
            case DIK_ADD:
                if ((key==DIK_RIGHT && kstate == KS_CTRL) || key!=DIK_RIGHT)
                    if (ztPlayer->playing)
                        ztPlayer->ffwd();
                break;
            case DIK_LEFT:
            case DIK_MINUS:
                if ((key==DIK_LEFT && kstate == KS_CTRL) || key!=DIK_RIGHT)
                    if (ztPlayer->playing)
                        ztPlayer->rewind();
                break;
            case DIK_S:
                Keys.insert(SDLK_F10, KMOD_ALT);
                act++;
                break;
            case DIK_G:
                cur_edit_pattern = ztPlayer->playing_cur_pattern;
                switch_page(UIP_Patterneditor);
                cur_state = STATE_PEDIT;
                UIP_Patterneditor->clear = 1;
                break;

            case DIK_PGUP:
            case DIK_PGDN:
                if (UI == UI_PatternDisplay)
                  UI = UI_VUMeters;
                else
                  UI = UI_PatternDisplay;
                clear++;
                act++;
                break;
        }
    }
    if (act) need_refresh++;
}

void CUI_Playsong::draw(Drawable *S) {
//  char str[256];
    if (S->lock()==0) {
      if (clear)
          S->fillRect(0,row(15),CONSOLE_WIDTH,row(50)/*410*/,COLORS.Background);
        printtitle(11,"Play Song",COLORS.Text,COLORS.Background,S);
        need_refresh = 0;
        updated=2;
        UI->draw(S);
        S->unlock();
      clear=0;
    }
}




