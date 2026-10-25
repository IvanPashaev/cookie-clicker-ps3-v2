#include <malloc.h>
#include <math.h>
#include <raylib.h>
#include <rlgl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "audio.h"
#include "fonts.h"
#include "textures.h"
#include "values.h"

#define GOLD_COOKIE_LIFETIME 13.0f
#define GOLD_COOKIE_MIN_DELAY 5.0f
#define GOLD_COOKIE_MAX_DELAY 15.0f
#define GOLD_COOKIE_DELAY_MULTIPLIER 60.0f
#define GOLD_COOKIE_SPLASH_TIME 2.5f
#define FRENZY_TIME 15.0f
#define FRENZY_MULTIPLIER 7.0

static int RandomInt(int max_exclusive) {
  if (max_exclusive <= 1) {
    return 0;
  }
  return (int)((float)rand() / ((float)RAND_MAX + 1.0f) * (float)max_exclusive);
}

static enum gold_cookie_effects RandomGoldCookieEffect(void) {
  int roll = RandomInt(100);
  if (roll < 45) {
    return LUCKY;
  }
  if (roll < 90) {
    return FRENZY;
  }
  return BUILDING_SPECIAL;
}

static float RandomGoldCookieDelay(void) {
  return (GOLD_COOKIE_MIN_DELAY +
          ((float)rand() / (float)RAND_MAX) *
              (GOLD_COOKIE_MAX_DELAY - GOLD_COOKIE_MIN_DELAY)) *
         GOLD_COOKIE_DELAY_MULTIPLIER;
}

static Vector2 RandomGoldCookiePosition(void) {
  float size = gold_cookie_texture.width * ui_scale;
  float margin = 8.0f * ui_scale;
  float max_x = (float)SW - size - margin;
  float max_y = (float)SH - size - margin;
  if (max_x < margin)
    max_x = margin;
  if (max_y < margin)
    max_y = margin;

  Vector2 pos;
  pos.x = margin + (float)(rand() % (int)fmaxf(1.0f, max_x - margin + 1.0f));
  pos.y = margin + (float)(rand() % (int)fmaxf(1.0f, max_y - margin + 1.0f));
  return pos;
}

void Initialize() {
  u32 spu;
  sysSpuImage spu_image;
  sysSpuInitialize(6, 5);
  sysSpuRawCreate(&spu, NULL);
  sysSpuImageImport(&spu_image, spu_soundmodule_bin, 0);
  sysSpuRawImageLoad(spu, &spu_image);
  SND_Init(spu);
}

void UpdateCPSValue() {
  cookies_per_second = (double)cursors * 0.1 + (double)grandmas * 1.0 +
                       (double)farms * 4.0 + (double)mines * 47;
}

int main() {
  Initialize(); // init spu module for audio playing
  InitWindow(1280, 720, "Cookie Clicker PS3 edition");
  LoadFonts();
  LoadTextures();
  LoadStoreData();
  UpdateCPSValue();
  srand(time(NULL)); // random seed from current time
  enum store_select current_store_select = CURSOR;
  enum gold_cookie_effects gold_cookie_effect = LUCKY;

  float gold_cookie_timer = 0.0f;
  float gold_cookie_hide_timer = 0.0f;
  float gold_cookie_time_spawn = RandomGoldCookieDelay();
  bool gold_cookie_spawned = false;
  Vector2 gold_cookie_coords = {0};
  float frenzy_timer = 0.0f;
  double cps_multiplier = 1.0;

  // main game loop
  while (!WindowShouldClose()) {

    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
      cookies++;
      TRACELOG(LOG_WARNING, "%f", cookies);
      cookie_scale = 0.5f * ui_scale;

      PlayRandomClickSound();
    } else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
      if (current_store_select != 3) {
        current_store_select++;
      }
    } else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
      if (current_store_select > 0) {
        current_store_select--;
      }
    } else if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
      switch (current_store_select) {
      case CURSOR: {
        if (cookies >= cursors_price) {
          cookies -= cursors_price;
          cursors++;
          cursors_price = cursors_base_price * powf(1.15f, (float)cursors);
          PlayRandomBuySound();
          UpdateCPSValue();
        }
        break;
      }
      case GRANDMA: {
        if (cookies >= grandmas_price) {
          cookies -= grandmas_price;
          grandmas++;
          grandmas_price = grandmas_base_price * powf(1.15f, (float)grandmas);
          PlayRandomBuySound();
          UpdateCPSValue();
        }
        break;
      }
      case FARM: {
        if (cookies >= farms_price) {
          cookies -= farms_price;
          farms++;
          farms_price = farms_base_price * powf(1.15f, (float)farms);
          PlayRandomBuySound();
          UpdateCPSValue();
        }
        break;
      }
      case MINE: {
        if (cookies >= mines_price) {
          cookies -= mines_price;
          mines++;
          mines_price = mines_base_price * powf(1.15f, (float)mines);
          PlayRandomBuySound();
          UpdateCPSValue();
        }
        break;
      }
      }
    }

    if (gold_cookie_spawned &&
        IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_THUMB)) {
      gold_cookie_effect = RandomGoldCookieEffect();

      Vector2 splash_center = {SW * 0.5f, SH * 0.88f};
      float splash_size = 48.0f * ui_scale;

      switch (gold_cookie_effect) {
      case LUCKY: {

        double bonus = fmin(cookies * 0.15, cookies_per_second * 900.0) + 13.0;
        cookies += bonus;
        AddSplash("Lucky!", GOLD, splash_center, splash_size,
                  GOLD_COOKIE_SPLASH_TIME);
        break;
      }
      case FRENZY: {
        frenzy_timer = FRENZY_TIME;
        AddSplash("Frenzy!", GOLD, splash_center, splash_size,
                  GOLD_COOKIE_SPLASH_TIME);
        break;
      }
      case BUILDING_SPECIAL: {
        long long buildings = cursors + grandmas + farms + mines;
        cookies += cookies * 0.10 + (double)buildings * 10.0;
        AddSplash("Building special!", GOLD, splash_center, splash_size,
                  GOLD_COOKIE_SPLASH_TIME);
        break;
      }
      }

      PlayRandomBuySound();

      // after click cookie despawn, timers restarts
      gold_cookie_spawned = false;
      gold_cookie_hide_timer = 0.0f;
      gold_cookie_timer = 0.0f;
      gold_cookie_time_spawn = RandomGoldCookieDelay();
    }

    float deltaTime = GetFrameTime();

    // frenzy in a moment multiply CPS
    if (frenzy_timer > 0.0f) {
      frenzy_timer -= deltaTime;
      if (frenzy_timer < 0.0f) {
        frenzy_timer = 0.0f;
      }
    }
    cps_multiplier = (frenzy_timer > 0.0f) ? FRENZY_MULTIPLIER : 1.0;

    // every frame add cookies
    cookies += cookies_per_second * cps_multiplier * deltaTime;

    // spawn and despawn by timer (gold cookie)
    if (!gold_cookie_spawned) {
      gold_cookie_timer += deltaTime;
      if (gold_cookie_timer >= gold_cookie_time_spawn) {
        gold_cookie_coords = RandomGoldCookiePosition();
        gold_cookie_spawned = true;
        gold_cookie_hide_timer = 0.0f;
        gold_cookie_timer = 0.0f;
      }
    } else {
      gold_cookie_hide_timer += deltaTime;
      if (gold_cookie_hide_timer >= GOLD_COOKIE_LIFETIME) {
        gold_cookie_spawned = false;
        gold_cookie_hide_timer = 0.0f;
        gold_cookie_timer = 0.0f;
        gold_cookie_time_spawn = RandomGoldCookieDelay();
      }
    }

    BeginDrawing();

    DrawBackground();
    DrawShine();
    DrawCookie();
    DrawStore(current_store_select);
    DrawTextEx(fonts[0], TextFormat("%.0f cookies", cookies),
               (Vector2){SW * 0.05f, SH * 0.1}, 64.0f * ui_scale, 0.0f, WHITE);

    DrawTextEx(fonts[0],
               TextFormat("%.1f cookies per second",
                          cookies_per_second * cps_multiplier),
               (Vector2){SW * 0.03f, SH * 0.2}, 32.0f * ui_scale, 0.0f, WHITE);

    if (gold_cookie_spawned) {
      float life = gold_cookie_hide_timer;

      // pulsation
      float pulse = 1.0f + 0.08f * sinf(life * 6.0f);

      // tween spawn/despawn on start and end
      const float fade_time = 0.5f;
      float fade = 1.0f;
      if (life < fade_time) {
        fade = life / fade_time;
      }
      float time_left = GOLD_COOKIE_LIFETIME - life;
      if (time_left < fade_time) {
        fade = time_left / fade_time;
      }
      if (fade < 0.0f) {
        fade = 0.0f;
      }
      if (fade > 1.0f) {
        fade = 1.0f;
      }

      // gold shine of cookie
      DrawGoldCookie(gold_cookie_coords, pulse * 1.25f,
                     (Color){255, 200, 60, (unsigned char)(70.0f * fade)});
      DrawGoldCookie(gold_cookie_coords, pulse,
                     (Color){255, 255, 255, (unsigned char)(255.0f * fade)});
    }

    DrawSplashes();

    EndDrawing();

    if (cookie_scale < 0.6f * ui_scale) {
      cookie_scale += 0.01 * ui_scale;
    }

    shine_rotation += 0.09f;
    TRACELOG(LOG_WARNING, "ui_scale: %f", ui_scale);
  }

  CloseWindow();

  return 0;
}
