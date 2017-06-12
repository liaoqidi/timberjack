//
//  Settting.h
//  胖次英雄
//
//  Created by lyon on 14-3-3.
//
//
#include "global.h"
#ifndef _____Setting_h
#define _____Setting_h

#define GAME_FONT_NAME	"CevicheOne-Regular.ttf"

#define NAME_BACKGROUND_1_0     "scene_background_1_0.png"
#define NAME_BACKGROUND_1_1     "scene_background_1_1.png"
#define NAME_BACKGROUND			"scene_background_0_0.png"
#define NAME_BUILD				"scene_build_0.png"
#define NAME_PANTS				"scene_build_panty_0.png"
#define NAME_WARN_FINGER		"ui_img_getreadyfinger_01.png"
#define NAME_WARN_DOWN          "ui_img_getready_finger_02.png"
#define NAME_WARN_TEXT          "ui_img_getreadytap_01.png"
#define NAME_READY              "ui_img_getready_01.png"

#define LAYER_BACKGROUND_0      0
#define LAYER_BACKGROUND_1      0
#define LAYER_OBSTACLE          2
#define LAYER_HERO              3

#ifdef iphone
#define FILE_PLIST_PANTS        "panty_iphone.plist"
#define FILE_PLIST_CELL         "ui_iphone_cell.plist"
#define FILE_PLIST_BACKGROUND   "iphone_scene_resources.plist"
#define FILE_PLIST_HERO         "hero_iphone.plist"

#define FILE_JSON_START_LAYOUT  "pantyhero_iphone_startgame.ExportJson"
#define FILE_JSON_READY_LAYOUT  "pantyhero_iphone_getready.ExportJson"
#define FILE_JSON_OVER_LAYOUT   "pantyhero_iphone_gameover.ExportJson"
#define FILE_JSON_SHOP_LAYOUT   "pantyhero_iphone_shop.ExportJson"
#endif

#define FILE_PNG_BACKGROUND     "background.png"
#define FILE_PNG_HERO           "hero.png"
#define FILE_PLIST_UI           "ui_data_pantieshero_all_00.plist"
#define FILE_PNG_UI             "ui_data_pantieshero_all_00.png"


#define FPS_HERO                6


#define KEY_HAS_HERO            "1_______%d"
#define KEY_HAS_PANTY           "2_______%d"
#define KEY_HAS_EXCHANGE        "3_______%d"

#define KEY_DATA_SCORE          "data_score"
#define KEY_DATA_SCORE_MAX      "data_score_max"
#define KEY_DATA_PANTS          "data_pants"
#define KEY_DATA_HERO10         "data_hero10"
#define KEY_DATA_HERO20         "data_hero20"
#define KEY_DATA_RELIVE         "data_relive"
#define KEY_USED_HERO_ID        "used_hero_id"
#endif
