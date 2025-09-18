
#ifndef _OLED_H
#define _OLED_H

#include <PololuOLED.h>


/* 
 *  This class is a wrapper written by Paul O'Dowd
 *  to expose the OLED functionality and provide a
 *  count down timer of 4 minutes.
 *  
 *  To use this, please follow the instructions in
 *  Supplementary labsheet 4.
 */
class OLED_c: public PololuSH1106 {

  public:
    OLED_c( uint8_t clk, uint8_t mos, uint8_t res, uint8_t dc, uint8_t cs ): PololuSH1106(clk, mos, res, dc, cs) {

    }

    void startStopwatch() {
      end_ts = millis() + max_ms;
      display_ts = millis();
    }

    void setMaxMinutes( unsigned long minutes ) {
      max_ms = minutes * 60 * 1000;// convert to ms
    }

    void reset() {
      disableUSB();
      this->clear();
      enableUSB();
    }

    bool timeRemaining() {
      unsigned long now;
      now = millis();

      if (now - display_ts > 1000 ) {
        display_ts = millis();

        //disableUSB();
        this->clear();

        if ( now < end_ts ) {
          unsigned long dt = end_ts - now;
          dt /= 1000;

          this->gotoXY(0, 1);
          this->print(dt);
		  //enableUSB();
		  return true;
        } else {
          this->gotoXY(0, 1);
          this->print("- Done -");
		  //enableUSB();
		  return false;
        }
      }
    }
  private:

    // Some variables to track time.
    unsigned long end_ts;
    unsigned long display_ts;
    unsigned long max_ms = 120000;

    // Two helper functions, disableUSB() and enableUSB().
    // Adapted from:
    // https://github.com/pololu/usb-pause-arduino/blob/master/USBPause.h
    // Accessed 25/09/24.
    uint8_t savedUDIEN;
    uint8_t savedUENUM;
    uint8_t savedUEIENX0;
    void disableUSB() {
      // Disable the general USB interrupt.  This must be done
      // first, because the general USB interrupt might change the
      // state of the EP0 interrupt, but not the other way around.
      savedUDIEN = UDIEN;
      UDIEN = 0;

      // Select endpoint 0.
      savedUENUM = UENUM;
      UENUM = 0;

      // Disable endpoint 0 interrupts.
      savedUEIENX0 = UEIENX;
      UEIENX = 0;
    }
    void enableUSB() {
      // Restore endpoint 0 interrupts.
      UENUM = 0;
      UEIENX = savedUEIENX0;

      // Restore endpoint selection.
      UENUM = savedUENUM;

      // Restore general device interrupt.
      UDIEN = savedUDIEN;
    }
};

#endif
