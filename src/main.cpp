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

#define BLUE_FIELD_FIRST_PIXEL 16
#define BLUE_FIELD_WIDTH 128
#define BLUE_FIELD_HEIGHT 48

#define BORDER_LINE_WIDTH 1
#define BORDER_WIDTH BLUE_FIELD_WIDTH
#define BORDER_HEIGHT BLUE_FIELD_HEIGHT

#define PLAYER_LENGTH BLUE_FIELD_HEIGHT / 4
#define INITIAL_Y_PLAYER BLUE_FIELD_HEIGHT / 4 + BLUE_FIELD_FIRST_PIXEL
#define INITIAL_X_PLAYER_1 SSD1306_LCDWIDTH / 32
#define BALL_RADIUS 2

double ball_x = SSD1306_LCDWIDTH / 2;
double ball_y = (SSD1306_LCDHEIGHT / 2) + BLUE_FIELD_FIRST_PIXEL;

int BALL_SPEED_X = 2;
int BALL_SPEED_Y = 2;

int player_position_y0 = INITIAL_Y_PLAYER;
int player_position_y1 = player_position_y0 + PLAYER_LENGTH;

int score = 0;

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

void print_score() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(SSD1306_LCDWIDTH / 2, 3);
  display.print("score: ");
  display.println(score);
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
  ball_y = (SSD1306_LCDHEIGHT / 2) + BLUE_FIELD_FIRST_PIXEL;

  player_position_y0 = INITIAL_Y_PLAYER;
  player_position_y1 = player_position_y0 + PLAYER_LENGTH;

  display.fillCircle(ball_x, ball_y, BALL_RADIUS, WHITE);
  BALL_SPEED_X = 2;
  BALL_SPEED_Y = 2;

  display.drawRect(0, BLUE_FIELD_FIRST_PIXEL, BORDER_WIDTH, BORDER_HEIGHT, WHITE);

  score = 0;
  print_score();
  display.display();
}

void ball_movement(double *ball_x, double *ball_y) {
  if (*ball_x == SSD1306_LCDWIDTH - BALL_RADIUS *2 ||
      (*ball_x == INITIAL_X_PLAYER_1 + BALL_RADIUS *2
        && *ball_y >= player_position_y0 && *ball_y <= player_position_y1)) {
          BALL_SPEED_X = BALL_SPEED_X * -1;
          if(*ball_x == INITIAL_X_PLAYER_1 + BALL_RADIUS *2) {
            score++;
          }
  } else if (*ball_x < INITIAL_X_PLAYER_1) {
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

  if (*ball_y == SSD1306_LCDHEIGHT - BALL_RADIUS *2 || *ball_y == BLUE_FIELD_FIRST_PIXEL + BALL_RADIUS *2) {
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
  if(*ball_y < BLUE_FIELD_FIRST_PIXEL + BALL_RADIUS *2){
    *ball_y = BLUE_FIELD_FIRST_PIXEL + BALL_RADIUS *2;
  }
}


void loop() {
    display.clearDisplay();
    display.drawRect(0, BLUE_FIELD_FIRST_PIXEL, BORDER_WIDTH, BORDER_HEIGHT, WHITE);
    int y = analogRead(Y_PIN);

    y = round((AXIS_Y_MID - y) / 100);

    if (player_position_y0 + y > BLUE_FIELD_FIRST_PIXEL && (player_position_y1 + y) < SSD1306_LCDHEIGHT) {
      player_position_y0 += y;
      player_position_y1 += y;
    }

    if (player_position_y0 > BLUE_FIELD_FIRST_PIXEL && player_position_y1 < SSD1306_LCDHEIGHT) {
      display.drawLine(INITIAL_X_PLAYER_1,
        player_position_y0,
        INITIAL_X_PLAYER_1,
        player_position_y1,
        WHITE
      );
    }

    ball_movement(&ball_x, &ball_y);
    print_score();
    display.fillCircle(ball_x, ball_y, BALL_RADIUS, WHITE);
    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
    display.display();
    delay(1);
}
