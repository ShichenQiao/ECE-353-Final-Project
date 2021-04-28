/*
 * buzzer.h
 *
 *      Author: Mark Xia
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include <stdint.h>
#include <stdbool.h>
#include "msp.h"
#include "ece353.h"

//*****************************************************************************
// You will need to determine the number of SMCLKs it will
// take for the following notes and thier associated frequencies.
//
// Assume that your MCU is running at 24MHz.
//*****************************************************************************
#define SPACE          0 // 0 Hz
#define NOTE_A5        27271 // 880 Hz
#define NOTE_A5S       25750 // 932 Hz
#define NOTE_B5        24290 // 988 Hz
#define NOTE_C6        22944 // 1046 Hz
#define NOTE_C6S       21641 // 1109 Hz
#define NOTE_D6        20425 // 1175 Hz
#define NOTE_D6S       19277 // 1245 Hz
#define NOTE_E6        18195 // 1319 Hz
#define NOTE_F6        17179 // 1397 Hz
#define NOTE_F6S       16216 // 1480 Hz
#define NOTE_G6        15306 // 1568 Hz
#define NOTE_G6S       14449 // 1661 Hz
#define NOTE_A6        13636 // 1760 Hz
#define NOTE_A6S       12868 // 1865 Hz
#define NOTE_B6        12145 // 1976 Hz
#define NOTE_C7        11466 // 2093 Hz
#define NOTE_C7S       10825 // 2217 Hz
#define NOTE_D7        10217 // 2349 Hz
#define NOTE_D7S       9642 // 2489 Hz
#define NOTE_E7        9101 // 2637 Hz
#define NOTE_F7        8589 // 2794 Hz
#define NOTE_F7S       8108 // 2960 Hz
#define NOTE_G7        7653 // 3136 Hz
#define NOTE_G7S       7224 // 3322 Hz

//*****************************************************************************
// DO NOT MODIFY ANYTHING BELOW
//*****************************************************************************
#define MEASURE_DURATION 12000000    // 500 milliseconds
#define MEASURE_RATIO           2    // 2/4 time
#define DELAY_AMOUNT       240000    // 10  milliseconds
#define SONG_NUM_NOTES 28

typedef enum measure_time_t {
    ONE_QUARTER,
    ONE_HALF,
    ONE_EIGTH,
    THREE_EIGTH
} measure_time_t;

typedef struct{
    uint32_t period;
    measure_time_t time;
    bool delay;
}Note_t;

//***************************************************************
//***************************************************************
void music_init(void);

//***************************************************************
//***************************************************************
bool music_play_song_shine(void);

void music_play_shot(void);

void music_play_hit(void);

void led_blink(void);

#endif /* BUZZER_H_ */
