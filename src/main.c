#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

// POMODORO TIMER //

typedef struct
{
  time_t end_time;
  int is_running;
} Timer;

void init_timer(Timer *timer)
{
  timer->end_time = 0;
  timer->is_running = 0;
}

void stop_timer(Timer *timer)
{
  timer->end_time = 0;
  timer->is_running = 0;
}

void start_timer(Timer *timer)
{
  timer->end_time = time(NULL) + (60);
  timer->is_running = 1;

  while (timer->is_running)
  {
    time_t current_time = time(NULL);
    time_t remaining_time = timer->end_time - current_time;
    printf("Remaining time: %ld\n", remaining_time);
    if (current_time >= timer->end_time)
    {
      stop_timer(timer);
     }
     // Sleep for 2 second
      sleep(2);
  }
}

// PARSING WEBSITES //

typedef struct {
  char **websites; // array of strings
  size_t size; // size of array
} Websites;

#define MAX_LINE_LENGTH 100
#define INITIAL_SIZE 10

Websites read_lines(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("Error: could not open file %s\n", filename);
    exit(1);
  }
  size_t size = INITIAL_SIZE;
  char **websites = malloc(size * sizeof(char *));
  if (!websites) {
    printf("Error: could not allocate memory\n");
    exit(1);
  }

  char line[MAX_LINE_LENGTH];
  size_t i = 0;
  while (fgets(line, MAX_LINE_LENGTH, file)) {
    websites[i++] = strdup(line);
  }
  Websites result = { websites, i };
  fclose(file);
  return result;
}

int main(int argc, char **argv)
{
  Websites websites = read_lines("../websites.txt");
  if (websites.size == 0) {
    printf("Error: no websites found\n");
    exit(1);
  }
  for (size_t i = 0; i < websites.size; i++) {
    printf("%ld: ", i);
    printf("%s", websites.websites[i]);
  }
  if (argc > 1)
  {
    // Get argument
    char *arg = argv[1];

    // Initialize timer
    Timer timer;
    init_timer(&timer);

    // Start timer
    if (arg[0] == 's')
    {
      start_timer(&timer);
    }
    printf("Timer stopped!\n");
  }
  else
  {
    // Warn user to provide arguments
    printf("Please provide an argument!\n");
  }
  return 0;
}
