/*
Author: Adam Wilson, University of Wisconsin-Madison
Date: 12-16-07
The CertLauncher handles several responsibilities, including:
- parsing the BCI2000Certification.ini file for task info
    e.g., which programs to run
- running the programs for each configuration
- monitoring processes and logging CPU load

It is designed to be as platform independent as possible, but
#DEFINE statements are used where necessary, particularly when
programs need to be launched, and in the process monitoring functions

(C) 2000-2010, BCI2000 Project
http://www.bci2000.org
*/
//---------------------------------------------------------------------------

#ifndef CertLauncherH
#define CertLauncherH

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>

//here is windows stuff
#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <tchar.h>

//#include "analysisClass.h"
#include "TaskType.h"
using namespace std;

/*class TaskConfig {
public:
    string name;
    string SignalSource;
    string SigProc;
    string App;
    string parmFile;
    string folder;

    void clear()
    {
        name = "";
        SignalSource="";
        SigProc = "";
        App="";
        parmFile="";
        folder="";
    }
};*/

class CertLauncher
{
private:

    bool mTasksRemaining;
    int mCurTask;
	string getLogFile();

public:
    CertLauncher();
	~CertLauncher();
	Tasks tasks;

	bool nextTask();
	void reset(){mCurTask = -1;}
    bool tasksRemain(){return mTasksRemaining;}

	bool parseIni();
	bool parseIni(string);
    bool launchProgs();
    bool monitorProgs();

    int nTasks(){return tasks.size();}
    int taskReturnCode(){return tasks.getReturnCode();}

	TaskType& operator[](const int i){return tasks[i];}
	string GlobalSource(){return tasks.GlobalSource;}
	void setGlobalSource(string s){tasks.GlobalSource = s;}
	int GetCurrentTask(){return mCurTask;}
	string mDataDir;
	int mWinLeft, mWinTop, mWinWidth, mWinHeight;
	bool useWinLeft, useWinTop, useWinWidth, useWinHeight;
};
//---------------------------------------------------------------------------
#endif
