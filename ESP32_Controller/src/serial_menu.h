#ifndef SERIAL_MENU_H
#define SERIAL_MENU_H

#include <Arduino.h>


#include <RTTTL.h>
extern RTTTL rtttl;

void menu_init();
void menu_update();
int menu_get_selected_tune();
void menu_print_playlist();

void menu_load_alarm_tune();
int menu_get_startup_tune();

#endif // SERIAL_MENU_H
