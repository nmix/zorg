#include <unistd.h>

#include "master_node.h"

int main(int, char*[])
{
  MasterNode node;
  node.send_to(101, "Hello world");
  sleep(1);
  exit(0);
}