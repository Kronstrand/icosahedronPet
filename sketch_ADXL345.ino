#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <FastLED.h>

#define NUM_LEDS 40 // 40
#define DATA_PIN D6

// Define the array of leds
CRGB leds[NUM_LEDS];

//light intensity


/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);




void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print ("Sensor: "); Serial.println(sensor.name);
  Serial.print ("Driver Ver: "); Serial.println(sensor.version);
  Serial.print ("Unique ID: "); Serial.println(sensor.sensor_id);
  Serial.print ("Max Value: "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print ("Min Value: "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print ("Resolution: "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayDataRate(void)
{
  Serial.print ("Data Rate: ");

  switch (accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print ("3200 ");
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print ("1600 ");
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print ("800 ");
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print ("400 ");
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print ("200 ");
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print ("100 ");
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print ("50 ");
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print ("25 ");
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print ("12.5 ");
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print ("6.25 ");
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print ("3.13 ");
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print ("1.56 ");
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print ("0.78 ");
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print ("0.39 ");
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print ("0.20 ");
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print ("0.10 ");
      break;
    default:
      Serial.print ("???? ");
      break;
  }
  Serial.println(" Hz");
}

void displayRange(void)
{
  Serial.print ("Range: +/- ");

  switch (accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print ("16 ");
      break;
    case ADXL345_RANGE_8_G:
      Serial.print ("8 ");
      break;
    case ADXL345_RANGE_4_G:
      Serial.print ("4 ");
      break;
    case ADXL345_RANGE_2_G:
      Serial.print ("2 ");
      break;
    default:
      Serial.print ("?? ");
      break;
  }
  Serial.println(" g");
}

void setup(void)
{

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setMaxRefreshRate(200);

  Serial.begin(115200);
  Serial.println("Accelerometer Test"); Serial.println("");

  /* Initialise the sensor */
  if (!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1);
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_16_G);
  // displaySetRange(ADXL345_RANGE_8_G);
  // displaySetRange(ADXL345_RANGE_4_G);
  // displaySetRange(ADXL345_RANGE_2_G);

  /* Display some basic information on this sensor */
  displaySensorDetails();

  /* Display additional settings (outside the scope of sensor_t) */
  displayDataRate();
  displayRange();
  Serial.println("");
}

//timer
float timer = 0;
float stateTimer = 0;
float lastMillis = 0;

//pet stats
float trust = 90; //from 0 to 100
float needy = 0; //from 0 to 100
String state = "sleeping"; //sleeping, active, angry
String lastState = "sleeping";
byte scale = 100; // 0-255


float oldAccel[] = {0,0,0};
float activities[] = {0,0,0};

byte red = 0;
byte green = 0;
byte blue = 0;

void loop(void)
{

  //timer
  timer = timer + millis() - lastMillis;
  stateTimer = stateTimer + millis() - lastMillis;
  lastMillis = millis();

  

  /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  /* Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print(" ");
    Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(" ");
    Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print(" ");*/
  //Serial.println("m/s^2 ");


  float newAccel[] = {event.acceleration.x, event.acceleration.y, event.acceleration.z};
  
  for(int i = 0; i < 3; i++)
  {
    float diff = newAccel[i] - oldAccel[i];
    if (diff < 0)
    {
      diff = diff * -1;
    }
    activities[i] = activities[i] * 0.99f + diff*1000.0f * 0.01f;
    oldAccel[i] = newAccel[i]; 
  }

  float combinedActivity = (activities[0] + activities[1] + activities[2]) / 3;

  //Serial.println(combinedActivity);

  //Serial.print(activity);
  //Serial.print(",");
  //Serial.println(diff);
  
  //white -> green
  if(combinedActivity <= 255)
  {
    red = 255 - combinedActivity;
    green = 255;
    blue = 255 - combinedActivity; 
    
  }
  //green -> yellow
  else if (combinedActivity <= 510)
  {
    red = combinedActivity - 255;
    green = 255;
    blue = 0;
  }
  //yellow -> red
  else if(combinedActivity <= 765)
  {
    red = 255;
    green = 255 - (combinedActivity - 510);
    blue = 0;
  }


  //still
  if (combinedActivity < 100)
  {
     //goes to sleep if not bored
      state = "sleeping";
     
    if(needy > 0)
    {
      trust -= 0.01f;
      /* 
      if(random(0, 3000) == 0)
      {
        needy = 0;
      }
      */
    }
    
    if(random(0, 10000) == 0)
    {
      needy = 100;
    }
  }
  // postive movement
  else if (combinedActivity < 510)
  {
    
    state = "active";
    needy -= 0.06f;
    trust += 0.01f;

  }
  //shaking
  else if (combinedActivity >= 510)
  {

    trust -= 0.02f;
    needy -= 0.06f;

  }

//Serial.println(state);
Serial.print(event.acceleration.x);
Serial.print(",");
Serial.print(combinedActivity);
Serial.print(",");
Serial.print(trust);
Serial.print(",");
Serial.println(needy);


float sinWave = (sin(timer / 2000) + 1) / 2 * 255;



for (int i = 1; i < NUM_LEDS; i++)
{
  if(random(0,needy / 2) == 0)
  {
    if (state == "sleeping")
    {
      //leds[i] = CRGB(sinWave, sinWave, sinWave);
      leds[i] = CRGB(red, green, blue);
    }
    else if (state = "active")
    {
      leds[i] = CRGB(red, green, blue);
    }  
  }
  else
  {
    if(random(0,3) == 0)
    {
      leds[i] = CRGB(255, 0, 0);
    }
    else
   {
    leds[i] = CRGB(0, 0, 0);
   }
  }
  yield();
  
}

  if (needy < 0)
  {
    needy = 0;
  }
  else if (needy > 100)
  {
    needy = 100;
  }
  
  if (trust < 0)
  {
    trust = 0;
  }
  else if (trust > 100)
  {
    trust = 100;
  }
  scale = trust * 2.55f;

  FastLED.show(scale*0.5f);
  delay(1);
}
