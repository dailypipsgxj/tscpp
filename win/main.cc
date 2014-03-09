#include <stdio.h>

#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>

class FIXApplication : public FIX::Application, public FIX::MessageCracker {
};

int main(int argc, char** argv) {
  printf("Hello, Windows\n");

  return 0;
}