#ifndef SERIAL_MENU_H
#define SERIAL_MENU_H

#include <Arduino.h>


#include <RTTTL.h>
extern RTTTL rtttl;

void menu_init();
void menu_update();
int menu_get_selected_tune();
void menu_print_playlist();

#endif // SERIAL_MENU_H
