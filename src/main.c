#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

// GLOBAL VARIABLES //
typedef struct {
  char **websites; // array of strings
  size_t size; // size of array
} Websites;

Websites websites;

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
  timer->end_time = time(NULL) + (60 * 25);
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
     // Sleep for 30 seconds
      sleep(30);
  }
}

// MODIFY HOSTS FILE //

#define HOSTS_FILE "/etc/hosts"

void print_hosts_file()
{
  FILE *file = fopen(HOSTS_FILE, "r");
  if (!file)
  {
    printf("Error: could not open file /etc/hosts\n");
    exit(1);
  }
  char line[100];
  while (fgets(line, 100, file))
  {
    printf("%s", line);
  }
  fclose(file);
}

int add_website_to_hosts_file(const char *website)
{
  FILE *file = fopen(HOSTS_FILE, "a");
  if (!file)
  {
    printf("Error: could not open file /etc/hosts\n");
    EXIT_FAILURE;
  }
  fprintf(file, "0.0.0.0 %s\n", website);
  fclose(file);
  return EXIT_SUCCESS;
}

int remove_website_from_hosts_file(const char *website)
{
  FILE *file = fopen(HOSTS_FILE, "r");
  if (!file)
  {
    printf("Error: could not open file /etc/hosts\n");
    exit(1);
  }
  FILE *temp_file = fopen("temp.txt", "w");
  if (!temp_file)
  {
    printf("Error: could not open file temp.txt\n");
    exit(1);
  }
  char line[100];
  while (fgets(line, 100, file))
  {
    if (strstr(line, website) == NULL)
    {
      fputs(line, temp_file);
    }
  }
  fclose(file);
  fclose(temp_file);
  remove(HOSTS_FILE);
  rename("temp.txt", HOSTS_FILE);
  return 0;
}

// SIGNAL HANDLING //

void exit_gracefully(int sig)
{
  signal(SIGINT, SIG_DFL);
  printf("Exiting...\n");
  for (size_t i = 0; i < websites.size; i++) {
    remove_website_from_hosts_file(websites.websites[i]);
  }
  exit(0);
}

// PARSING WEBSITES //


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
  websites = read_lines("../websites.txt");
  signal(SIGINT, exit_gracefully);
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
      for (size_t i = 0; i < websites.size; i++) {
        add_website_to_hosts_file(websites.websites[i]);
      }
      start_timer(&timer);
    }
    for (size_t i = 0; i < websites.size; i++) {
      // remove_website_from_hosts_file(websites.websites[i]);
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
