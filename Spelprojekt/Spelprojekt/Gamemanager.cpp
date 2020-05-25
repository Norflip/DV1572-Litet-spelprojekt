#include "Gamemanager.h"

Gamemanager::Gamemanager(DX11Handler* dx11) : dx11(dx11)
{
	this->dxhandler = &dx11;

	this->music = new SoundHandler();	
	this->musicVol = 0.5f;
	this->music->SetGlobalVolume(this->musicVol);

	this->soundeffect = new SoundHandler();
	this->soundVol = 1.0f;
	this->soundeffect->SetGlobalVolume(this->soundVol);

	// Level soundtrack from start
	this->currentTrack = 1;			// 1 | 2 | 3 - Track1 | Track2 | Track3
	this->currentMusictrack = "SoundEffects/Ben.wav";

	this->vsyncState = false;

	this->difficultystate = 1;		// 1 = easy | 2 = medium | 3 = hard
	this->timer = 120.0f;			// 120.0f	| 160.0f	 | 200.0f 
	this->nrOfEnemies = 20;			// 20st		| 40st		 | 60st


	// Get latest highscore list ---
	highscoreFiles.open("Datafiles/Highscore.txt");

	for (int i = 0; i < MAXSCORES; i++) {
		highscoreFiles >> this->highscorePoints[i];
		highscoreFiles >> this->highscorename[i];
	}

	highscoreFiles.close();
	
	// Display current textfile
	for (int i = 0; i < MAXSCORES; i++) {
		displayPoints[i] = new GUIText(dx11, std::to_string(static_cast<int>(highscorePoints[i])), 0.0f, 0.0f);
		displayNames[i] = new GUIText(dx11, highscorename[i], 0.0f, 0.0f);
	}	
	// Get latest highscore list ---
}

Gamemanager::~Gamemanager()
{
	this->music = nullptr;
	this->music = 0;

	this->soundeffect = nullptr;
	this->soundeffect = 0;	
}

void Gamemanager::UpdateHighscore(GUI* gui, int score)
{
	if (score > highscorePoints[4]) {
		GUISprite* newHighscore = new GUISprite(*dxhandler, "Sprites/newhighscore.png", 655.0f, 180.0f);
		gui->AddGUIObject(newHighscore, "highscore");

		// Init all to OLD. 
		for (int i = 0; i < MAXSCORES; i++) {
			highscorename[i] = "OLD";
		}		

		// Change 6th place in array before sortation
		highscorePoints[5] = GetCurrentScore();
		highscorename[5] = "LATEST";

		// Sort highscore
		SortHighscore(highscorename, highscorePoints, MAXSCORES);

		// Rename and set new scores to guitexts
		for (int i = 0; i < MAXSCORES; i++) {
			displayPoints[i]->SetString(std::to_string(static_cast<int>(highscorePoints[i]))); 
			displayNames[i]->SetString(highscorename[i]);
		}

		// Write to file again
		writeToHighscore.open("Datafiles/Highscore.txt");

		for (int i = 0; i < MAXSCORES; i++) {
			writeToHighscore << highscorePoints[i] << std::endl;
			writeToHighscore << highscorename[i] << std::endl;
		}

		writeToHighscore.close();
	}	
}

void Gamemanager::DisplayHighscore(GUI* gui)
{
	GUIText* newHighscoreName[5];
	for (int i = 0; i < 5; i++) {
		newHighscoreName[i] = new GUIText(*displayNames[i]);
		newHighscoreName[i]->SetFontSize({ 1.5f, 1.5f });
	}

	newHighscoreName[0]->SetPosition(210, 215);
	newHighscoreName[1]->SetPosition(210, 265);
	newHighscoreName[2]->SetPosition(210, 320);
	newHighscoreName[3]->SetPosition(210, 373);
	newHighscoreName[4]->SetPosition(210, 425);
	
	gui->AddGUIObject(newHighscoreName[0], "firstname");
	gui->AddGUIObject(newHighscoreName[1], "secondname");
	gui->AddGUIObject(newHighscoreName[2], "thirdname");
	gui->AddGUIObject(newHighscoreName[3], "fourthname");
	gui->AddGUIObject(newHighscoreName[4], "fifthname");

	GUIText* newHighscore[5];
	for (int i = 0; i < 5; i++) {
		newHighscore[i] = new GUIText(*displayPoints[i]);
		newHighscore[i]->SetFontSize({ 1.5f, 1.5f });
	}

	newHighscore[0]->SetPosition(415, 215);
	newHighscore[1]->SetPosition(415, 265);
	newHighscore[2]->SetPosition(415, 320);
	newHighscore[3]->SetPosition(415, 373);
	newHighscore[4]->SetPosition(415, 425);

	gui->AddGUIObject(newHighscore[0], "firstscore");
	gui->AddGUIObject(newHighscore[1], "secondscore");
	gui->AddGUIObject(newHighscore[2], "thirdscore");
	gui->AddGUIObject(newHighscore[3], "fourthscore");
	gui->AddGUIObject(newHighscore[4], "fifthscore");

	for (int i = 0; i < 5; i++) {
		if (newHighscoreName[i]->GetString() == "LATEST") {
			newHighscoreName[i]->SetFontColor({ 0,1,0,1 });
			newHighscore[i]->SetFontColor({ 0,1,0,1 });
		}
		else {
			newHighscoreName[i]->SetFontColor({ 1,0,0,1 });
			newHighscore[i]->SetFontColor({ 1,0,0,1 });
		}
	}
	

}

void Gamemanager::SortHighscore(std::string name[], int points[], int totalscores)
{
	for (int i = 0; i < totalscores; i++)
	{
		int min = i;

		for (int k = i + 1; k < totalscores; k++)
		{
			if (points[k] > points[min])
			{
				min = k;
			}
		}

		int oldMin;
		std::string oldnameMin;

		oldMin = points[i];
		oldnameMin = name[i];

		points[i] = points[min];
		name[i] = name[min];
		points[min] = oldMin;
		name[min] = oldnameMin;
	}
}
