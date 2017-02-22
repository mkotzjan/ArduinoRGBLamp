#include <EEPROM.h>
#include <Wire.h>
#include <skywriter.h>

// Lamp mode
enum mode {
  wait,
  xyz_color,
  defined_color,
  special
};

// Max values of x, y and z
unsigned int max_x, max_y, max_z;
unsigned int min_x, min_y, min_z;

// ms since start of arduino
unsigned long ms;

// ms_first_xyz is used to save the ms at the first call of xyz(int, int, int).
// This is used to get the xyz(int, int, int) method to use new values only
// after a short amount of time (trigger).
unsigned long ms_first_xyz;
unsigned long trigger = 800;
unsigned long before = 0;

// Was xyz called the previous iteration?
bool xyz_called = false;

// last values r, g, b
double r, g, b;

double delta = 0.1;

int pin_r = 10;
int pin_g = 9;
int pin_b = 11;

// Current mode
mode current_mode;

void setup() {
  // Serial.begin(9600);
  // Serial.println("Hello world!"); 
  Skywriter.begin(12, 13);
  Skywriter.onTouch(touch);
  Skywriter.onAirwheel(airwheel);
  Skywriter.onGesture(gesture);
  Skywriter.onXYZ(xyz);
  ms_first_xyz = 0;
  //load();
  // Serial.println(r);
  // Serial.println(g);
  // Serial.println(b);
  analogWrite(pin_r, 255);
  analogWrite(pin_g, 255);
  analogWrite(pin_b, 255);
}

void loop() {
  delay(100);
  // Poll new data
  Skywriter.poll();
  check_xyz_called();
}

void xyz(unsigned int x, unsigned int y, unsigned int z){
  xyz_called = true;
  if (current_mode == xyz_color)
  {
    if (x < min_x) min_x = x;
    if (y < min_y) min_y = y;
    if (z < min_z) min_z = z;
    if (x > max_x) max_x = x;
    if (y > max_y) max_y = y;
    if (z > max_z) max_z = z;
  
    char buf[64];
    // sprintf(buf, "%05u:%05u:%05u gest:%02u touch:%02u", x, y, z, Skywriter.last_gesture, Skywriter.last_touch);
    // Serial.println(buf);

    // Calculate color delta
    byte rgb[3];
    hslToRgb(x/65535.0, 1, y/65535.0, rgb);
    int r_delta = rgb[0] - r;
    int g_delta = rgb[1] - g;
    int b_delta = rgb[2] - b;
    // Serial.println(String(rgb[0]) + " " + String(rgb[1]) + " " + String(rgb[2]));
    r = r + (r_delta * delta);
    g = g + (g_delta * delta);
    b = b + (b_delta * delta);
    // Serial.println(String(r) + " " + String(g) + " " + String(b));
    analogWrite(pin_r, r);
    analogWrite(pin_g, g);
    analogWrite(pin_b, b);
  }
}

void gesture(unsigned char type){
  // Serial.println("Got gesture ");
  // Serial.print(type,DEC);
  // Serial.print('\n');
}

void touch(unsigned char type){
  // Serial.println("Got touch ");
  // Serial.print(type,DEC);
  // Serial.print('\n');
}

void airwheel(int delta){
  // Serial.println("Got airwheel ");
  // Serial.print(delta);
  // Serial.print('\n');
}

// Check if xyz was called
void check_xyz_called()
{
  // Trigger this only from mode wait and if xyz(int, int, int) was previously called
  if (current_mode == wait && xyz_called)
  {
    // Serial.println("xyz");
    // If millis() was not called before
    if (ms == 0)
    {
      ms = millis();
    } else if (ms + trigger <= millis())
    {
      current_mode = xyz_color;
    }
  } else if (!xyz_called)
  {
    ms = 0;
    current_mode = wait;
    save();
  }
  // reset xyz_called and ms
  xyz_called = false;
}

// Load the last state and values from EEPROM
void load()
{
  // Load mode
  current_mode = (mode)int(EEPROM.read(0));

  // Load last r, g, b values
  r = int(EEPROM.read(1));
  g = int(EEPROM.read(2));
  b = int(EEPROM.read(3));
  analogWrite(pin_r, r);
  analogWrite(pin_g, g);
  analogWrite(pin_b, b);

  // Load array index of predefined colors
  // TODO
}

// Save actual state and values to EEPROM
void save()
{
  // Save Values only if they changed
  if (int(EEPROM.read(0)) != current_mode)
  {
    EEPROM.write(0, current_mode);
  }
  if (int(EEPROM.read(1)) != r)
  {
    EEPROM.write(1, r);
  }
  if (int(EEPROM.read(2)) != g)
  {
    EEPROM.write(2, g);
  }
  if (int(EEPROM.read(3)) != b)
  {
    EEPROM.write(3, b);
  }
}

void hslToRgb(double h, double s, double l, byte rgb[]) {
  double r, g, b;

  if (s == 0) {
    r = g = b = l; // achromatic
  } else {
    double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
    double p = 2 * l - q;
    r = hue2rgb(p, q, h + 1.0/3.0);
    g = hue2rgb(p, q, h);
    b = hue2rgb(p, q, h - 1.0/3.0);
  }
  rgb[0] = r * 255;
  rgb[1] = g * 255;
  rgb[2] = b * 255;
}

double hue2rgb(double p, double q, double t) {
  if(t < 0) t += 1;
  if(t > 1) t -= 1;
  if(t < 1/6.0) return p + (q - p) * 6 * t;
  if(t < 1/2.0) return q;
  if(t < 2/3.0) return p + (q - p) * (2/3.0 - t) * 6;
  return p;
}

