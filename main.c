#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libnotify/notify.h>

#include <signal.h>
#include <syslog.h>
#include <unistd.h>

#include <sys/stat.h>

// static const int MIN_THRESHOLD = 21;
// static const int MAX_THRESHOLD = 60;

static const char* MIN_STR = "21";
static const char* MAX_STR = "60";
FILE* fin;

char tmp[15];

short smaller(const char *a, const char *b){
    if(strlen(a) < strlen(b))   return 1;

    if((*a)<(*b))   return 1;   //compare 1st digit
    return a[1]<=b[1];
}

void alertLow(){
    fin = fopen("status", "r");
    fgets( tmp, 15, fin );
    fclose(fin);
    if( strcmp(tmp, "Charging\n") == 0 ){   //if IT IS CHARGING, sleep
        return sleep(1200);    //check again after 20 minutes
    }

    notify_notification_show(
        notify_notification_new("Battery is Low, Please Charge it", MIN_STR, NULL),
        0
    );
    sleep(90);
}

void alertHigh(){
    fin = fopen("status", "r");
    fgets( tmp, 15, fin );
    fclose(fin);
    if( strcmp(tmp, "Charging\n") != 0 ){   //if it's not charging then sleep
        return sleep(1200);    //check again after 20 minutes
    }

    notify_notification_show(
        notify_notification_new("Remove the Charger, Battery enough charged", MAX_STR, NULL),
        0
    );

    sleep(90);
}

void make_daemon(){
    pid_t pid = fork();

    if(pid < 0) //Couldn't fork from tty
        exit(EXIT_FAILURE);
    else if(pid > 0)
        exit(0);    //kill/terminate the parent
    //  Because the parent process has terminated, the child process now runs in the background.

    /*
        Create a new session.
        The calling process becomes the leader of the new session and the process group leader of the new process group.
        The process is now detached from its controlling terminal (CTTY).
    */
    if(setsid() < 0){    //make session leader
        syslog(0, "Couldn't set seesion_id for alerter");
        exit(2);
    }

    // Handling signals
    signal( SIGCHLD, SIG_IGN );
    signal( SIGHUP, SIG_IGN );

    //let the parent process terminate to ensure that you get rid of the session leading process. (Only session leaders may get a TTY again.)
    pid = fork();
    if(pid < 0) //Couldn't fork from parent, ie. give away possibility of taking tty again
        exit(EXIT_FAILURE);
    else if(pid > 0)    // terminate parent
        exit(0);

    umask(222); //DONT GRANT WRITE permission
    chdir("/sys/class/power_supply/BAT0");

    // close();

}

int main(){
    // int percent;  //actually max 3 chars only, ie. 4 length required
    char current[5];
    notify_init("Alerter");

    #ifndef WILL_RUN_BY_SYSTEMD
        make_daemon();    //daeomonize it
    #endif

    while (1){
        fin = fopen("capacity", "r");
        if(fin == NULL) continue;
        // percent = getw( fin );  //not standard   ,, always reading -1

        fgets( current, 4, fin );
        fclose(fin);
        sleep(2.5);

        if(smaller(current, MIN_STR))   alertLow();
        if(smaller(MAX_STR, current))   alertHigh();

    }

    return 0;
}
