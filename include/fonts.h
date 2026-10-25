#pragma once

#include <raylib.h>
#include <stdbool.h>

#define MAX_FONTS 1
#define MAX_SPLASHES 4

extern Font fonts[MAX_FONTS];

typedef struct {
  const char *text;
  Color color;
  Vector2 center; // центр текста по X, верх по Y
  float font_size;
  float lifetime;
  float timer;
  bool active;
} Splash;

void LoadFonts(void);

// добавить всплывающий текст (рисуется и обновляется в DrawSplashes)
void AddSplash(const char *text, Color color, Vector2 center, float font_size,
               float lifetime);

// вызывать каждый кадр после всей отрисовки
void DrawSplashes(void);
