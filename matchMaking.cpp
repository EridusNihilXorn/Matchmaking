// matchMaking.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>

using namespace std;
// std::func-name is not used cuz it is not growing type of project for time-economy.

void playersFileToVector(ifstream& players, string fileName, vector<int>& playersVector)
{
	string line;
	string word;
	try
	{
		players.open(fileName);
	}
	catch (ios_base::failure)
	{
		cout << "Failed to open players file.\n";
	}
	int counter = 0;
	if (players.good())
	{
		while (getline(players, line))  //Getting current string-player.
		{
			stringstream streamLine(line);
			getline(streamLine, word, ' '); //Geting number of a player.
			//cout << "Player " << word << " ";
			getline(streamLine, word, ' ');//Getting rating of player.
			playersVector.push_back(stoi(word));
			counter++;
			//cout << "Rating " << word << "\n";
		}
		//cout << " >>>>>>>>>> Players added: " << counter << "\n";
		players.close();
	}
}

void createTeamList(ifstream& teams, string teamsFileName, vector<pair<int,int>>& teamsRatings,vector<int>& playersRatings) 
{
	string line, word;
	int playerRating, curTeamNum = 0;
	try
	{
		teams.open(teamsFileName);
	}
	catch (ios_base::failure)
	{
		cout << "Failed to open teams file.\n";
	}
	if (teams.good())
	{
		while (getline(teams, line))  //Getting current string-team.
		{
			stringstream streamLine(line);
			getline(streamLine, word, ' '); //Geting number of a team.
			//cout << "Current team is " << word << "\n";
			teamsRatings.push_back(make_pair(stoi(word), 0));
			while (getline(streamLine, word, ' '))  //Getting rating of players.
			{
				//cout << "Number player. " << word << "\n";
				playerRating = playersRatings[stoi(word)];
				//cout << "Player's rating. " << playerRating << "\n";
				teamsRatings[curTeamNum].second += playerRating;
			}
			//cout << ">>>>>>>>>>>Team " << teamsRatings[curTeamNum].first << " Rating " << teamsRatings[curTeamNum].second << "\n";
			curTeamNum++;
		}
		//cout << "Amount of teams " << curTeamNum << "\n";
		teams.close();
	}
}

bool compareFunc(const pair<int,int> &a, const pair<int,int> &b)
{
	return (a.second  < b.second);
}

int main()
{
	ifstream teams;
	ifstream players;
	ofstream matchMaking;
	vector <pair<int, int>> teamsRatings;
	vector <int> playersRatings;

	playersFileToVector(players, "players.txt", playersRatings);
	createTeamList(teams, "teams.txt", teamsRatings, playersRatings);
	sort(teamsRatings.begin(), teamsRatings.end(), compareFunc);

	try
	{
		matchMaking.open("TEST_A_pairs.txt");       //Change filename to write in correct file!
	}
	catch (ios_base::failure)
	{
		cout << "Failed to open test file.\n";
	}

	if (teamsRatings.size() % 2 == 0)
	{
		for (vector<pair<int, int>>::iterator it = teamsRatings.begin(); it < teamsRatings.end(); it++)
		{
			matchMaking << it->first << " ";
			it++;
			matchMaking << it->first << "\n";
		}
	}
	//Deleting first or last team in vector depending on their distance from closest ones.
	//This algorithm is totally unefficient for low amount of teams,
	//since their difference in rating is large.
	//But it is efficient for a large amount of teams with low difference.
	//We have extra large amount of players, that's why i use this one.
	//More complex algorithm is written in document with more details.
	if (teamsRatings.size() % 2 != 0)
	{
		if (teamsRatings[1].second - teamsRatings[0].second > teamsRatings[teamsRatings.size() - 1].second - teamsRatings[teamsRatings.size() - 2].second)
		{
			for (vector<pair<int, int>>::iterator it = teamsRatings.begin() + 1; it < teamsRatings.end(); it++)
			{
				matchMaking << it->first << " ";
				it++;
				matchMaking << it->first << "\n";
			}
		}
		else
		{
			for (vector<pair<int, int>>::iterator it = teamsRatings.begin(); it < teamsRatings.end() - 1; it++)
			{
				matchMaking << it->first << " ";
				it++;
				matchMaking << it->first << "\n";
			}
		}
	}
    return 0;
}

