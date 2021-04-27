/*
 * task_buzzer.c
 *
 * Author: Mark Xia
 *
 */

#include "task_buzzer.h"
#include "main.h"

Note_t Theme[] =
{
     {NOTE_G6,ONE_QUARTER,true}, // SO
     {SPACE,ONE_QUARTER,true},
     {NOTE_G6,ONE_QUARTER,true}, // SO
     {SPACE,ONE_QUARTER,true},
     {NOTE_G6,ONE_QUARTER,true}, // SO
     {SPACE,ONE_QUARTER,true},
     {NOTE_D6S,ONE_QUARTER,true}, // #MI
     {SPACE,ONE_EIGTH, true},
     {NOTE_A6S,ONE_EIGTH,true},  // XI
     {NOTE_G6,THREE_EIGTH,true}, // SO
     {SPACE,ONE_EIGTH, true},
     {NOTE_D6S,THREE_EIGTH,true}, // MI
     {NOTE_A6S,ONE_EIGTH,true},  // XI
     {NOTE_G6,ONE_HALF,true},    // SO

     {SPACE,ONE_HALF, true},

     {NOTE_D7,ONE_QUARTER,true}, // RAI
     {SPACE,ONE_QUARTER,true},
     {NOTE_D7,ONE_QUARTER,true}, // RAI
     {SPACE,ONE_QUARTER,true},
     {NOTE_D7,ONE_QUARTER,true}, // RAI
     {SPACE,ONE_QUARTER,true},
     {NOTE_D7S,ONE_QUARTER,true}, // MI#
     {SPACE,ONE_EIGTH,true},
     {NOTE_A6S,ONE_EIGTH,true}, // XI
     {NOTE_F6S,THREE_EIGTH,true}, // FA
     {SPACE,ONE_EIGTH, true},
     {NOTE_D6S,THREE_EIGTH,true}, // RAI
     {NOTE_A6S,ONE_EIGTH,true}, // XI
     {NOTE_G6,ONE_HALF,true},    // SO
     {SPACE,ONE_HALF, true},
};

Note_t Shot[] =
{
     {NOTE_G7S,ONE_EIGTH, true},
     {NOTE_A5,ONE_EIGTH, true},
};

Note_t Hit[] =
{
     {NOTE_A5,ONE_EIGTH, true},
};

//***************************************************************
// This function returns how long an individual  notes is played
//***************************************************************
uint32_t music_get_time_delay(measure_time_t time)
{
    uint32_t time_return = 0;

    time_return  = MEASURE_DURATION * MEASURE_RATIO;

    switch(time)
    {
        case ONE_QUARTER:
        {
            time_return  = time_return / 4;
            break;
        }
        case ONE_HALF:
        {
            time_return  = time_return / 2;
            break;
        }
        case ONE_EIGTH:
        {
            time_return  = time_return / 8;
            break;
        }
        case THREE_EIGTH:
        {
            time_return = time_return * 3;
            time_return  = time_return / 8;
            break;
        }
        default:
        {
            break;
        }
    }

    return time_return - DELAY_AMOUNT;

}

// initialize the buzzer ports
void music_init(){
    P2->DIR |= BIT7;
    P2->REN |= BIT7;
    P2->OUT |= BIT7;
}
//***************************************************************************
// Plays a single note of the song based on note_index.  After
// the note is played, there is an optional delay in between
// notes.
//
// Examples
// Song[note_index].period       -- Used to determine the period of the
//                                  PWM pulse.
//
// Song[note_index].time         -- 1/4 or 1/2 time note.  Call
//                                  music_get_time_delay() to determine how
//                                  long to play the note
//
// Song[note_index].delay        -- If true, add a period of silence for
//                                  DELAY_AMOUNT
//
//                                  If false, return without delaying.
//*************************************************************************
static void music_theme(uint16_t note_index)
{
    // Set the delay time variable for later use
    uint32_t dt = music_get_time_delay(Theme[note_index].time);
    // Set the delay flag to indicate whether silence for a period or not.
    bool delay = Theme[note_index].delay;
    // Set the note for the buzzer
    ece353_MKII_Buzzer_Init(Theme[note_index].period);
    // turn on the buzzer
    ece353_MKII_Buzzer_On();
    // let the buzzer hold the note for a period of time
    ece353_T32_1_wait(dt);
    // turn off the buzzer
    ece353_MKII_Buzzer_Off();
    // if there's a silence period, silence for DELAY_AMOUNT of time.
    if(delay){
        ece353_T32_1_wait(DELAY_AMOUNT);
    }
}

static void music_shot(uint16_t note_index)
{
    // Set the delay time variable for later use
    uint32_t dt = music_get_time_delay(Shot[note_index].time);
    // Set the delay flag to indicate whether silence for a period or not.
    bool delay = Shot[note_index].delay;
    // Set the note for the buzzer
    ece353_MKII_Buzzer_Init(Shot[note_index].period);
    // turn on the buzzer
    ece353_MKII_Buzzer_On();
    // let the buzzer hold the note for a period of time
    ece353_T32_1_wait(dt);
    // turn off the buzzer
    ece353_MKII_Buzzer_Off();
    // if there's a silence period, silence for DELAY_AMOUNT of time.
    if(delay){
        ece353_T32_1_wait(DELAY_AMOUNT);
    }
}

static void music_hit(uint16_t note_index)
{
    // Set the delay time variable for later use
    uint32_t dt = music_get_time_delay(Hit[note_index].time);
    // Set the delay flag to indicate whether silence for a period or not.
    bool delay = Hit[note_index].delay;
    // Set the note for the buzzer
    ece353_MKII_Buzzer_Init(Hit[note_index].period);
    // turn on the buzzer
    ece353_MKII_Buzzer_On();
    // let the buzzer hold the note for a period of time
    ece353_T32_1_wait(dt);
    // turn off the buzzer
    ece353_MKII_Buzzer_Off();
    // if there's a silence period, silence for DELAY_AMOUNT of time.
    if(delay){
        ece353_T32_1_wait(DELAY_AMOUNT);
    }
}

//***************************************************************
// Plays the song (loop through, playing each note)
// and then returns
//***************************************************************
bool music_play_song_shine(void)
{
    // loop through the struct to play the notes as the song.
    uint32_t i;
    for(i = 0; i < 30; i++){
        if(S1_PRESSED)
        {
            S1_PRESSED = false;
            ece353_MKII_Buzzer_Off();
            return false;
        }
        if (i % 3 == 0) {
            ece353_RGB_LED(true, false, false);
            ece353_MKII_RGB_LED(true, false, false);
        }
        else if (i % 3 == 1) {
            ece353_RGB_LED(false, true, false);
            ece353_MKII_RGB_LED(false, true, false);
        }
        else if (i % 3 == 2) {
            ece353_RGB_LED(false, false, true);
            ece353_MKII_RGB_LED(false, false, true);
        }
        music_theme(i);
        if (ece353_MKII_S1()){
            ece353_RGB_LED(false, false, false);
            ece353_MKII_RGB_LED(false, false, false);
            break;
        }
    }
    return true;
}

void music_play_shot(void)
{
    // loop through the struct to play the notes as the song.
    uint32_t i;
    for(i = 0; i < 2; i++){
        if (i == 0) {
            ece353_MKII_RGB_LED(true, true, true);
        }
        else if (i == 1) {
            ece353_MKII_RGB_LED(true, true, true);
        }
        music_shot(i);
    }
    ece353_MKII_RGB_LED(false, false, false);
}

void music_play_hit(void)
{
    // loop through the struct to play the notes as the song.
    music_hit(0);
}

void led_blink(void) {
    ece353_MKII_RGB_LED(true, false, false);
    vTaskDelay(pdMS_TO_TICKS(25));
    ece353_MKII_RGB_LED(false, false, false);
    vTaskDelay(pdMS_TO_TICKS(25));
    ece353_MKII_RGB_LED(true, false, false);
    vTaskDelay(pdMS_TO_TICKS(25));
    ece353_MKII_RGB_LED(false, false, false);
    vTaskDelay(pdMS_TO_TICKS(25));
    ece353_MKII_RGB_LED(true, false, false);
    vTaskDelay(pdMS_TO_TICKS(25));
    ece353_MKII_RGB_LED(false, false, false);
}
