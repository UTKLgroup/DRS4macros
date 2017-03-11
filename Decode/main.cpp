#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>

#include "DRS4.h"

int main() {
  DRS4* DataSet1 = new DRS4("example.bin");
  DataSet1->Decode(1000);

  return 0;
}
