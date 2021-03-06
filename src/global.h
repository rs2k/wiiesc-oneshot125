/**
 * Wii ESC NG 2.0 - 2013
 * Global definitions
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef global_h
#define global_h

////////////////////////////////////////////////////////////////////////////////
/// Setup GCC environment
//  Turn on/off warnings of interest.
//
// These warnings are normally suppressed by the Arduino IDE,
// but with some minor hacks it's possible to have warnings
// emitted.  This helps greatly when diagnosing subtle issues.
//
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Winline"
#pragma GCC diagnostic warning "-Wextra"
#pragma GCC diagnostic warning "-Wlogical-op"
#pragma GCC diagnostic ignored "-Wredundant-decls"

////////////////////////////////////////////////////////////////////////////////
///
//  Preprocessor constants
//
#define _NONE_         000
#define _BS_NFET_      100
#define _TP_           110
#define _TGY_          120
#define _TGY_8_        130
#define _BS_           140
#define _QYNX_         150
#define _TP_NFET_      160
#define _RB50A_        170
#define _BS40A_        180
#define _A32U4ESC_     190
#define _RCT45NFS_     200
#define _RCT30NFS_     210
#define _KDA_          220

////////////////////////////////////////////////////////////////////////////////
///
//  Unit conversions
//
#define RPM_TO_COMM_TIME(x) (1000000UL * 10U / (x) * TICKS_PER_US)
#define US_TO_TICKS(x)      ((x) * (uint8_t)TICKS_PER_US)

////////////////////////////////////////////////////////////////////////////////
///
//  Helpers
//
#define PT_EXIT_EX(pt, result)	        \
  if (1) {                              \
    __result = result;                  \
    break;                              \
  }

struct config_data {
  uint16_t rcp_min_us;
  uint16_t rcp_max_us;
  uint16_t rcp_start_us;
  uint16_t rcp_full_us;
  uint16_t rcp_cal_us;
  uint8_t  rcp_deadband_us;
  uint8_t  braking;
  int8_t   timing_adv;
  uint8_t  stick_cal_dis;
  uint8_t  rev;
};
struct config_data cfg;

struct rx_data {
  union {
    uint16_t raw;
  };
  uint8_t frame_received;
  uint16_t rcp_min;
  uint16_t rcp_max;
  uint16_t rcp_start;
  uint16_t rcp_cal;
  uint16_t rcp_os125_min;
  uint16_t rcp_os125_max;
};
struct rx_data rx;

struct pwr_stage_data {
  uint8_t com_state: 3;
  uint8_t rev: 1;
  uint8_t sdm_state: 1;
  uint8_t aco: 1;
  uint8_t recovery: 1;
  uint8_t braking_enabled: 1;
};
register struct pwr_stage_data pwr_stage asm("r16");

register int16_t sdm_ref asm("r4");

struct sdm_rt_data {
  int16_t sdm_err;
  int16_t sdm_left;
  int16_t sdm_top;
  int16_t sdm_run_min;
  int16_t sdm_start_min;
  int16_t sdm_start_max;
};
struct sdm_rt_data sdm_rt;

static uint16_t last_tick;
static uint16_t est_comm_time;
static uint16_t last_comm_time;
static uint8_t __result;
static int8_t timing_adv;

// Core Function prototypes

// RX input capture
void AttachPPM();
inline void rx_ppm_callback(uint16_t time, uint8_t state);

// SysTick
inline uint16_t __systick();
inline uint16_t __interval(uint16_t i_start, uint16_t i_end);
inline uint16_t __interval(uint16_t i_start);

// Init
void Board_Init();

struct timer_small { uint16_t elapsed, interval; uint16_t last_systick;};
struct timer_big   { uint32_t elapsed, interval; uint16_t last_systick;};

// __attribute__ ((noinline))
uint8_t timer_expired(struct timer_small *t, uint16_t systick = __systick()) {
  uint16_t dt = __interval(t->last_systick, systick);
  t->last_systick = systick;
  if (t->elapsed <= dt) {
    t->elapsed = t->interval;
    return 1;
  } ;
  t->elapsed -= dt;
  return 0;
};

__attribute__ ((noinline)) uint8_t timer_expired(struct timer_big *t, uint16_t systick = __systick()) {
  uint16_t dt = __interval(t->last_systick, systick);
  t->last_systick = systick;
  if (t->elapsed <= dt) {
    t->elapsed = t->interval;
    return 1;
  } ;
  t->elapsed -= dt;
  return 0;
};

#endif

