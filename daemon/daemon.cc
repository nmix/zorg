#include "daemon.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>

Daemon::Daemon(int address)
{
  addr = address;
  started = false;
  logger = new Logger("ZORG");
}

int Daemon::get_address() 
{
  return addr;
}

void Daemon::log(std::string message) 
{
  logger->log(Logger::Level::debug, message);
}

void Daemon::start() 
{
  /* Our process ID and Session ID */
  pid_t pid, sid;
        
  /* Fork off the parent process */
  pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  /* If we got a good PID, then
     we can exit the parent process. */
  if (pid > 0) {
          exit(EXIT_SUCCESS);
  }

  /* Change the file mode mask */
  umask(0);
              
  /* Open any logs here */        
              
  /* Create a new SID for the child process */
  sid = setsid();
  if (sid < 0) {
    /* Log the failure */
    exit(EXIT_FAILURE);
  }
      
  /* Change the current working directory */
  if ((chdir("/")) < 0) {
          /* Log the failure */
          exit(EXIT_FAILURE);
  }
      
  /* Close out the standard file descriptors */
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
      
  /* Daemon-specific initialization goes here */
  
  /* The Big Loop */
  while (1) {
     /* Do some task here ... */
    loop();
     // sleep(1); /* wait 30 seconds */
  }
  exit(EXIT_SUCCESS);
}

void Daemon::stop()
{

}

void Daemon::restart()
{
  if (started) 
  {
    stop();
  }
  start();
}

void Daemon::status()
{

}
