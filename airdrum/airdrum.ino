#include <Seeed_Arduino_SSCMA.h>
#include <WiFi.h>

// ===========================
// Enter your WiFi credentials
// ===========================
const char* ssid     = "Ken phone";
const char* password = "aaaaaaaa";

SSCMA AI;

// Drum regions defined:

// Top Left 
#define TOP_LEFT_X_MIN 300
#define TOP_LEFT_X_MAX 480
#define TOP_LEFT_Y_MIN 200
#define TOP_LEFT_Y_MAX 330

// Top Right
#define TOP_RIGHT_X_MIN 220
#define TOP_RIGHT_X_MAX 330
#define TOP_RIGHT_Y_MIN 120
#define TOP_RIGHT_Y_MAX 300

// Bottom Left
#define BOTTOM_LEFT_X_MIN 150
#define BOTTOM_LEFT_X_MAX 200
#define BOTTOM_LEFT_Y_MIN 200
#define BOTTOM_LEFT_Y_MAX 330

// Bottom Right
#define BOTTOM_RIGHT_X_MIN 0
#define BOTTOM_RIGHT_X_MAX 150
#define BOTTOM_RIGHT_Y_MIN 180
#define BOTTOM_RIGHT_Y_MAX 230

void setup()
{
    Serial.begin(9600);
    AI.begin();

    // Need to set up PinMode for sound output
}

void loop()
{
    if (!AI.invoke())
    {
        
        Serial.println("invoke success");
        Serial.print("perf: prepocess=");
        Serial.print(AI.perf().prepocess);
        Serial.print(", inference=");
        Serial.print(AI.perf().inference);
        Serial.print(", postpocess=");
        Serial.println(AI.perf().postprocess);
        

        for (int i = 0; i < AI.boxes().size(); i++)
        {
            int x = AI.boxes()[i].x;
            int y = AI.boxes()[i].y;
            
            Serial.print("Box[");
            Serial.print(i);
            Serial.print("] target=");
            Serial.print(AI.boxes()[i].target);
            Serial.print(", score=");
            Serial.print(AI.boxes()[i].score);
            Serial.print(", x=");
            Serial.print(AI.boxes()[i].x);
            Serial.print(", y=");
            Serial.print(AI.boxes()[i].y);
            Serial.print(", w=");
            Serial.print(AI.boxes()[i].w);
            Serial.print(", h=");
            Serial.println(AI.boxes()[i].h);
            
            // Check if x and y are within the defined regions
            if (x >= TOP_LEFT_X_MIN && x <= TOP_LEFT_X_MAX &&
                y >= TOP_LEFT_Y_MIN && y <= TOP_LEFT_Y_MAX)
            {
                Serial.println("Top Left Region detected!");
                // play sound
            }
            else if (x >= TOP_RIGHT_X_MIN && x <= TOP_RIGHT_X_MAX &&
                     y >= TOP_RIGHT_Y_MIN && y <= TOP_RIGHT_Y_MAX)
            {
                Serial.println("Top Right Region detected!");
                // play sound
            }
            else if (x >= BOTTOM_LEFT_X_MIN && x <= BOTTOM_LEFT_X_MAX &&
                     y >= BOTTOM_LEFT_Y_MIN && y <= BOTTOM_LEFT_Y_MAX)
            {
                Serial.println("Bottom Left Region detected!");
                // play sound
            }
            else if (x >= BOTTOM_RIGHT_X_MIN && x <= BOTTOM_RIGHT_X_MAX &&
                     y >= BOTTOM_RIGHT_Y_MIN && y <= BOTTOM_RIGHT_Y_MAX)
            {
                Serial.println("Bottom Right Region detected!");
                // play sound
            }
        }
        /*
        for (int i = 0; i < AI.classes().size(); i++)
        {
            Serial.print("Class[");
            Serial.print(i);
            Serial.print("] target=");
            Serial.print(AI.classes()[i].target);
            Serial.print(", score=");
            Serial.println(AI.classes()[i].score);
        }
        for (int i = 0; i < AI.points().size(); i++)
        {
            Serial.print("Point[");
            Serial.print(i);
            Serial.print("]: target=");
            Serial.print(AI.points()[i].target);
            Serial.print(", score=");
            Serial.print(AI.points()[i].score);
            Serial.print(", x=");
            Serial.print(AI.points()[i].x);
            Serial.print(", y=");
            Serial.println(AI.points()[i].y);
        }
        for (int i = 0; i < AI.keypoints().size(); i++)
        {
            Serial.print("keypoint[");
            Serial.print(i);
            Serial.print("] target=");
            Serial.print(AI.keypoints()[i].box.target);
            Serial.print(", score=");
            Serial.print(AI.keypoints()[i].box.score);
            Serial.print(", box:[x=");
            Serial.print(AI.keypoints()[i].box.x);
            Serial.print(", y=");
            Serial.print(AI.keypoints()[i].box.y);
            Serial.print(", w=");
            Serial.print(AI.keypoints()[i].box.w);
            Serial.print(", h=");
            Serial.print(AI.keypoints()[i].box.h);
            Serial.print("], points:[");
            for (int j = 0; j < AI.keypoints()[i].points.size(); j++)
            {
                Serial.print("[");
                Serial.print(AI.keypoints()[i].points[j].x);
                Serial.print(",");
                Serial.print(AI.keypoints()[i].points[j].y);
                Serial.print("],");
            }
            Serial.println("]");
        }
        */
    }
}