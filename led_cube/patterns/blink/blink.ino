#include <avr/pgmspace.h>                        // allows use of PROGMEM to store patterns in flash memory instead of SRAM      sidenote: flash have a lot more storage than SRAM

#define CUBESIZE 4
#define PLANESIZE CUBESIZE*CUBESIZE
#define PLANETIME 2000                           // time each plane is displayed in us -> 100 Hz refresh
#define TIMECONST 10                             // multiplies DisplayTime to get ms - why not =100?

const unsigned char PROGMEM PatternTable[] = {   // LED Pattern Table in PROGMEM, saves the binary representation LEDs in flash memory as an array. Last column is display time in 100ms units
  
B1111,B1111,B1111,B1111   ,B1111,B1111,B1111,B1111   ,B1111,B1111,B1111,B1111  ,B1111,B1111,B1111,B1111, 50,
B0000,B0000,B0000,B0000   ,B0000,B0000,B0000,B0000   ,B0000,B0000,B0000,B0000   ,B0000,B0000,B0000,B0000   , 50,

};

int LEDPin[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1};
int PlanePin[] = {A4, A5, A2, A3};

void setup(){                        // initialization
  int pin;                             // for-loop counter
  for (pin=0; pin<PLANESIZE; pin++)    // set up LED pins as output (active HIGH)
    pinMode( LEDPin[pin], OUTPUT );

  for (pin=0; pin<CUBESIZE; pin++)     // set up plane pins as outputs (active HIGH)
    pinMode( PlanePin[pin], OUTPUT );
  }


void loop(){                 // display pattern in table until DisplayTime is zero (then repeat)
                               // declare variables
  byte PatternBuf[PLANESIZE];  // saves current pattern from PatternTable
  byte DisplayTime;            // time*100ms to display pattern
  unsigned long EndTime;       // end while-loop
  int PatternIdx = 0;          // Initialize PatternIdx to beginning of pattern table
  int plane;                   // loop counter for cube refresh
  int patbufidx;               // indexes which byte from pattern buffer
  int ledrow, ledcol, ledpin;  // counts LEDs in refresh loop
                               
  do {                                                              // loop over entries in pattern table - while DisplayTime>0
    memcpy_P( PatternBuf, PatternTable+PatternIdx, PLANESIZE );       // read pattern from PROGMEM and save in array
    PatternIdx += PLANESIZE;                                        
    DisplayTime = pgm_read_byte_near( PatternTable + PatternIdx++ );  // read DisplayTime from PROGMEM and increment index
    EndTime = millis() + ((unsigned long) DisplayTime) * TIMECONST;   // compute EndTime from current time (ms) and DisplayTime
    while ( millis() < EndTime ) {                                      // loop while DisplayTime>0 and current time < EndTime
    patbufidx = 0;                                                    // reset index counter to beginning of buffer
    for (plane=0; plane<CUBESIZE; plane++) {                          // loop over planes
      if (plane==0)                                                   // turn previous plane off
        digitalWrite( PlanePin[CUBESIZE-1], LOW );
      else 
        digitalWrite( PlanePin[plane-1], LOW ); 
  
        ledpin = 0;                                                         // load current plane pattern data into ports
        for (ledrow=0; ledrow<CUBESIZE; ledrow++) {
         for (ledcol=0; ledcol<CUBESIZE; ledcol++) 
          digitalWrite( LEDPin[ledpin++], PatternBuf[patbufidx] & (1 << ledcol) );
         patbufidx++;
        }
       
        digitalWrite( PlanePin[plane], HIGH );                              // turn current plane on
        delayMicroseconds( PLANETIME );                                     // delay PLANETIME us
        } // for plane
      } // while <EndTime
    } while (DisplayTime > 0);                                              // read patterns until time=0 which signals end
  }
