#include "AnaRun2.h"
#include <iostream>

int main() {

  AnaRun2 * myAnaRun2 = new AnaRun2();

  myAnaRun2->Loop();
  myAnaRun2->PSD(100); //enter -1 to analyze all events

  return 0;

}
