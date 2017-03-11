#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>

#include "DRS4.h"

int main() {
  DRS4* DataSet1 = new DRS4("07-13-16-drs4run5ch1.dat");
  DataSet1->Decode(300000);

  return 0;
}
