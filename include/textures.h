#pragma once

#include "values.h"
#include <raylib.h>
#include <stdint.h>

Texture2D bg_texture;
Texture2D cookie_texture;
Texture2D small_cookie_texture;
Texture2D gold_cookie_texture;
Texture2D shine_texture;
Texture2D store_button_texture;
Texture2D cursor_store_texture;
Texture2D grandma_store_texture;
Texture2D farm_store_texture;
Texture2D mine_store_texture;

float ui_scale;
float shine_rotation;
float cookie_scale;

int SW;
int SH;

void LoadTextures();
void DrawBackground();
void DrawCookie();
void DrawGoldCookie(Vector2 coords, float scale, Color tint);
void DrawShine();
void DrawStore(int current_store_select);
