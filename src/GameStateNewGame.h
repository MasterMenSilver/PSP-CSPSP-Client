#ifndef _GAME_STATE_NEW_GAME_H_
#define _GAME_STATE_NEW_GAME_H_

#include <vector>
#include <algorithm>
#include "GameState.h"
#include "MenuItem.h"
#include "ListBox.h"
#ifdef WIN32
#else
#include <malloc.h>
#include <pspdisplay.h>
#include <pspkernel.h>
#include <dirent.h>
#include <unistd.h>

#define TDM 0
#define CTF 1
#define FFA 2
#define ZM 3

#define Map 0
#define Set 1

#define NGGame 0
#define NGBots 1
#define NGRules 2

	#define NGGamemode 0
	#define NGTime 1


	#define NGMaxBots 0
	#define NGAllowCustomName 9
	#define NGDifficulty 0

		#define NoHarm 0
		#define Easy 1
		#define Medium 2
		#define Hard 3
	
	#define NGAllowRegen 0
	
		#define NGRegenTime 0
		#define NGRegenHP 1

#endif

class MapItem : public ListItem 
{
public:
	char name[256];
	bool match;
	MapItem(char *name) {
		strcpy(this->name, name);
		match = false;
	}
	~MapItem() {}

	void Render(float x, float y, bool selected) {
		JRenderer *renderer = JRenderer::GetInstance();
		if (match && !selected) {
			renderer->FillRect(0,y,SCREEN_WIDTH,25,ARGB(100,0,128,255));
		}
		gFont->DrawShadowedString(name,x,y+3);
	}
	static bool Compare(ListItem* first, ListItem* second) {
		MapItem *a = (MapItem*)first;
		MapItem *b = (MapItem*)second;
		return (strcmpi(a->name,b->name) <= 0);
	}
};


class GameStateNewGame:	public GameState

{
private:
	ListBox *mMapsListBox;

	char mSearchString[256];
	bool mMatch;
	int mMatchIndex;
	
	

public:
	GameStateNewGame(GameApp* parent);
	~GameStateNewGame();
	
	int NGmState;
	int NGmPage;
	
		int NGmGame;
		int NGmBots;
		int NGmRules;
		
			int NGGameTime;
			int GM;
			
			int MaxBots;
			bool CustomNames;
			int BotDiff;
			
			bool AllowRegeneration;
			int RegenerationTime;
			int HealthPerRegeneration;
	
	void Create();
	void Destroy();
	void Start();
	void End();
	void Update(float dt);
	void Load();
	void Save();
	void Render();

};


#endif

