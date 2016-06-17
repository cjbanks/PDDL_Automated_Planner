#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <io.h>
#include <map>
#include <vector>
#include <time.h>
//#include <boost/thread/thread.hpp>

using namespace std;

//files
ofstream spreadsheet("C:\\Users\\chris\\Desktop\\PDDLSpreadsheets\\Spreadsheet.xls");

typedef map<string, int> StrIntMap;

//preprocessor
void RunPlanner(string pfileinput, char pfilechar[], char inputchar[]);
void ReadPlannerFile(string& word, string& plancost, string& actions, string& time1, string dataname);
int ReadProblemFile(string& word, string goals, int& count,char pfilechar[], string pfileinput, StrIntMap& goal);
void PrintResults(string plancost, string actions, string time1, int count, string dataname);
void GetTime(string& time1);
//void runsystem(char inputchar[]) { system(inputchar); }

int main()
{
	string plancost = " ";
	string actions = " ";
	string time1 = " ";
	string word;
	string goals = "(and";
	string pfileinput = " ";
	char pfilechar[1024] = "C:\\Users\\chris\\Downloads\\IPC3\\Tests1\\ZenoTravel\\Time\\";
	char *inputchar = " ";
	_finddata_t data;
	int next = 0;
	int file = _findfirst("C:\\Users\\chris\\Downloads\\IPC3\\Tests1\\ZenoTravel\\Time\\pfi*", &data);
	string temp;
	regex e("pfile");
	StrIntMap goal, acti;


	if (file != -1)
	{
		string result;
		ostringstream convert, divert;
		convert << pfilechar;
		result = convert.str();
		pfileinput = result;
		divert << data.name;
		result = divert.str();
		int count = 0;
		pfileinput += result;

		//loops through all files in a specified folder
		while (next != -1)
		{
			if (regex_search(pfileinput, e))
			{
				string result;
				ostringstream convert, divert;
				convert << pfilechar;
				result = convert.str();
				pfileinput = result;
				divert << data.name;
				result = divert.str();
				pfileinput += result;
				string dataname = data.name;
				//boost::thread timedplan(boost::bind(runsystem, inputchar));
				RunPlanner(pfileinput, pfilechar, inputchar);
		     	ReadPlannerFile(word, plancost, actions, time1, dataname);
				GetTime(time1);
				count = ReadProblemFile(word, goals, count, pfilechar, pfileinput, goal);
				PrintResults(plancost, actions, time1, count, dataname);
				//For Time
				cout << "Finished with " << data.name << "....." << endl;
				//For Time
				count = 0;
				goal.clear();
			}

			next = _findnext(file, &data);
		}
		_findclose(file);
	}

	else
	{
		cout << "FILE DOES NOT EXIST" << endl;
	}

	cout << "Done." << endl;
	spreadsheet.close();
	cin.get();
	return 0;
	
}

//run planner file through command prompt
void RunPlanner(string pfileinput, char pfilechar[], char inputchar[])
{
	//Run for Strips and Numeric
	//string input = "C:\\Users\\chris\\lpg-td-1.0.exe -o C:\\Users\\chris\\Downloads\\IPC3\\Tests1\\ZenoTravel\\Numeric\\zenonumeric.pddl -f ";
	//input = input + pfileinput + " -quality";

	//Run for Time
	string input = "C:\\Users\\chris\\lpg-td-1.0.exe -o C:\\Users\\chris\\Downloads\\IPC3\\Tests1\\ZenoTravel\\Time\\zenotravelTandN.pddl -f ";
	input = input + pfileinput + " -quality -noout > text.SOL";

	inputchar = (char*)malloc(input.size() + 1);
	memcpy(inputchar, input.c_str(), input.size() + 1);
	system(inputchar);
}


//reads planner file generated from the planner
void ReadPlannerFile(string& word, string& plancost, string& actions, string& time1, string dataname)
{
	//run for Strips and Numeric
	//string PlanFile = "plan_";
	//PlanFile = PlanFile + dataname + "_1.SOL";
	
	//Run for Time
	string PlanFile = "text.SOL";

	ifstream planningfile(PlanFile);
	int num = 0;
	/*
	while (!planningfile.eof())
	{
		planningfile >> word;
		regex cost("Quality");
		regex timing("Time");
		regex action("\\[.*\\]");


		if (regex_search(word, cost))
			planningfile >> plancost;
		
		 if (regex_search(word, timing))
			planningfile >> time1;
		 
		 if (regex_search(word, action))
			 num++;
		
	}*/

	string result;
	ostringstream convert;
	num--;
	convert << num;
	result = convert.str();
	actions = result;
	

	//For Time

	while (!planningfile.eof())
	{
		planningfile >> word;
		regex cost("quality:");
		regex action("Actions:");

		if (regex_search(word, cost))
			planningfile >> plancost;

		if (regex_search(word, action))
			planningfile >> actions;

	}

	//cout << num;
	//plancost = actions;
	planningfile.close();
}

void GetTime(string& time1)
{
	string PlanFile = "text.SOL";

	ifstream planningfile(PlanFile);

	while (!planningfile.eof())
	{
		regex timing("Total time:");
		string line;

		getline(planningfile, line);
		if (regex_search(line, timing))
		{
			cout << line;
			time1 = line;
		}
	}

}


//reads problem file to determine number of goals
int ReadProblemFile(string& line, string goals, int& count, char pfilechar[],string pfileinput, StrIntMap& goal)
{
	ifstream problemfile(pfileinput);
	while (!problemfile.eof())
	{
		regex goalie("\\(.*\\)");
		regex paren("and");
		string endgoal;
		problemfile >> endgoal;

		if (regex_search(endgoal,paren))
		{
			while (!problemfile.eof())
			{
				getline(problemfile, line);
				sregex_iterator it(line.begin(), line.end(), goalie);
				sregex_iterator it_end;
				while (it != it_end)
				{
					count++;
					++it;
				}
			}
		}
	}
	
	problemfile.close();

	//For Numeric and Time ONLY
	count--;
	//FOR NUMERIC and Time ONLY
	return count;

}

//prints results to spreadsheet.xls
void PrintResults(string plancost, string actions, string time1, int count, string dataname)
{
	spreadsheet << endl<< dataname << endl << "Plan Cost: " << plancost << endl << "Number of Actions: " <<
		actions << endl << "Computation Time: " << time1 << endl << "Number of goals: " << count << endl;
}
