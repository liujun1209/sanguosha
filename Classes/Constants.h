#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include <string>

//主界面菜单tag
static const int MENU_SINGLE_GAME_TAG = 0;
static const int MENU_MULTI_GAME_TAG = 1;
static const int MENU_ABOUT_GAME_TAG = 2;
static const int MENU_EXIT_GAME_TAG = 3;


static const int MENU_INPUT_NAME_TAG = 4;


static const std::string CONFIG_KEY_VERSION = "version";
static const std::string CONFIG_KEY_NAME = "name";


static const std::string STRING_KEY_INPUT = "inputNick";
static const std::string STRING_KEY_LOBBY_ROOM_NAME = "lobbyRoomName";



static const short NET_BROADCAST_PORT = 5868;
static const short NET_SERVER_PORT = 9126;


static const char* RESOURCES_PLIST_FORMAT_STR = "image/Big_%s.plist";
static const char* RESOURCES_PVR_FORMAT_STR = "image/Big_%s.pvr";

//resource
static const char* IMAGE_LOBBY_BG_PATH = "lobby_gamebg.png";
static const char* IMAGE_LOBBY_TITLE_PATH = "lobby_title_up.png";

static const char* IMAGE_LOBBY_USER_BG_PATH = "lobby_board_info_user_head_bg.png";
static const char* IMAGE_LOBBY_USER_HEAD_PATH = "lobby_board_info_user_head_man.png";
static const char* IMAGE_LOBBY_BOARD_BG_PATH = "lobby_board_info_bg.png";


static const char* IMAGE_TABLE_BG_PATH = "table_bg.png";
static const char* IMAGE_TABLE_CONTROLLER_USER_PATH = "table_user.png";
static const char* IMAGE_TABLE_CONTROLLER_EQU_PATH = "table_equ.png";
static const char* IMAGE_TABLE_MENU_PATH = "table_menu.png";
static const char* IMAGE_TABLE_PILES_PATH = "table_piles.png";
static const char* IMAGE_TABLE_PLAYER_BG_PATH = "table_player.png";
static const char* IMAGE_TABLE_ROLES_PATH = "table_roles.png";


static const char* IMAGE_TABLE_PLAYER_BLOOD_BG_PATH = "blood_point_bg.png";
static const char* IMAGE_TABLE_PLAYER_BLOOD_1_PATH = "blood_point_1.png";
static const char* IMAGE_TABLE_PLAYER_BLOOD_3_PATH = "blood_point_2.png";
static const char* IMAGE_TABLE_PLAYER_BLOOD_5_PATH = "blood_point_3.png";


static const char* IMAGE_BUTTON_CREATE_PATH = "button_common_button_creat_table_1.png";
static const char* IMAGE_BUTTON_START_PATH = "button_common_button_start_table.png";
static const char* IMAGE_BUTTON_BACK_PATH = "button_common_button_back_1.png";

static const char* IMAGE_LOBBY_TABLE_CELL_BG_PATH = "lobby_table_list_row_bg_1.png";
static const char* IMAGE_LOBBY_TABLE_CELL_SELECT_BG_PATH = "lobby_table_list_row_bg_2.png";

static const char* IMAGE_ROOM_SEAT_OPEN_BG_PATH = "room_OpenSeatBack.png";
static const char* IMAGE_ROOM_SEAT_CLOSE_BG_PATH = "room_CloseSeatBack.png";
static const char* IMAGE_ROOM_SEAT_COVER_PATH = "room_cover.png";
static const char* IMAGE_ROOM_SEAT_HEAD_PATH = "room_head.png";
static const char* IMAGE_ROOM_SEAT_HOST_PATH = "room_Host.png";
static const char* IMAGE_ROOM_SEAT_READY_PATH = "room_Ready.png";
static const char* IMAGE_ROOM_SEAT_CLOSE_PATH = "room_Close.png";



static const char* ANIMATION_PLIST_PATH = "emotion_animation.plist";
static const char* ANIMATION_PNG_PATH = "image/emotion/animation.png";

static const char* ANIMATION_DAMAGE_KEY = "damage";
static const int ANIMATION_DAMAGE_COUNT = 5;
static const char* ANIMATION_DAMAGE_FORMAT_STR = "damage_%d.png";

static const char* ANIMATION_SLASH_KEY = "slash";
static const int ANIMATION_SLASH_COUNT = 14;
static const char* ANIMATION_SLASH_FORMAT_STR = "slash_%d.png";

static const char* ANIMATION_JINK_KEY = "jink";
static const int ANIMATION_JINK_COUNT = 23;
static const char* ANIMATION_JINK_FORMAT_STR = "jink_%d.png";

static const char* ANIMATION_PEACH_KEY = "peach";
static const int ANIMATION_PEACH_COUNT = 17;
static const char* ANIMATION_PEACH_FORMAT_STR = "peach_%d.png";



static const char* FONT_NAME_SONG = "宋体";
static const char* FONT_NAME_HEI = "黑体";
static const int FONT_SIZE_NAME = 20;



static const int ERROR_CODE_ROOM_FULL = 101;
static const int ERROR_CODE_VERSION_INVALIDATE = 102;



#endif