#include <stdint.h>

#include "spu_soundmodule.bin.h"

#include "soundlib/audioplayer.h"
#include "soundlib/spu_soundlib.h"

extern const uint8_t audio_click1_bin[];
extern const uint32_t audio_click1_bin_size;

extern const uint8_t audio_click2_bin[];
extern const uint32_t audio_click2_bin_size;

extern const uint8_t audio_click3_bin[];
extern const uint32_t audio_click3_bin_size;

extern const uint8_t audio_click4_bin[];
extern const uint32_t audio_click4_bin_size;

extern const uint8_t audio_buy1_bin[];
extern const uint32_t audio_buy1_bin_size;

extern const uint8_t audio_buy2_bin[];
extern const uint32_t audio_buy2_bin_size;

extern const uint8_t audio_buy3_bin[];
extern const uint32_t audio_buy3_bin_size;

extern const uint8_t audio_buy4_bin[];
extern const uint32_t audio_buy4_bin_size;

extern const uint8_t audio_click5_bin[];
extern const uint32_t audio_click5_bin_size;

extern const uint8_t audio_click6_bin[];
extern const uint32_t audio_click6_bin_size;

extern const uint8_t audio_click7_bin[];
extern const uint32_t audio_click7_bin_size;

typedef struct {
  const uint8_t *src;
  uint32_t src_size;
  short *samples;
  int size_samples;
  int freq;
  int is_stereo;
} CachedSound;

static CachedSound *decodeSound(const uint8_t *sound, uint32_t sound_size);
int PlaySoundMy(const uint8_t *sound, uint32_t sound_size);
void PlayRandomBuySound(void);
void PlayRandomClickSound(void);
