#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <gba_timers.h>
#include <cstdio>

#define GBA_TIMER_FREQ 16777216  // Define the GBA timer frequency here

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main() {
    irqInit();
    irqEnable(IRQ_VBLANK);

    s8 x_position = 10;
    s8 y_position = 10;

    const s8 X_MAX = 29;
    const s8 X_MIN = 0;
    const s8 Y_MAX = 19;
    const s8 Y_MIN = 0;

    consoleInit(0, 4, 0, NULL, 1, 15);

    BG_COLORS[0] = RGB8(58, 110, 165);
    BG_COLORS[241] = RGB5(31, 31, 31);

    SetMode(MODE_0 | BG0_ON);

    // Initialize the timer
    REG_TM0CNT_H = TIMER_START | TIMER_IRQ | TIMER_COUNT;

    u16 prevTimerValue = 0;

    while (true) {
        scanKeys();
        u16 keyChar = keysDown();

        switch (keyChar) {
            case KEY_UP:
                if (y_position-- <= Y_MIN) {
                    y_position = Y_MAX;
                }
                break;

            case KEY_DOWN:
                if (y_position++ >= Y_MAX) {
                    y_position = Y_MIN;
                }
                break;

            case KEY_LEFT:
                if (x_position-- <= X_MIN) {
                    x_position = X_MAX;
                }
                break;

            case KEY_RIGHT:
                if (x_position++ >= X_MAX) {
                    x_position = X_MIN;
                }
                break;
        }

        // Calculate delta time
        u16 timerValue = REG_TM0CNT_L;
        u16 delta = timerValue - prevTimerValue;
        float deltaTime = delta / (float)GBA_TIMER_FREQ;  // Convert delta to seconds

        // Update values every 1/60th of a second
        if (deltaTime >= 1.0f / 60.0f) {
            prevTimerValue = timerValue;  // Reset the timer value

            printf(CON_CLS());

            printf("\x1b[0;0H Line %d, Column %d", y_position, x_position);
            printf("\x1b[%d;%dH+", y_position, x_position);

            printf("\x1b[2;0H Delta Time: %f", deltaTime);

            VBlankIntrWait();
        }
    }
}
