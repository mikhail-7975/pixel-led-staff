#include "MPU6050.h"                 // Подключение библиотеки MPU6050 
#include "Wire.h"                    // Подключение библиотеки WireCdev
#include "FastLED.h"

#include "Circle.h"

#define PIN 5
#define NUM_LEDS 20
#define IMG_SIZE NUM_LEDS
#define N_CALIBRATION_CYCLES 100

CRGB leds_[3] = {CRGB(0, 0, 0), CRGB(0, 0, 0), CRGB(0, 0, 0)};
 
CRGB leds[NUM_LEDS];
MPU6050 CY531;                       // Создаем объект, символизирующий модуль датчика
int16_t ax, ay, az;                  // Переменные для хранения значений акселерометра
int16_t gx, gy, gz;                  // Переменные для хранения значений гироскоп

float gx_grad, gy_grad, gz_grad;

uint8_t red, green, blue;
int32_t x; 
uint8_t set_x_zero_counter = 0;

//uint8_t img_data[15552] = DOCTOR_DIY;
uint8_t img_data[1200] = CIRCLE;// = DOCTOR_DIY;

void getRGB(const uint8_t* img_arr, uint32_t x, uint32_t y, 
            uint8_t* red, uint8_t* green, uint8_t* blue) {
*red = img_arr[3 * x + 3 * y * IMG_SIZE];
*green = img_arr[3 * x + 3 * y * IMG_SIZE + 1];
*blue = img_arr[3 * x + 3 * y * IMG_SIZE + 2];
// *red = img_arr[x + y * IMG_SIZE];
// *green = img_arr[x + y * IMG_SIZE + IMG_SIZE * IMG_SIZE];
// *blue = img_arr[x + y * IMG_SIZE + 2 * IMG_SIZE * IMG_SIZE];
}
 
void setup()
{
FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
FastLED.setBrightness(50);  
Serial.begin(115200);
Wire.begin(); 
CY531.initialize();
CY531.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
CY531.CalibrateGyro();// Инициализация MPU
leds[38] = CHSV(0, 255, 255);
FastLED.show(); 
delay(100);                           // Пауза
}

void loop()
{
  CY531.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); // Чтение значений гироскопа и акселерометра
    
  for (int y = 0; y < NUM_LEDS; y++ ) {         // от 0 до первой трети
    getRGB(img_data, (x * 500 / 32767) % 20, y, &red, &green, &blue);
    leds[y] = CRGB(red, green, blue);
  }
  FastLED.show(); 
  // вывод значений в монитор
  
  //Serial.println(abs(ax) + abs(ay) + abs(az));// Serial.print("\t");
  //Serial.print(ax - init_ax);Serial.print("\t");
  //Serial.print(ay - init_ay); Serial.print("\t");
  //Serial.println(az - init_az);
//  Serial.print(gx_grad); Serial.print("\t");
//  Serial.print(gy_grad); Serial.print("\t");
  Serial.println((x * 500 / 32767) % 20);
  delay(1);
  if(abs(gx) > 10) {
    x += abs(gx) / 100;
    set_x_zero_counter = 0;
  } else {
    set_x_zero_counter++; 
  }
  if (set_x_zero_counter > 50) {
    x = 0;
  }
}
