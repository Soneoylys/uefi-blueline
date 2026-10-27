// Host test of BluelineSpmiKeys.h against a simulated version 5 arbiter (as sheng's Test/keys_host.c for its v7 header).
//   zig cc -o keys_host.exe keys_host.c && keys_host.exe
#include <stdio.h>
#include <string.h>
#include "../BluelineSpmiKeys.h"

static uint32_t core[0x1100 / 4];
static uint32_t obs[(0x10000 + 0x80 * 64) / 4];
static int failures;

#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); failures++; } } while (0)

// A channel answers like hardware once its command is written: done, with one data byte.
static void answer(int apid, uint8_t value) {
  uint32_t *ch = obs + (0x80 * apid) / 4;
  ch[BK_STATUS / 4] = BK_STATUS_DONE;
  ch[BK_RDATA0 / 4] = value;
}

int main(void) {
  struct bk_arbiter a = { core, obs, 0 };
  struct bk_keys k;
  uint8_t pressed;

  core[0] = 0x70000000;   // a v7 arbiter must be refused
  CHECK(bk_open(&a, &k) == 1);

  core[0] = 0x50020000;
  core[BK_FEATURES / 4] = 0x7ff;   // more APIDs than the window holds: the lookup must stay inside it
  core[(0x900 + 4 * 3) / 4] = (0x008u << 8) | 1;    // PON at APID 3
  core[(0x900 + 4 * 40) / 4] = (0x0c5u << 8) | 1;   // GPIO6 at APID 40
  CHECK(bk_open(&a, &k) == 0);
  CHECK(k.pon == 3 && k.gpio == 40);

  // Nothing pressed: PON bits clear, GPIO6 high (released, active low).
  answer(3, 0x00); answer(40, 0x01);
  CHECK(bk_sample(&a, &k, &pressed) == 0 && pressed == 0);
  CHECK(obs[(0x80 * 3) / 4] == ((1u << 27) | (0x10u << 4)));   // extended read of RT_STS on APID 3's channel

  answer(3, 0x01); answer(40, 0x01);
  CHECK(bk_sample(&a, &k, &pressed) == 0 && pressed == BK_KEY_POWER);
  answer(3, 0x02); answer(40, 0x01);
  CHECK(bk_sample(&a, &k, &pressed) == 0 && pressed == BK_KEY_VOLDOWN);
  answer(3, 0x00); answer(40, 0x00);
  CHECK(bk_sample(&a, &k, &pressed) == 0 && pressed == BK_KEY_VOLUP);

  // An arbiter error reports nothing pressed.
  answer(3, 0x01); obs[(0x80 * 3) / 4 + BK_STATUS / 4] = BK_STATUS_DONE | 0x2;
  CHECK(bk_sample(&a, &k, &pressed) == 2 && pressed == 0);

  // EE stride: EE 1 uses the channel 0x10000 bytes further on.
  struct bk_arbiter a1 = { core, obs, 1 };
  uint32_t *ch1 = obs + (0x10000 + 0x80 * 3) / 4;
  ch1[BK_STATUS / 4] = BK_STATUS_DONE; ch1[BK_RDATA0 / 4] = 0x01;
  uint8_t v = 0;
  CHECK(bk_read8(&a1, 3, BK_REG_RT_STS, &v) == 0 && v == 0x01);

  // Debounce: one sample is not a press, two are; a held volume key repeats.
  struct bk_filter f; memset(&f, 0, sizeof f);
  CHECK(bk_step(&f, BK_KEY_VOLUP, 3, 2) == 0);
  CHECK(bk_step(&f, BK_KEY_VOLUP, 3, 2) == BK_KEY_VOLUP);
  CHECK(bk_step(&f, BK_KEY_VOLUP, 3, 2) == 0);
  CHECK(bk_step(&f, BK_KEY_VOLUP, 3, 2) == 0);
  CHECK(bk_step(&f, BK_KEY_VOLUP, 3, 2) == BK_KEY_VOLUP);

  printf(failures ? "%d failure(s)\n" : "keys_host: all checks passed\n", failures);
  return failures != 0;
}
