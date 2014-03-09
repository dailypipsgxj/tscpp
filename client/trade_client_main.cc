#include <string.h>

#include "fix_application_impl.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "usage: %s config-file SYMBOL\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FIXApplicationImpl fix_app;
  fix_app.Init(argv[1]);

  while (true) {
    if (fix_app.IsLogged()) {
      fix_app.RequestMarketData(argv[2]);
      break;
    }
  }

  // Quickfix is now running in the background, so we just wait here until the
  // user hits ENTER to shut us down.
  fprintf(stdout, "Hit [ENTER] to quit...\n");
  char s[100];
  fgets(s, sizeof(s), stdin);

  return 0;
}
