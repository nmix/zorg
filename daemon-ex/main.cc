#include "exdaemon.h"

int main(int argc, char* argv[])
{
  ExDaemon d(3);
  d.start();
}