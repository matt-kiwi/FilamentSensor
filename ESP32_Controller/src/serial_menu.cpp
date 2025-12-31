
#include "serial_menu.h"
#include "RTTTLTunes.h"
#include <Arduino.h>

#include <Preferences.h>

extern RTTTL rtttl;

// Fix: Declare splash screen as extern so linker can find it
#include "splash_screen.h"

static int selected_tune = 0;
static int startup_tune = 0;
static bool menu_shown = false;
static bool test_playing = false;

Preferences preferences;

void menu_print_playlist() {
    Serial.println("\nAvailable Tunes:");
    const int columns = 4;
    for (int i = 0; i < rtttlTunesCount; ++i) {
        Serial.printf("%2d: %-18s", i, rtttlTunes[i].name);
        if ((i + 1) % columns == 0 || i == rtttlTunesCount - 1) {
            Serial.println();
        } else {
            Serial.print("  ");
        }
    }
}

void menu_print_status() {
    Serial.print("Current alarm tune: ");
    Serial.print(selected_tune);
    Serial.print(" - ");
    Serial.println(rtttlTunes[selected_tune].name);
    Serial.print("Startup tune: ");
    Serial.print(startup_tune);
    Serial.print(" - ");
    Serial.println(rtttlTunes[startup_tune].name);
}

void menu_init() {
    menu_print_playlist();
    menu_print_status();
    Serial.printf("\nTo select an alarm, type 'a' then the number (e.g. a15).\n");
    Serial.printf("To select the startup tune, type 's' then the number (e.g. s21).\n");
    Serial.printf("Type 'p' then number to play/test a song (e.g. p3), space to stop test.\n");
    Serial.printf("Type 'c' to show current alarm and startup tune.\n");
    menu_shown = true;
}

void menu_update() {
    static int enter_count = 0;
    while (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        bool isSpace = (input == " ");
        input.trim();
        Serial.println(input); // Echo user input
        if (input.length() == 0) {
            if (isSpace && test_playing) {
                rtttl.stop();
                test_playing = false;
                Serial.println("Stopped test playback.");
                continue;
            }
            enter_count++;
            if (enter_count >= 2) {
                Serial.println(FPSTR(textSplashScreen));
                menu_init();
                enter_count = 0;
            }
            continue;
        }
        enter_count = 0;
        // Make command parsing case-insensitive
        String cmd = input;
        cmd.toLowerCase();
        // One-step commands only
        if (cmd.startsWith("a")) {
            String num = input.substring(1);
            int idx = num.toInt();
            if (num.length() > 0 && idx >= 0 && idx < rtttlTunesCount) {
                selected_tune = idx;
                preferences.putInt("alarm_tune", selected_tune);
                Serial.print("Alarm tune selected: ");
                Serial.print(selected_tune);
                Serial.print(" - ");
                Serial.println(rtttlTunes[selected_tune].name);
            } else {
                Serial.println("Invalid tune number for alarm.");
            }
            continue;
        }
        if (cmd.startsWith("s")) {
            String num = input.substring(1);
            int idx = num.toInt();
            if (num.length() > 0 && idx >= 0 && idx < rtttlTunesCount) {
                startup_tune = idx;
                preferences.putInt("startup_tune", startup_tune);
                Serial.print("Startup tune selected: ");
                Serial.print(startup_tune);
                Serial.print(" - ");
                Serial.println(rtttlTunes[startup_tune].name);
            } else {
                Serial.println("Invalid tune number for startup.");
            }
            continue;
        }
        if (cmd.startsWith("p")) {
            String num = input.substring(1);
            int idx = num.toInt();
            if (num.length() > 0 && idx >= 0 && idx < rtttlTunesCount) {
                rtttl.loadSong(rtttlTunes[idx].song);
                rtttl.play();
                test_playing = true;
                Serial.print("Playing: ");
                Serial.println(rtttlTunes[idx].name);
            } else {
                Serial.println("Invalid tune number for test.");
            }
            continue;
        }
        if (cmd == "c") {
            menu_print_status();
            continue;
        }
    }
}

int menu_get_selected_tune() {
    return selected_tune;
}

int menu_get_startup_tune() {
    return startup_tune;
}

// Load alarm and startup tune from preferences at startup
void menu_load_alarm_tune() {
    preferences.begin("filament", false);
    selected_tune = preferences.getInt("alarm_tune", 19); // Monty Python
    startup_tune = preferences.getInt("startup_tune", 2); // Mario
}
