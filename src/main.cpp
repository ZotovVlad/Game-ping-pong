#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define OLED_RESET LED_BUILTIN
#define X_PIN PC1
#define Y_PIN PC0
#define BUTTON PB1
#define AXIS_Y_MIN 0
#define AXIS_Y_MID 350
#define AXIS_Y_MAX 700

#define PLAYER_LENGTH SSD1306_LCDHEIGHT / 4
#define INITIAL_Y_PLAYER SSD1306_LCDHEIGHT / 4
#define INITIAL_X_PLAYER_1 SSD1306_LCDWIDTH / 32

int y0, y1;

Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  Serial.begin(9600);

  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(BUTTON, INPUT);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // init done

  // Clear the buffer.
  display.clearDisplay();

  display.drawLine(INITIAL_X_PLAYER_1,
                    INITIAL_Y_PLAYER,
                    INITIAL_X_PLAYER_1,
                    INITIAL_Y_PLAYER + PLAYER_LENGTH,
                    WHITE
                  );
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
}

void loop() {
    display.clearDisplay();
    int y = analogRead(Y_PIN);

    y = round((AXIS_Y_MID - y) / 100);
    if ((y0 + y + INITIAL_Y_PLAYER) > 0 && (y1 + INITIAL_Y_PLAYER + PLAYER_LENGTH + y) < SSD1306_LCDHEIGHT) {
      y0 += y;
      y1 += y;
    }

    if (INITIAL_Y_PLAYER + y0 >= 0 && INITIAL_Y_PLAYER + PLAYER_LENGTH + y1 <= SSD1306_LCDHEIGHT) {
      display.drawLine(INITIAL_X_PLAYER_1,
        INITIAL_Y_PLAYER + y0,
        INITIAL_X_PLAYER_1,
        INITIAL_Y_PLAYER + PLAYER_LENGTH + y1,
        WHITE
      );
      display.display();
    }
    delay(200);
}
