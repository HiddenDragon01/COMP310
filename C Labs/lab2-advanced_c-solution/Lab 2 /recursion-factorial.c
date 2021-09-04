#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int factorial(int x) {
  if (x == 0) {
    return 1;
  } else {
    return x * factorial(x - 1);
  }
}

int main(int argc, const char* argv[]) {
  int x;
  while (true) {
    if (scanf("%d", &x) != 1) {
      printf("malformed input: only support single integer as input\n");
      return EXIT_FAILURE;
    }
    if (x < 0) {
      printf("malformed input: only non-negative integers are allowed\n");
      return EXIT_FAILURE;
    }
    int result = factorial(x);
    printf("factorial(%d) = %d\n", x, result);
    printf("continue? [y/n] ");
    // consume the remaining character from previous input
    while (getchar() != '\n');
    char c = getchar();
    
    if (!(c == 'y' || c == 'Y')) break;
    
  }
  return EXIT_SUCCESS;
}
