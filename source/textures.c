#include "textures.h"
#include "fonts.h"
#include "raylib.h"
#include "values.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern const uint8_t cookie_bin[];
extern const uint32_t cookie_bin_size;

extern const uint8_t bg_bin[];
extern const uint32_t bg_bin_size;

extern const uint8_t small_cookie_bin[];
extern const uint32_t small_cookie_bin_size;

extern const uint8_t gold_cookie_bin[];
extern const uint32_t gold_cookie_bin_size;

extern const uint8_t shine_bin[];
extern const uint32_t shine_bin_size;

extern const uint8_t store_button_bin[];
extern const uint32_t store_button_bin_size;

extern const uint8_t cursor_store_bin[];
extern const uint32_t cursor_store_bin_size;

extern const uint8_t grandma_store_bin[];
extern const uint32_t grandma_store_bin_size;

extern const uint8_t gray_bin[];
extern const uint32_t gray_bin_size;

extern const uint8_t farm_store_bin[];
extern const uint32_t farm_store_bin_size;

extern const uint8_t mine_store_bin[];
extern const uint32_t mine_store_bin_size;

void LoadTextures() {
  SW = GetScreenWidth();
  SH = GetScreenHeight();
  ui_scale = SH / 720.0f;

  Image bg_image = LoadImageFromMemory(".png", bg_bin, bg_bin_size);
  bg_texture = LoadTextureFromImage(bg_image);
  UnloadImage(bg_image);

  Image cookie_image = LoadImageFromMemory(".png", cookie_bin, cookie_bin_size);
  cookie_texture = LoadTextureFromImage(cookie_image);
  UnloadImage(cookie_image);
  SetTextureFilter(cookie_texture, TEXTURE_FILTER_BILINEAR);
  cookie_scale = 0.6f * ui_scale;

  Image shine_image = LoadImageFromMemory(".png", shine_bin, shine_bin_size);
  shine_texture = LoadTextureFromImage(shine_image);
  UnloadImage(shine_image);

  Image store_button_image =
      LoadImageFromMemory(".png", store_button_bin, store_button_bin_size);
  store_button_texture = LoadTextureFromImage(store_button_image);
  UnloadImage(store_button_image);

  Image small_cookie_image =
      LoadImageFromMemory(".png", small_cookie_bin, small_cookie_bin_size);
  small_cookie_texture = LoadTextureFromImage(small_cookie_image);
  UnloadImage(small_cookie_image);

  Image cursor_store_image =
      LoadImageFromMemory(".png", cursor_store_bin, cursor_store_bin_size);
  cursor_store_texture = LoadTextureFromImage(cursor_store_image);
  UnloadImage(cursor_store_image);

  Image grandma_store_image =
      LoadImageFromMemory(".png", grandma_store_bin, grandma_store_bin_size);
  grandma_store_texture = LoadTextureFromImage(grandma_store_image);
  UnloadImage(grandma_store_image);

  Image farm_store_image =
      LoadImageFromMemory(".png", farm_store_bin, farm_store_bin_size);
  farm_store_texture = LoadTextureFromImage(farm_store_image);
  UnloadImage(farm_store_image);

  Image mine_store_image =
      LoadImageFromMemory(".png", mine_store_bin, mine_store_bin_size);
  mine_store_texture = LoadTextureFromImage(mine_store_image);
  UnloadImage(mine_store_image);

  Image gold_cookie_image =
      LoadImageFromMemory(".png", gold_cookie_bin, gold_cookie_bin_size);
  gold_cookie_texture = LoadTextureFromImage(gold_cookie_image);
  UnloadImage(gold_cookie_image);
}

void DrawBackground() {
  for (float y = 0.0f; y < (float)SH; y += bg_texture.height) {
    for (float x = 0.0f; x < (float)SW; x += bg_texture.width) {
      DrawTexture(bg_texture, (int)x, (int)y, WHITE);
    }
  }
}

void DrawCookie() {
  float w = cookie_texture.width * cookie_scale;
  float h = cookie_texture.height * cookie_scale;

  Rectangle sourceRec = {0.0f, 0.0f, (float)cookie_texture.width,
                         (float)cookie_texture.height};

  Rectangle destRec = {SW * 0.15f, SH * 0.45f, w, h};

  Vector2 origin = {w * 0.5f, h * 0.5f};

  DrawTexturePro(cookie_texture, sourceRec, destRec, origin, 0.0f, WHITE);
}

void DrawGoldCookie(Vector2 coords, float scale, Color tint) {
  // coords — left up corner of base size; + size in round of center,
  // for pulsation dont lag right-up
  float base = gold_cookie_texture.width * ui_scale;
  float size = base * scale;
  Vector2 center = {coords.x + base * 0.5f, coords.y + base * 0.5f};

  Rectangle sourceRec = {0.0f, 0.0f, (float)gold_cookie_texture.width,
                         (float)gold_cookie_texture.height};
  Rectangle destRec = {center.x, center.y, size, size};
  Vector2 origin = {size * 0.5f, size * 0.5f};

  DrawTexturePro(gold_cookie_texture, sourceRec, destRec, origin, 0.0f, tint);
}

void DrawShine() {

  float shine_w = shine_texture.width * 3.0f * ui_scale;
  float shine_h = shine_texture.height * 3.0f * ui_scale;

  Rectangle sourceRec = {0.0f, 0.0f, (float)shine_texture.width,
                         (float)shine_texture.height};

  Rectangle destRec = {SW * 0.15f, SH * 0.45, shine_w, shine_h};
  Vector2 origin = {shine_w * 0.5f, shine_h * 0.5f};

  DrawTexturePro(shine_texture, sourceRec, destRec, origin, shine_rotation,
                 WHITE);
}

void DrawStoreItem(Texture2D icon, const char *label, float price, int index,
                   int selected, float y) {
  float btn_w = store_button_texture.width * ui_scale;
  float btn_h = store_button_texture.height * ui_scale;
  float button_x = SW - btn_w;
  float button_y = y;

  bool can_afford = cookies >= price;
  Color tint = can_afford ? WHITE : GRAY;

  // button draw
  DrawTextureEx(store_button_texture, (Vector2){button_x, button_y}, 0.0f,
                ui_scale, tint);

  if (index == selected) {
    DrawRectangleLinesEx((Rectangle){button_x, button_y, btn_w, btn_h},
                         3.0f * ui_scale, WHITE);
  }

  // icon draw
  float icon_w = icon.width * ui_scale;
  float icon_h = icon.height * ui_scale;
  float pad = 10.0f * ui_scale;

  float icon_x = button_x + pad;
  float icon_y = button_y + (btn_h - icon_h) * 0.5f;

  DrawTextureEx(icon, (Vector2){icon_x, icon_y}, 0.0f, ui_scale, tint);

  // name + price
  float title_size = 48.0f * ui_scale;
  float price_size = 16.0f * ui_scale;

  const char *safe_label = (label != NULL) ? label : "";

  Vector2 title_metrics = MeasureTextEx(fonts[0], safe_label, title_size, 0);

  Vector2 price_metrics =
      MeasureTextEx(fonts[0], TextFormat("%.0f", price), price_size, 0);

  float gap = 2.0f * ui_scale;
  float block_h = title_metrics.y + gap + price_metrics.y;
  float block_y = button_y + (btn_h - block_h) * 0.5f;

  float text_x = icon_x + icon_w + pad - 3.0f * ui_scale;
  float title_y = block_y;

  float price_y = block_y + title_metrics.y + gap - 7.0f * ui_scale;

  DrawTextEx(fonts[0], safe_label, (Vector2){text_x, title_y}, title_size, 0.0f,
             tint);

  // cookie + price
  float cookie_scale = price_metrics.y / small_cookie_texture.height;
  float cookie_w = small_cookie_texture.width * cookie_scale;

  DrawTextureEx(small_cookie_texture, (Vector2){text_x, price_y}, 0.0f,
                cookie_scale, tint);

  Color price_color =
      can_afford ? (Color){1, 99, 1, 255} : (Color){220, 80, 80, 255};

  DrawTextEx(fonts[0], TextFormat("%.0f", price),
             (Vector2){text_x + cookie_w + 3.0f * ui_scale, price_y},
             price_size, 0.0f, price_color);
}

void DrawStore(int current_store_select) {
  float row_h = store_button_texture.height * ui_scale;
  float row_gap = 0.0f * ui_scale;
  float start_y = SH * 0.15f;

  float prices[4] = {cursors_price, grandmas_price, farms_price, mines_price};

  for (int i = 0; i < 4; i++) {
    float y = start_y + i * (row_h + row_gap);
    DrawStoreItem(store_items[i].icon, store_items[i].name, prices[i], i,
                  current_store_select, y);
  }
}
