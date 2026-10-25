#include "audio.h"

#define MAX_CACHED_SOUNDS 16

static CachedSound sound_cache[MAX_CACHED_SOUNDS];
static int num_cached_sounds = 0;

static CachedSound *decodeSound(const uint8_t *sound, uint32_t sound_size) {
  for (int i = 0; i < num_cached_sounds; i++) {
    if (sound_cache[i].src == sound)
      return &sound_cache[i];
  }

  if (num_cached_sounds >= MAX_CACHED_SOUNDS)
    return NULL;

  int size_samples = 48000 * 2 * 2 * 2;
  short *samples = malloc(size_samples);

  if (!samples)
    return NULL;

  CachedSound *c = &sound_cache[num_cached_sounds];
  c->size_samples = size_samples;
  if (DecodeAudio((void *)sound, sound_size, samples, &c->size_samples,
                  &c->freq, &c->is_stereo) != 0) {
    free(samples);
    return NULL;
  }

  c->src = sound;
  c->src_size = sound_size;
  c->samples = samples;
  num_cached_sounds++;
  return c;
}

int PlaySoundMy(const uint8_t *sound, uint32_t sound_size) {
  CachedSound *c = decodeSound(sound, sound_size);

  if (!c)
    return -1;

  SND_Pause(0);
  SND_SetVoice(2, c->is_stereo ? VOICE_STEREO_16BIT : VOICE_MONO_16BIT, c->freq,
               0, c->samples, c->size_samples, 255, 255, NULL);
  return 0;
}

void PlayRandomBuySound(void) {
  switch (rand() % 4 + 1) {
  case 1:
    PlaySoundMy(audio_buy1_bin, audio_buy1_bin_size);
    break;
  case 2:
    PlaySoundMy(audio_buy2_bin, audio_buy2_bin_size);
    break;
  case 3:
    PlaySoundMy(audio_buy3_bin, audio_buy3_bin_size);
    break;
  case 4:
    PlaySoundMy(audio_buy4_bin, audio_buy4_bin_size);
    break;
  }
}

void PlayRandomClickSound() {
  switch (rand() % 7 + 1) {
  case 1:
    PlaySoundMy(audio_click1_bin, audio_click1_bin_size);
    break;
  case 2:
    PlaySoundMy(audio_click2_bin, audio_click2_bin_size);
    break;
  case 3:
    PlaySoundMy(audio_click3_bin, audio_click3_bin_size);
    break;
  case 4:
    PlaySoundMy(audio_click4_bin, audio_click4_bin_size);
    break;
  case 5:
    PlaySoundMy(audio_click5_bin, audio_click5_bin_size);
    break;
  case 6:
    PlaySoundMy(audio_click6_bin, audio_click6_bin_size);
    break;
  case 7:
    PlaySoundMy(audio_click7_bin, audio_click7_bin_size);
    break;
  }
}
