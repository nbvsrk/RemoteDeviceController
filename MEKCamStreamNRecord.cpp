/* 
 * File:   MEKCamStreamNRecord.cpp
 * Author: newmek7
 *
 * Created on 27 March, 2013, 12:03 PM
 */

#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include "mystdlib.h"
#include <sys/types.h>
#include <signal.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    pid_t fcpid;
    time_t rawtime;
    struct tm * timeinfo;
    char fn [80];
    time(&rawtime);
    int ecode;
    while (true) {
	timeinfo = localtime(&rawtime);
	strftime(fn, 80, "record/%Y-%m-%d %H:%M:%S.flv", timeinfo);
	char * args[] = {"ffmpeg", "-f", "v4l2", "-r", "15", "-s", "40x30", "-i", "/dev/video0", "-f", "flv", "rtmp://192.168.2.25:25333/venkat/test1", fn, "&>", "/dev/null", NULL};
	ecode = system("pkill -9 ffmpeg &> /dev/null");
	fcpid = spawn("ffmpeg", args);
	sleep(15 * 60);
    }
    return 0;
}

