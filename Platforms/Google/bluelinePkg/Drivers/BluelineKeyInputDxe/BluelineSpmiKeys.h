/** @file
  Hardware keys of the Google Pixel 3 (blueline, SDM845 + PM8998) through the SPMI PMIC arbiter, as pure register-pointer code.

  The structure is sheng's ShengSpmiKeys.h (shengPkg ShengKeyInputDxe). The facts differ, each from the stock Pixel 3 device tree
  (boot.img of SP1A.210812.016.C2, dtb 0 and dtbo overlay 8) and mainline Linux (drivers/spmi/spmi-pmic-arb.c,
  drivers/input/misc/pm8941-pwrkey.c, drivers/pinctrl/qcom/pinctrl-spmi-gpio.c):
    arbiter     qcom,spmi-pmic-arb: core 0xc440000 (0x1100 bytes), obsrvr 0xe600000, qcom,ee = 0. Version 5:
                the APID map is at core + 0x900 + 4 * apid, an APID's observer channel at obsrvr + 0x10000 * ee + 0x80 * apid
    power key   PM8998 (SID 0) PON at 0x800 (qcom,power-on@800, pon_1: pon-type 0, linux,code 116), PON_RT_STS at +0x10,
                KPDPWR_N_SET = bit 0 (PM8998 is not a gen3 PON), set = pressed
    volume down same register, pon_2: pon-type 1 (RESIN), linux,code 114, RESIN_N_SET = bit 1
    volume up   gpio-keys vol_up: pm8998_gpios 6, flags 1 (active low), linux,code 115; PMIC GPIO6 is peripheral 0xc5
                (0xc000 + 5 * 0x100), its RT_STS at +0x10 bit 0 is the input level

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef BLUELINE_SPMI_KEYS_H_
#define BLUELINE_SPMI_KEYS_H_

#include <stdint.h>

enum {
  BK_VERSION_V5_MIN = 0x50000000u, BK_VERSION_V7_MIN = 0x70000000u,
  BK_FEATURES = 0x04, BK_FEATURES_APIDS_MASK = 0x7ffu,
  BK_APID_MAP = 0x900, BK_CORE_SIZE = 0x1100,
  BK_OBS_EE_STRIDE = 0x10000, BK_OBS_APID_STRIDE = 0x80,
  BK_CMD = 0x00, BK_STATUS = 0x08, BK_RDATA0 = 0x18,
  BK_OP_EXT_READL = 1,
  BK_STATUS_DONE = 1u << 0, BK_STATUS_ERRORS = 0xeu,
  BK_POLLS = 100000,
  BK_PPID_PON = 0x008, BK_PPID_GPIO6 = 0x0c5, BK_REG_RT_STS = 0x10,
  BK_PON_KPDPWR = 1u << 0, BK_PON_RESIN = 1u << 1,
  BK_KEY_POWER = 1u << 0, BK_KEY_VOLDOWN = 1u << 1, BK_KEY_VOLUP = 1u << 2
};

struct bk_arbiter {
  volatile uint32_t *core;   // BK_CORE_SIZE bytes
  volatile uint32_t *obs;    // observer channels (EE 0 starts at the base)
  uint8_t ee;
};

// 0 and *apid on success. 1 not a version 5 arbiter, 2 no APIDs, 3 PPID not mapped.
static inline int bk_find_apid (const struct bk_arbiter *a, uint32_t ppid, int *apid)
{
  uint32_t version = a->core[0], apids = a->core[BK_FEATURES / 4] & BK_FEATURES_APIDS_MASK;
  *apid = -1;
  if (version < BK_VERSION_V5_MIN || version >= BK_VERSION_V7_MIN) return 1;
  if (apids > (BK_CORE_SIZE - BK_APID_MAP) / 4) apids = (BK_CORE_SIZE - BK_APID_MAP) / 4;   // stay inside the core window
  if (!apids) return 2;
  for (uint32_t i = 0; i < apids; ++i) {
    uint32_t map = a->core[(BK_APID_MAP + 4 * i) / 4];
    if (map && ((map >> 8) & 0xfff) == ppid) { *apid = (int) i; break; }
  }
  return *apid < 0 ? 3 : 0;
}

// One register byte through the observer channel. 0 ok, 1 timeout, 2 the arbiter reported an error.
static inline int bk_read8 (const struct bk_arbiter *a, int apid, uint8_t reg, uint8_t *value)
{
  volatile uint32_t *ch = a->obs + (BK_OBS_EE_STRIDE * a->ee + BK_OBS_APID_STRIDE * (uint32_t) apid) / 4;
  ch[BK_CMD / 4] = ((uint32_t) BK_OP_EXT_READL << 27) | ((uint32_t) reg << 4);
  for (unsigned n = 0; n < BK_POLLS; ++n) {
    uint32_t s = ch[BK_STATUS / 4];
    if (s & BK_STATUS_DONE) {
      if (s & BK_STATUS_ERRORS) return 2;
      *value = (uint8_t) ch[BK_RDATA0 / 4];
      return 0;
    }
  }
  return 1;
}

struct bk_keys { int pon, gpio; };   // APIDs, -1 = not available

static inline int bk_open (const struct bk_arbiter *a, struct bk_keys *k)
{
  int rc = bk_find_apid (a, BK_PPID_PON, &k->pon);
  if (rc) return rc;
  if (bk_find_apid (a, BK_PPID_GPIO6, &k->gpio)) k->gpio = -1;   // power and volume down still work without it
  return 0;
}

// Pressed keys as BK_KEY_* bits. 0 ok; non-zero when a read failed (nothing is reported pressed then).
static inline int bk_sample (const struct bk_arbiter *a, const struct bk_keys *k, uint8_t *pressed)
{
  uint8_t v = 0;
  *pressed = 0;
  int rc = bk_read8 (a, k->pon, BK_REG_RT_STS, &v);
  if (rc) return rc;
  if (v & BK_PON_KPDPWR) *pressed |= BK_KEY_POWER;
  if (v & BK_PON_RESIN) *pressed |= BK_KEY_VOLDOWN;
  if (k->gpio >= 0) {
    rc = bk_read8 (a, k->gpio, BK_REG_RT_STS, &v);
    if (rc) { *pressed = 0; return rc; }
    if (!(v & 1)) *pressed |= BK_KEY_VOLUP;       // active low
  }
  return 0;
}

// Debounce and auto-repeat (sheng's sk_step). A key counts as pressed after two identical samples. Volume keys repeat while held:
// first after `delay` polls, then every `period` polls. Returns the keys to report now (new presses plus repeats).
struct bk_filter { uint8_t last, stable; uint16_t held[3]; };

static inline uint8_t bk_step (struct bk_filter *f, uint8_t sample, uint16_t delay, uint16_t period)
{
  uint8_t out = 0;
  if (sample == f->last) {
    uint8_t fresh = (uint8_t) (sample & ~f->stable);
    f->stable = sample;
    out = fresh;
    for (unsigned i = 0; i < 3; ++i) {
      uint8_t bit = (uint8_t) (1u << i);
      if (!(sample & bit)) { f->held[i] = 0; continue; }
      if (fresh & bit) { f->held[i] = 0; continue; }
      ++f->held[i];
      if (bit != BK_KEY_POWER && f->held[i] >= delay && (f->held[i] - delay) % period == 0) out |= bit;
    }
  }
  f->last = sample;
  return out;
}

#endif
