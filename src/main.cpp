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
#define BALL_RADIUS 2

double ball_x = SSD1306_LCDWIDTH / 2;
double ball_y = SSD1306_LCDHEIGHT / 2;
int BALL_SPEED_X = 2;
int BALL_SPEED_Y = 2;

int player_position_y0 = INITIAL_Y_PLAYER;
int player_position_y1 = player_position_y0 + PLAYER_LENGTH;


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
}

void reset_game_field() {
  display.clearDisplay();
  display.drawLine(INITIAL_X_PLAYER_1,
    INITIAL_Y_PLAYER,
    INITIAL_X_PLAYER_1,
    INITIAL_Y_PLAYER + PLAYER_LENGTH,
    WHITE
  );
  ball_x = SSD1306_LCDWIDTH / 2;
  ball_y = SSD1306_LCDHEIGHT / 2;

  player_position_y0 = INITIAL_Y_PLAYER;
  player_position_y1 = player_position_y0 + PLAYER_LENGTH;
  
  display.fillCircle(ball_x, ball_y, BALL_RADIUS, WHITE);
  BALL_SPEED_X = 2;
  BALL_SPEED_Y = 2;
  display.display();
}

void ball_movement(double *ball_x, double *ball_y) {
  if (*ball_x == SSD1306_LCDWIDTH - BALL_RADIUS *2 ||
      (*ball_x == INITIAL_X_PLAYER_1 + BALL_RADIUS *2
        && *ball_y >= player_position_y0 &&  *ball_y <= player_position_y1)) {
    BALL_SPEED_X = BALL_SPEED_X * -1;
  } else if (*ball_x < INITIAL_X_PLAYER_1){
      BALL_SPEED_X = 0;
      BALL_SPEED_Y = 0;

      delay(3000);

      display.invertDisplay(true);
      delay(300);
      display.invertDisplay(false);
      delay(300);
      display.invertDisplay(true);
      delay(300);
      display.invertDisplay(false);
      delay(300);
      display.invertDisplay(true);
      delay(300);
      display.invertDisplay(false);
      delay(300);
      display.clearDisplay();
      reset_game_field();
      // return;
  }

  if (*ball_y == SSD1306_LCDHEIGHT - BALL_RADIUS *2 || *ball_y == BALL_RADIUS *2) {
    BALL_SPEED_Y = BALL_SPEED_Y * -1;
  }

  *ball_x += BALL_SPEED_X;
  *ball_y += BALL_SPEED_Y;

  if (*ball_x > SSD1306_LCDWIDTH) {
    *ball_x = SSD1306_LCDWIDTH - BALL_RADIUS *2;
  }
  if (*ball_y > SSD1306_LCDHEIGHT) {
    *ball_y = SSD1306_LCDHEIGHT - BALL_RADIUS *2;
  }
  if(*ball_y < BALL_RADIUS *2){
    *ball_y = BALL_RADIUS *2;
  }
}


void loop() {
    display.clearDisplay();
    int y = analogRead(Y_PIN);

    y = round((AXIS_Y_MID - y) / 100);

    if (player_position_y0 + y > 0 && (player_position_y1 + y) < SSD1306_LCDHEIGHT) {
      player_position_y0 += y;
      player_position_y1 += y;
    }

    if (player_position_y0 >= 0 && player_position_y1 <= SSD1306_LCDHEIGHT) {
      display.drawLine(INITIAL_X_PLAYER_1,
        player_position_y0,
        INITIAL_X_PLAYER_1,
        player_position_y1,
        WHITE
      );
    }

    ball_movement(&ball_x, &ball_y);
    display.fillCircle(ball_x, ball_y, BALL_RADIUS, WHITE);
    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
    display.display();
    delay(1);
}
