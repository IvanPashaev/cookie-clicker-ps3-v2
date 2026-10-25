#include "fonts.h"
#include "textures.h"
#include <raylib.h>
#include <rlgl.h>

#include <stdint.h>
#include <stdlib.h>

Font fonts[MAX_FONTS] = {0};

static Splash splashes[MAX_SPLASHES] = {0};

extern const uint8_t Merriweather_Bold_ttf_bin[];
extern const uint32_t Merriweather_Bold_ttf_bin_size;

void LoadFonts(void) {
  fonts[0] = LoadFontFromMemory(".ttf", Merriweather_Bold_ttf_bin,
                                Merriweather_Bold_ttf_bin_size, 128, NULL, 0);
}

void AddSplash(const char *text, Color color, Vector2 center, float font_size,
               float lifetime) {
  for (int i = 0; i < MAX_SPLASHES; i++) {
    if (!splashes[i].active) {
      splashes[i] =
          (Splash){text, color, center, font_size, lifetime, 0.0f, true};
      return;
    }
  }

  TRACELOG(LOG_WARNING, "No free splash slot");
}

void DrawSplashes(void) {
  float delta_time = GetFrameTime();

  for (int i = 0; i < MAX_SPLASHES; i++) {
    Splash *splash = &splashes[i];
    if (!splash->active) {
      continue;
    }

    splash->timer += delta_time;
    if (splash->timer >= splash->lifetime) {
      splash->active = false;
      continue;
    }

    float fade = 1.0f - splash->timer / splash->lifetime;
    if (fade < 0.0f) {
      fade = 0.0f;
    }

    Vector2 text_size =
        MeasureTextEx(fonts[0], splash->text, splash->font_size, 0.0f);
    Vector2 pos = {splash->center.x - text_size.x * 0.5f, splash->center.y};

    DrawTextEx(fonts[0], splash->text, pos, splash->font_size, 0.0f,
               Fade(splash->color, fade));
  }
}
