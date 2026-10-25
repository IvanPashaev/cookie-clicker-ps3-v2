#pragma once
#include "textures.h"
#include <raylib.h>
typedef enum store_select { CURSOR, GRANDMA, FARM, MINE } store_select;

enum gold_cookie_effects { LUCKY, FRENZY, BUILDING_SPECIAL };

typedef struct {
  Texture2D icon;
  const char *name;
  float price;
} StoreItem;

extern double cookies;
extern double cookies_per_second;

extern long long cursors;
extern float cursors_price;
extern float cursors_base_price;

extern long long grandmas;
extern float grandmas_price;
extern float grandmas_base_price;

extern long long farms;
extern float farms_price;
extern float farms_base_price;

extern long long mines;
extern float mines_price;
extern float mines_base_price;

void LoadStoreData(void);

extern StoreItem store_items[4];
