#include <Arduino.h>

#define BUZZER_PIN 21

bool stopRequested = false;

bool waitWithStop(int duration)
{
    for (int i = 0; i < duration; i += 10)
    {
        if (
            Serial.available())
        {
            char c = Serial.read();

            if (c == '0')
            {
                stopRequested = true;
                noTone(BUZZER_PIN);
                return false;
            }
        }

        delay(10);
    }

    return true;
}

void playTone(int frequency, int duration)
{
    if (stopRequested)
        return;

    tone(BUZZER_PIN, frequency);

    if (!waitWithStop(duration))
        return;

    noTone(BUZZER_PIN);
    waitWithStop(100);
}

void toneSequence()
{
    playTone(300, 500);
    playTone(600, 500);
    playTone(1000, 500);
    playTone(1500, 500);
    playTone(2000, 500);
}

void alarmSound()
{
    for (int i = 0; i < 5 && !stopRequested; i++)
    {
        playTone(1000, 300);
        playTone(2000, 300);
    }
}

void doorbell()
{
    for (int i = 0; i < 2 && !stopRequested; i++)
    {
        playTone(1000, 300);
        playTone(1500, 600);
        waitWithStop(300);
    }
}

void shortBeeps()
{
    for (int i = 0; i < 5 && !stopRequested; i++)
    {
        playTone(2000, 100);
        waitWithStop(100);
    }
}

void melody()
{
    int notes[] = {
        523, 587, 659, 698,
        784, 880, 988, 1047
    };

    int durations[] = {
        300, 300, 300, 300,
        300, 300, 300, 600
    };

    for (int i = 0; i < 8 && !stopRequested; i++)
    {
        playTone(notes[i], durations[i]);
    }
}

void showMenu()
{
    Serial.println();
    Serial.println("===== BUZZER MENU =====");
    Serial.println("1. Rising Tone Sequence");
    Serial.println("2. Alarm");
    Serial.println("3. Doorbell");
    Serial.println("4. Short Beeps");
    Serial.println("5. Melody");
    Serial.println("0. Stop Sound");
    Serial.println("Enter your choice:");
}

void setup()
{
    Serial.begin(115200);
    pinMode(BUZZER_PIN, OUTPUT);
    noTone(BUZZER_PIN);

    Serial.println("ESP32 Buzzer Ready!");
    showMenu();
}

void loop()
{
    if (Serial.available())
    {
        char choice = Serial.read();

        // Ignore Enter and line-ending characters
        if (choice == '\n' || choice == '\r')
            return;

        if (choice == '0')
        {
            stopRequested = true;
            noTone(BUZZER_PIN);
            Serial.println("Buzzer stopped.");
            showMenu();
            return;
        }

        stopRequested = false;

        switch (choice)
        {
            case '1':
                Serial.println("Playing Rising Tone Sequence");
                toneSequence();
                break;

            case '2':
                Serial.println("Playing Alarm");
                alarmSound();
                break;

            case '3':
                Serial.println("Playing Doorbell");
                doorbell();
                break;

            case '4':
                Serial.println("Playing Short Beeps");
                shortBeeps();
                break;

            case '5':
                Serial.println("Playing Melody");
                melody();
                break;

            default:
                Serial.println("Invalid choice. Enter 0-5.");
                showMenu();
                return;
        }

        noTone(BUZZER_PIN);

        if (!stopRequested)
            Serial.println("Pattern finished.");

        showMenu();
    }
}