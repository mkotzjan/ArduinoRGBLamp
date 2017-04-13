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

// Predefined and saved colors
const int predefined = 6;
int predefined_index = -1;
int predefined_colors[predefined * 3];

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
bool xyz_called_old = false;

// last values r, g, b
double r, g, b;

double delta = 0.1;

int pin_r = 10;
int pin_g = 9;
int pin_b = 11;

// Current mode
mode current_mode;

void setup() {
  Serial.begin(9600);
  Serial.println("Hello world!"); 
  ms_first_xyz = 0;
  // White
  predefined_colors[0] = 255;
  predefined_colors[1] = 255;
  predefined_colors[2] = 255;
  // Red
  predefined_colors[3] = 255;
  predefined_colors[4] = 0;
  predefined_colors[5] = 0;
  // Green
  predefined_colors[6] = 0;
  predefined_colors[7] = 255;
  predefined_colors[8] = 0;
  // Blue
  predefined_colors[9] = 0;
  predefined_colors[10] = 0;
  predefined_colors[11] = 255;
  // rgb(155, 89, 182)
  predefined_colors[12] = 155;
  predefined_colors[13] = 89;
  predefined_colors[14] = 182;
  // rgb(243, 156, 18)
  predefined_colors[15] = 243;
  predefined_colors[16] = 156;
  predefined_colors[17] = 18;
  load();
  Skywriter.begin(12, 13);
  Skywriter.onTouch(touch);
  Skywriter.onAirwheel(airwheel);
  Skywriter.onGesture(gesture);
  Skywriter.onXYZ(xyz);
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);
}

void loop() {
  delay(100);
  // Poll new data
  // Serial.println("Loop");
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
    sprintf(buf, "%05u:%05u:%05u gest:%02u touch:%02u", x, y, z, Skywriter.last_gesture, Skywriter.last_touch);
    Serial.println(buf);

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
    //Serial.println(String(r) + " " + String(g) + " " + String(b));
    analogWrite(pin_r, r);
    analogWrite(pin_g, g);
    analogWrite(pin_b, b);
  }
}

void set_predefined(int index){
  Serial.println(predefined_colors[index*3]);
  Serial.println(predefined_colors[(index*3)+1]);
  Serial.println(predefined_colors[(index*3)+2]);
  analogWrite(pin_r, predefined_colors[index*3]);
  analogWrite(pin_g, predefined_colors[(index*3)+1]);
  analogWrite(pin_b, predefined_colors[(index*3)+2]);
}

void gesture(unsigned char type){
  //Serial.println("Got gesture ");
  //Serial.print(type,DEC);
  //Serial.print('\n');
  switch(int(type)) {
    case 2:
      Serial.println("Got 2");
      current_mode = defined_color;
      predefined_index = ((predefined_index+1) % predefined);
      Serial.println(predefined_index);
      set_predefined(predefined_index);
      save();
      break;
    case 3:
      Serial.println("Got 3");
      current_mode = defined_color;
      predefined_index -= 1;
      if (predefined_index < 0) predefined_index = predefined -1;
      Serial.println(predefined_index);
      set_predefined(predefined_index);
      save();
      break;
    }
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
    Serial.println("xyz");
    // If millis() was not called before
    if (ms == 0)
    {
      ms = millis();
    } else if (ms + trigger <= millis())
    {
      blink_twice();
      current_mode = xyz_color;
    }
  } else if (!xyz_called)
  {
    // Call save if changing is over
    if (ms != 0) save();
    ms = 0;
    current_mode = wait;
  }
  // reset xyz_called
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
  for (int i = 4; i <= (predefined * 3) + 3; ++i)
  {
    Serial.println("Result at " +String(i) + ": " + String(EEPROM.read(i)));
    predefined_colors[i - 4];
    Serial.println("Array at "+String(i)+": " +String(predefined_colors[i-4]));
  }
}

// Save actual state and values to EEPROM
void save()
{
  Serial.println("SAVE()");
  // Save Values only if they changed
  if (int(EEPROM.read(0)) != current_mode)
  {
    EEPROM.write(0, current_mode);
    Serial.println("Wrote 0");
  }
  if (int(EEPROM.read(1)) != r)
  {
    EEPROM.write(1, r);
    Serial.println("Wrote 1");
  }
  if (int(EEPROM.read(2)) != g)
  {
    EEPROM.write(2, g);
    Serial.println("Wrote 2");
  }
  if (int(EEPROM.read(3)) != b)
  {
    EEPROM.write(3, b);
    Serial.println("Wrote 3");
  }
  
  for(int i = 4; i <= (predefined * 3) + 3; ++i)
  {
    EEPROM.write(i, predefined_colors[i-4]);
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

void blink_twice() {
  for (int i = 0; i < 4; ++i) {
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;
    analogWrite(pin_r, r);
    analogWrite(pin_g, g);
    analogWrite(pin_b, b);
    if (i != 3) delay(80);
  }
}
