#include "ncurses_display.h"
#include "system.h"
#include "log.h"

int main() {
  //Log log;
  System system;
  NCursesDisplay::Display(system);
}