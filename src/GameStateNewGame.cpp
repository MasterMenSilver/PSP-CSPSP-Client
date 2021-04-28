#include "GameStateNewGame.h"

GameStateNewGame::GameStateNewGame(GameApp* parent): GameState(parent)
{
	NGmState = Map;
	// Map, Set
	
	NGmPage = NGGame;
	// NGGame, NGBots, NGRules
	
	
	
		NGmGame = NGGamemode;
		// NGGamemode, NGTime
		
		NGmBots = NGMaxBots;
		// NGMaxBots, NGAllowCustomName, NGDifficulty
			//	MaxBots = -1 - Map Value
			//	MaxBots = 0 - No Bots
			//	MaxBots = 1 - 32 - Bots Count
			
		NGmRules = NGAllowRegen;
		// NGRegenTime, NGRegenHP
		
		/*
		bool AllowRegeneration;
		int RegenerationTime;
		int HealthPerRegeneration;
		*/
	
	GM = TDM;
	// TDM, CTF, FFA, ZM
	NGGameTime = 12;
	AllowRegeneration = false;
	MaxBots = -1;
	RegenerationTime = 1;
	HealthPerRegeneration = 5;
}

GameStateNewGame::~GameStateNewGame()
{

}


void GameStateNewGame::Create()
{
	mMapsListBox = new ListBox(0,65,SCREEN_WIDTH,175,25,7);

	strcpy(mSearchString,"");
	mMatch = true;
	mMatchIndex = -1;

	gFont->SetBase(0);	// using 2nd font

	int y = 20;
	#ifdef WIN32
	for (int i=0; i<10; i++) {
		char buffer[10];
		sprintf(buffer,"test%d",i);
		mMapsListBox->AddItem(new MapItem(buffer));
	}
	#else
	
	int index = 0;
	DIR *dip;
	struct dirent *dit;
	dip = opendir("maps");
	char fullname[512];

	while ((dit = readdir(dip)) != NULL)
	{
		static char * name; 
		name = (char*)memalign( 16, 300); 
		sprintf(name,"%s" , dit->d_name);  
	 
		if ((FIO_S_IFREG & (dit->d_stat.st_mode & FIO_S_IFMT)) == 0 && stricmp (name, ".") != 0 && stricmp (name, "..") != 0) 
		{
			mMapsListBox->AddItem(new MapItem(name));
		}
	}
	closedir(dip);


	/**int index = 0;
	int fd; 
	struct SceIoDirent dirent; 
	fd = sceIoDopen("ms0:/psp/game/cspsp/maps"); 

	memset( &dirent, 0, sizeof(SceIoDirent)); 
	 
	while (sceIoDread(fd, &dirent) > 0) 
	{ 
		static char * name; 
		name = (char*)memalign( 16, 300); 
		sprintf(name,"%s" , dirent.d_name); 
		//sprintf(fullname, "%s/%s", "ms0:/PSP", dirent.d_name); 
	 
		if ((FIO_S_IFREG & (dirent.d_stat.st_mode & FIO_S_IFMT)) == 0 && stricmp (name, ".") != 0 && stricmp (name, "..") != 0) 
		{ 
			if (index == 0) {
				mGuiController->Add(new MenuItem(index, gFont, name, 20, y, TYPE_NORMAL, JGETEXT_LEFT, true));
			}else{
				mGuiController->Add(new MenuItem(index, gFont, name, 20, y, TYPE_NORMAL, JGETEXT_LEFT, false));
			}
			maps.push_back(name);
			index++;
			y += 20;
		} 
	} 
	sceIoDclose(fd);**/

	#endif

	mMapsListBox->Sort(MapItem::Compare);
}


void GameStateNewGame::Destroy()
{
}


void GameStateNewGame::Start()
{
	mRenderer->EnableVSync(true);
}


void GameStateNewGame::End()
{
	mRenderer->EnableVSync(false);
}


void GameStateNewGame::Update(float dt)
{
	if(NGmState == Map) {
		if (!gDanzeff->mIsActive) {
			if (mEngine->GetButtonClick(PSP_CTRL_CIRCLE)) {
				mParent->SetNextState(GAME_STATE_MENU);
			}
			if (mEngine->GetButtonClick(PSP_CTRL_SQUARE)) {
				gDanzeff->Enable();
				gDanzeff->mString = mSearchString;
			}
			if (mEngine->GetButtonClick(PSP_CTRL_CROSS)) {
				MapItem *item = (MapItem*)mMapsListBox->GetItem();
				if (item != NULL) {
					gMapName = item->name;
					NGmState = Set;
					return;
				}
			}

			mMapsListBox->Update(dt);
		}
		else {
			gDanzeff->Update(dt);
			if (gDanzeff->mString.length() > 50) {
				gDanzeff->mString = gDanzeff->mString.substr(0,50);
			}
			bool search = false;
			if (stricmp(mSearchString,(char*)gDanzeff->mString.c_str()) != 0) {
				search = true;
				strcpy(mSearchString,(char*)gDanzeff->mString.c_str());
			}
			if (mEngine->GetButtonClick(PSP_CTRL_START)) {
				search = true;
				gDanzeff->Disable();
			}
			else if (mEngine->GetButtonClick(PSP_CTRL_SELECT)) {
				gDanzeff->Disable();
			}

			if (search) {
				mMatch = false;
				mMatchIndex = -1;
				if (strlen(mSearchString) > 0) {
					for (int i=0; i<mMapsListBox->mItems.size(); i++) {
						MapItem *item = (MapItem*)mMapsListBox->mItems[i];
						//printf("%d\n",strnicmp(mSearchString,mMaps[i].name,strlen(mSearchString)));
						int s = strnicmp(mSearchString,item->name,strlen(mSearchString));
						if (s == 0) {
							mMatch = true;
							mMatchIndex = i;
							break;
						}
						else if (s < 0) {
							mMatch = false;
							mMatchIndex = -1;
							break;
						}
					}
				}
				for (int i=0; i<mMapsListBox->mItems.size(); i++) {
					MapItem *item = (MapItem*)mMapsListBox->mItems[i];
					if (i == mMatchIndex) {
						item->match = true;
						mMapsListBox->SetIndices(mMatchIndex,mMatchIndex);
					}
					else {
						item->match = false;
					}
				}
			}
		}
	}
	
	/*
	if(mState == Main) {
		
	}
	*/
	
	if(NGmState == Set) {
		if(NGmPage == NGGame) {
			// Gamemode
			if(NGmGame == NGGamemode) {
				if (mEngine->GetButtonClick(PSP_CTRL_LEFT)) {
					//GM--;
					//if (GM < 0) {
					//	GM = TDM;
					//}
				}
				if (mEngine->GetButtonClick(PSP_CTRL_RIGHT)) {
					//GM++;
					//if (GM > 2) {
					//	GM = FFA;
					//}
					
					//GM++;
					//if (GM > 3) {
					//	GM = ZM;
					//}
				}
				
				
				if (mEngine->GetButtonClick(PSP_CTRL_UP)) {
					//NGmGame = NGMulti;
				}
				if (mEngine->GetButtonClick(PSP_CTRL_DOWN)) {
					NGmGame = NGTime;
				}
				
				
				if (mEngine->GetButtonClick(PSP_CTRL_CIRCLE)) {
					NGmState = Map;
				}
				if (mEngine->GetButtonClick(PSP_CTRL_CROSS)) {
					Save();
					NGmState = Map;
					mParent->SetNextState(GAME_STATE_PLAY);
				}
			}
			if(NGmGame == NGTime) {
				if (mEngine->GetButtonClick(PSP_CTRL_LEFT)) {
					NGGameTime--;
				}
				if (mEngine->GetButtonClick(PSP_CTRL_RIGHT)) {
					NGGameTime++;
				}
				
				if (mEngine->GetButtonClick(PSP_CTRL_SQUARE)) {
					NGGameTime = 16;
				}
				
				if (mEngine->GetButtonClick(PSP_CTRL_TRIANGLE)) {
					NGGameTime = 1;
				}
				
				if (mEngine->GetButtonClick(PSP_CTRL_UP)) {
					NGmGame = NGGamemode;
				}
				if (mEngine->GetButtonClick(PSP_CTRL_DOWN)) {
					//NGmGame = NGMulti;
				}
				
				
				if (mEngine->GetButtonClick(PSP_CTRL_CIRCLE)) {
					NGmState = Map;
				}
				if (mEngine->GetButtonClick(PSP_CTRL_CROSS)) {
					Save();
					NGmState = Map;
					mParent->SetNextState(GAME_STATE_PLAY);
				}
			}
			
			/*
			if(NGmGame == NGMulti) {
				if (mEngine->GetButtonClick(PSP_CTRL_LEFT)) {
					Multi--;
				}
				if (mEngine->GetButtonClick(PSP_CTRL_RIGHT)) {
					Multi++;
				}
				
				
				if (mEngine->GetButtonClick(PSP_CTRL_UP)) {
					NGmGame = NGTime;
				}
					if (mEngine->GetButtonClick(PSP_CTRL_DOWN)) {
					NGmGame = NGGamemode;
				}
				
				
				if (mEngine->GetButtonClick(PSP_CTRL_CIRCLE)) {
					NGmState = Map;
				}
				if (mEngine->GetButtonClick(PSP_CTRL_CROSS)) {
					Save();
					NGmState = Map;
					mParent->SetNextState(GAME_STATE_PLAY);
				}
			}
			*/
			
			if (mEngine->GetButtonClick(PSP_CTRL_RTRIGGER)) {
				NGmPage = NGBots;
			}
			if (mEngine->GetButtonClick(PSP_CTRL_LTRIGGER)) {
				NGmPage = NGRules;
			}
			
			if(NGGameTime > 16) {
				NGGameTime = 16;
			}
			if(NGGameTime < 1) {
				NGGameTime = 1;
			}
		}
		
		
		// Bots
		if(NGmPage == NGBots) {
			if (NGmBots == NGMaxBots) {
				if (mEngine->GetButtonClick(PSP_CTRL_LEFT)) {
					MaxBots--;
				}
				if (mEngine->GetButtonClick(PSP_CTRL_RIGHT)) {
					MaxBots++;
				}
				
				if (mEngine->GetButtonClick(PSP_CTRL_SQUARE)) {
					MaxBots = 16;
				}
				
				if (mEngine->GetButtonClick(PSP_CTRL_TRIANGLE)) {
					MaxBots = -1;
				}
				
				if (mEngine->GetButtonClick(PSP_CTRL_UP)) {
					NGmBots = NGDifficulty;
				}
				if (mEngine->GetButtonClick(PSP_CTRL_DOWN)) {
					NGmBots = NGAllowCustomName;
				}
			}
			if(NGmBots == NGAllowCustomName) {
				if (mEngine->GetButtonClick(PSP_CTRL_LEFT)) {
					CustomNames = false;
				}
				if (mEngine->GetButtonClick(PSP_CTRL_RIGHT)) {
					CustomNames = true;
				}
				
				
				if (mEngine->GetButtonClick(PSP_CTRL_UP)) {
					NGmBots = NGMaxBots;
				}
				if (mEngine->GetButtonClick(PSP_CTRL_DOWN)) {
					NGmBots = NGDifficulty;
				}
			}
			if(NGmBots == NGDifficulty) {
				if (BotDiff == NoHarm) {
					if (mEngine->GetButtonClick(PSP_CTRL_LEFT)) {
						BotDiff = Hard;
					}
					if (mEngine->GetButtonClick(PSP_CTRL_RIGHT)) {
						BotDiff = Easy;
					}
				}
				if (BotDiff == Easy) {
					if (mEngine->GetButtonClick(PSP_CTRL_LEFT)) {
						BotDiff = NoHarm;
					}
					if (mEngine->GetButtonClick(PSP_CTRL_RIGHT)) {
						BotDiff = Medium;
					}
				}
				if (BotDiff == Medium) {
					if (mEngine->GetButtonClick(PSP_CTRL_LEFT)) {
						BotDiff = Easy;
					}
					if (mEngine->GetButtonClick(PSP_CTRL_RIGHT)) {
						BotDiff = Hard;
					}
				}
				if (BotDiff == Hard) {
					if (mEngine->GetButtonClick(PSP_CTRL_LEFT)) {
						BotDiff = Medium;
					}
					if (mEngine->GetButtonClick(PSP_CTRL_RIGHT)) {
						BotDiff = NoHarm;
					}
				}
				
				
				if (mEngine->GetButtonClick(PSP_CTRL_UP)) {
					NGmBots = NGMaxBots;
				}
				if (mEngine->GetButtonClick(PSP_CTRL_DOWN)) {
					NGmBots = NGDifficulty;
				}
			}
			
			if (mEngine->GetButtonClick(PSP_CTRL_CIRCLE)) {
				NGmState = Map;
			}
			if (mEngine->GetButtonClick(PSP_CTRL_CROSS)) {
				Save();
				NGmState = Map;
				mParent->SetNextState(GAME_STATE_PLAY);
			}
			
			if (mEngine->GetButtonClick(PSP_CTRL_RTRIGGER)) {
				NGmPage = NGRules;
			}
			if (mEngine->GetButtonClick(PSP_CTRL_LTRIGGER)) {
				NGmPage = NGGame;
			}
			
			if (MaxBots > 16) { MaxBots = 16; }
			if (MaxBots < -1) { MaxBots = -1; }
		}
		
		
		
		// Rules Page
		if(NGmPage == NGRules) {
			if (mEngine->GetButtonClick(PSP_CTRL_CIRCLE)) {
				NGmState = Map;
			}
			if (mEngine->GetButtonClick(PSP_CTRL_CROSS)) {
				Save();
				NGmState = Map;
				mParent->SetNextState(GAME_STATE_PLAY);
			}
			
			if(NGmRules == NGAllowRegen) {
				if (mEngine->GetButtonClick(PSP_CTRL_LEFT)) {
					AllowRegeneration = false;
				}
				if (mEngine->GetButtonClick(PSP_CTRL_RIGHT)) {
					AllowRegeneration = true;
				}
				
			}
			
			if (mEngine->GetButtonClick(PSP_CTRL_RTRIGGER)) {
				NGmPage = NGGame;
			}
			if (mEngine->GetButtonClick(PSP_CTRL_LTRIGGER)) {
				NGmPage = NGBots;
			}
			if (NGmRules != NGAllowRegen) { NGmRules = NGAllowRegen; }
		}
	}
}

void GameStateNewGame::Save()
{
	FILE *file;
	file = fopen("data/MatchSettings.txt", "w"); 
	if (file == NULL) return;
	
	if (GM == TDM) {
		fputs("Gamemode = TDM\r\n",file);
	}
	if (GM == CTF) {
		fputs("Gamemode = CTF\r\n",file);
	}
	if (GM == FFA) {
		fputs("Gamemode = FFA\r\n",file);
	}
	if (GM == ZM) {
		fputs("Gamemode = ZM\r\n",file);
	}
	
	fputs("RoundTime = ",file);
	if(NGGameTime == 1) {
		fputs("1",file);
	}
	if(NGGameTime == 2) {
		fputs("2",file);
	}
	if(NGGameTime == 3) {
		fputs("3",file);
	}
	if(NGGameTime == 4) {
		fputs("4",file);
	}
	if(NGGameTime == 5) {
		fputs("5",file);
	}
	if(NGGameTime == 6) {
		fputs("6",file);
	}
	if(NGGameTime == 7) {
		fputs("7",file);
	}
	if(NGGameTime == 8) {
		fputs("8",file);
	}
	if(NGGameTime == 9) {
		fputs("9",file);
	}
	if(NGGameTime == 10) {
		fputs("10",file);
	}
	if(NGGameTime == 11) {
		fputs("11",file);
	}
	if(NGGameTime == 12) {
		fputs("12",file);
	}
	if(NGGameTime == 13) {
		fputs("13",file);
	}
	if(NGGameTime == 14) {
		fputs("14",file);
	}
	if(NGGameTime == 15) {
		fputs("15",file);
	}
	if(NGGameTime == 16) {
		fputs("16",file);
	}
	fputs("\r\n",file);
	
	if (MaxBots == -1) {
		fputs("MaxBots = -1",file);
	}
	if (MaxBots == 0) {
		fputs("MaxBots = 0",file);
	}
	if (MaxBots == 1) {
		fputs("MaxBots = 2",file);
	}
	if (MaxBots == 2) {
		fputs("MaxBots = 4",file);
	}
	if (MaxBots == 3) {
		fputs("MaxBots = 6",file);
	}
	if (MaxBots == 4) {
		fputs("MaxBots = 8",file);
	}
	if (MaxBots == 5) {
		fputs("MaxBots = 10",file);
	}
	if (MaxBots == 6) {
		fputs("MaxBots = 12",file);
	}
	if (MaxBots == 7) {
		fputs("MaxBots = 14",file);
	}
	if (MaxBots == 8) {
		fputs("MaxBots = 16",file);
	}
	if (MaxBots == 9) {
		fputs("MaxBots = 18",file);
	}
	if (MaxBots == 10) {
		fputs("MaxBots = 20",file);
	}
	if (MaxBots == 11) {
		fputs("MaxBots = 22",file);
	}
	if (MaxBots == 12) {
		fputs("MaxBots = 24",file);
	}
	if (MaxBots == 13) {
		fputs("MaxBots = 26",file);
	}
	if (MaxBots == 14) {
		fputs("MaxBots = 28",file);
	}
	if (MaxBots == 15) {
		fputs("MaxBots = 30",file);
	}
	if (MaxBots == 16) {
		fputs("MaxBots = 32",file);
	}
	
	fputs("\r\n",file);
	
	if (CustomNames == true) {
		fputs("CustomNames = true",file);
	}
	if (CustomNames == false) {
		fputs("CustomNames = false",file);
	}
	
	fputs("\r\n",file);
	
	if (AllowRegeneration == true) {
		fputs("AllowRegeneration = true",file);
	}
	if (AllowRegeneration == false) {
		fputs("AllowRegeneration = false",file);
	}
	
	fputs("\r\n",file);
	
	fputs("maxMoney = 16000",file);
	
	
	fputs("\r\n",file);
	
	fclose(file);
}

void GameStateNewGame::Render()
{
	
	if(NGmState == Map) {
		mRenderer->ClearScreen(ARGB(255,255,255,255));
		//mRenderer->FillRect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,ARGB(255,255,255,255));
		mRenderer->RenderQuad(gBgQuad, 0.0f, 0.0f);
		//mEngine->FillRect(15,15,SCREEN_WIDTH-30,SCREEN_HEIGHT-30,ARGB(150,0,0,0));

		mRenderer->FillRect(0,65,SCREEN_WIDTH,175,ARGB(100,0,0,0));
		mRenderer->FillRect(0,35,SCREEN_WIDTH,30,ARGB(175,0,0,0));
		mRenderer->DrawLine(90,35,90,65,ARGB(255,255,255,255));

		gFont->SetColor(ARGB(255,255,255,255));


		gFont->SetScale(1.0f);
		gFont->DrawShadowedString("Map Selection",20,10);
		gFont->DrawShadowedString("Search:",20,40);
		if (mMatch) {
			gFont->SetColor(ARGB(255,0,128,255));
		}
		else {
			gFont->SetColor(ARGB(255,255,0,0));
		}
		gFont->DrawShadowedString(mSearchString,100,40);

		gFont->SetColor(ARGB(255,255,255,255));
		if (gDanzeff->mIsActive) {
			gFont->DrawShadowedString("|",100+gFont->GetStringWidth(mSearchString),40);
		}

		mMapsListBox->Render();

		gFont->SetColor(ARGB(255,255,255,255));
		gFont->SetScale(0.75f);
		if (gDanzeff->mIsActive) {
			gFont->DrawShadowedString("[START] Enter    [SELECT] Cancel",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
			gDanzeff->Render(SCREEN_WIDTH-175,SCREEN_HEIGHT-175);
		}
		else {
			gFont->DrawShadowedString("[X] Select     [[]] Search     [O] Return to Menu",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
		}
	}
	
	////////////////////////////////////////////////////////////////////////////////////
	/*
	if(mState == Main) {
		mRenderer->ClearScreen(ARGB(255,255,255,255));
		//mRenderer->FillRect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,ARGB(255,255,255,255));
		mRenderer->RenderQuad(gBgQuad, 0.0f, 0.0f);
		//mEngine->FillRect(15,15,SCREEN_WIDTH-30,SCREEN_HEIGHT-30,ARGB(150,0,0,0));
		mRenderer->FillRect(0,31,SCREEN_WIDTH,175,ARGB(100,0,0,0));
		mRenderer->FillRect(0,0,SCREEN_WIDTH,29,ARGB(175,0,0,0));
		
		gFont->SetColor(ARGB(255,255,244,53));
		gFont->SetScale(1.0f);
		gFont->DrawShadowedString("Singleplayer",SCREEN_WIDTH_2,6,JGETEXT_CENTER);
		
		
		
		gFont->SetScale(0.75f);
		gFont->DrawShadowedString("[DIR PAD/ANALOG] Change Selection     [X] Select     [O] Return to Menu",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
	}
	*/
	
	if(NGmState == Set) {
		mRenderer->ClearScreen(ARGB(255,255,255,255));
		//mRenderer->FillRect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,ARGB(255,255,255,255));
		mRenderer->RenderQuad(gBgQuad, 0.0f, 0.0f);
		
		gFont->SetScale(0.75f);
		gFont->SetColor(ARGB(255,255,255,255));
		gFont->DrawShadowedString("< [L]",10,34,JGETEXT_LEFT);
		gFont->DrawShadowedString("[R] >",SCREEN_WIDTH-10,34,JGETEXT_RIGHT);
		
		//mEngine->FillRect(15,15,SCREEN_WIDTH-30,SCREEN_HEIGHT-30,ARGB(150,0,0,0));
		mRenderer->FillRect(0,60,SCREEN_WIDTH,175,ARGB(100,0,0,0));
											//  ARGB(100,0,0,0)
		gFont->SetColor(ARGB(255,255,255,255));
		gFont->SetScale(1.0f);
		gFont->DrawShadowedString("Singleplayer",SCREEN_WIDTH_2,5,JGETEXT_CENTER);
		mRenderer->FillRect(100 - 5,30,80,30,ARGB(100,0,0,0)); // Game
		mRenderer->FillRect(200 - 5,30,80,30,ARGB(100,0,0,0)); // Bots
		mRenderer->FillRect(300 - 5,30,80,30,ARGB(175,0,0,0)); // Rules
		
		
		
		if(NGmPage == NGGame) {
			gFont->SetScale(1.0f);
			gFont->SetColor(ARGB(255,255,244,53));
			gFont->DrawShadowedString("Game",135,35,JGETEXT_CENTER);
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->DrawShadowedString("Bots",238,35,JGETEXT_CENTER);
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->DrawShadowedString("Rules",335,35,JGETEXT_CENTER);
			if(NGmGame == NGGamemode) {
				gFont->SetColor(ARGB(255,255,244,53));
				gFont->SetScale(1.0f);
				mRenderer->FillRect(0,60,SCREEN_WIDTH,25,ARGB(175,0,0,0));
				gFont->DrawShadowedString("Gamemode",150,65,JGETEXT_RIGHT);
				gFont->SetColor(ARGB(255,255,255,255));
				if(GM == TDM) {
					gFont->DrawShadowedString("TDM",170,65,JGETEXT_LEFT);
				}
				if(GM == CTF) {
					gFont->DrawShadowedString("CTF",170,65,JGETEXT_LEFT);
				}
				if(GM == FFA) {
					gFont->DrawShadowedString("FFA",170,65,JGETEXT_LEFT);
				}
				if(GM == ZM) {
					gFont->DrawShadowedString("ZM",170,65,JGETEXT_LEFT);
				}
				
				gFont->SetScale(0.75f);
				gFont->DrawShadowedString("[X] Start Game     [O] Return to Menu     [<] [>] Change",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
			}
			else {
				gFont->SetColor(ARGB(255,255,255,255));
				gFont->SetScale(1.0f);
				gFont->DrawShadowedString("Gamemode",150,65,JGETEXT_RIGHT);
				gFont->SetColor(ARGB(255,255,255,255));
				if(GM == TDM) {
					gFont->DrawShadowedString("TDM",170,65,JGETEXT_LEFT);
				}
				if(GM == CTF) {
					gFont->DrawShadowedString("CTF",170,65,JGETEXT_LEFT);
				}
				if(GM == FFA) {
					gFont->DrawShadowedString("FFA",170,65,JGETEXT_LEFT);
				}
				if(GM == ZM) {
					gFont->DrawShadowedString("ZM",170,65,JGETEXT_LEFT);
				}
			}
			
			if(NGmGame == NGTime) {
				gFont->SetColor(ARGB(255,255,244,53));
				gFont->SetScale(1.0f);
				mRenderer->FillRect(0,85,SCREEN_WIDTH,25,ARGB(175,0,0,0));
				gFont->DrawShadowedString("Time (Min:Sec)",150,90,JGETEXT_RIGHT);
				gFont->SetColor(ARGB(255,255,255,255));
				
				if(NGGameTime == 1) {
					gFont->DrawShadowedString("0:15",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 2) {
					gFont->DrawShadowedString("0:30",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 3) {
					gFont->DrawShadowedString("0:45",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 4) {
					gFont->DrawShadowedString("1:00",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 5) {
					gFont->DrawShadowedString("1:15",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 6) {
					gFont->DrawShadowedString("1:30",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 7) {
					gFont->DrawShadowedString("1:45",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 8) {
					gFont->DrawShadowedString("2:00",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 9) {
					gFont->DrawShadowedString("2:15",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 10) {
					gFont->DrawShadowedString("2:30",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 11) {
					gFont->DrawShadowedString("2:45",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 12) {
					gFont->DrawShadowedString("3:00",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 13) {
					gFont->DrawShadowedString("3:15",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 14) {
					gFont->DrawShadowedString("3:30",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 15) {
					gFont->DrawShadowedString("3:45",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 16) {
					gFont->DrawShadowedString("4:00",170,90,JGETEXT_LEFT);
				}
				
				gFont->SetColor(ARGB(255,255,255,255));
				gFont->SetScale(0.75f);
				gFont->DrawShadowedString("[[]] Max     [^] Min ",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-35,JGETEXT_CENTER);
				gFont->DrawShadowedString("[X] Start Game     [O] Return to Menu     [<] [>] Change",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
			}
			else {
				gFont->SetColor(ARGB(255,255,255,255));
				gFont->SetScale(1.0f);
				gFont->DrawShadowedString("Time (Min:Sec)",150,90,JGETEXT_RIGHT);
				if(NGGameTime == 1) {
					gFont->DrawShadowedString("0:15",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 2) {
					gFont->DrawShadowedString("0:30",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 3) {
					gFont->DrawShadowedString("0:45",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 4) {
					gFont->DrawShadowedString("1:00",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 5) {
					gFont->DrawShadowedString("1:15",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 6) {
					gFont->DrawShadowedString("1:30",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 7) {
					gFont->DrawShadowedString("1:45",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 8) {
					gFont->DrawShadowedString("2:00",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 9) {
					gFont->DrawShadowedString("2:15",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 10) {
					gFont->DrawShadowedString("2:30",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 11) {
					gFont->DrawShadowedString("2:45",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 12) {
					gFont->DrawShadowedString("3:00",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 13) {
					gFont->DrawShadowedString("3:15",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 14) {
					gFont->DrawShadowedString("3:30",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 15) {
					gFont->DrawShadowedString("3:45",170,90,JGETEXT_LEFT);
				}
				if(NGGameTime == 16) {
					gFont->DrawShadowedString("4:00",170,90,JGETEXT_LEFT);
				}
			}
			/*
			if(NGmGame == NGMulti) {
				gFont->SetColor(ARGB(255,255,244,53));
				gFont->SetScale(1.0f);
				mRenderer->FillRect(0,110,SCREEN_WIDTH,25,ARGB(175,0,0,0));
				gFont->DrawShadowedString("Time Multiplier",150,115,JGETEXT_RIGHT);
				gFont->SetColor(ARGB(255,255,255,255));
				if(Multi == 1) {
					gFont->DrawShadowedString("x1",170,115,JGETEXT_LEFT);
				}
				if(Multi == 2) {
					gFont->DrawShadowedString("x2",170,115,JGETEXT_LEFT);
				}
				
				gFont->SetScale(0.75f);
				gFont->DrawShadowedString("[X] Start Game     [O] Return to Menu     [<] [>] Change",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
			}
			else {
				gFont->SetColor(ARGB(255,255,255,255));
				gFont->SetScale(1.0f);
				gFont->DrawShadowedString("Time Multiplier",150,116,JGETEXT_RIGHT);
				gFont->SetColor(ARGB(255,255,255,255));
				if(Multi == 1) {
					gFont->DrawShadowedString("x1",170,115,JGETEXT_LEFT);
				}
				if(Multi == 2) {
					gFont->DrawShadowedString("x2",170,115,JGETEXT_LEFT);
				}
			}
			*/
		}
		
		
		
		
		
		if(NGmPage == NGBots) {
			gFont->SetScale(1.0f);
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->DrawShadowedString("Game",135,35,JGETEXT_CENTER);
			gFont->SetColor(ARGB(255,255,244,53));
			gFont->DrawShadowedString("Bots",238,35,JGETEXT_CENTER);
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->DrawShadowedString("Rules",335,35,JGETEXT_CENTER);
			
			gFont->SetScale(0.75f);
			
			if (NGmBots == NGMaxBots) {
				gFont->SetColor(ARGB(255,255,244,53));
				mRenderer->FillRect(0,60,SCREEN_WIDTH,25,ARGB(175,0,0,0));
				gFont->SetScale(1.0f);
				gFont->DrawShadowedString("Bots",150,65,JGETEXT_RIGHT);
				gFont->SetColor(ARGB(255,255,255,255));
				if (MaxBots == -1) {
					gFont->DrawShadowedString("Default",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 0) {
					gFont->DrawShadowedString("0",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 1) {
					gFont->DrawShadowedString("1",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 2) {
					gFont->DrawShadowedString("3",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 3) {
					gFont->DrawShadowedString("5",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 4) {
					gFont->DrawShadowedString("7",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 5) {
					gFont->DrawShadowedString("9",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 6) {
					gFont->DrawShadowedString("11",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 7) {
					gFont->DrawShadowedString("13",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 8) {
					gFont->DrawShadowedString("15",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 9) {
					gFont->DrawShadowedString("17",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 10) {
					gFont->DrawShadowedString("19",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 11) {
					gFont->DrawShadowedString("21",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 12) {
					gFont->DrawShadowedString("23",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 13) {
					gFont->DrawShadowedString("25",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 14) {
					gFont->DrawShadowedString("27",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 15) {
					gFont->DrawShadowedString("29",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 16) {
					gFont->DrawShadowedString("31",170,65,JGETEXT_LEFT);
				}
				
				gFont->SetColor(ARGB(255,255,255,255));
				gFont->SetScale(0.75f);
				gFont->DrawShadowedString("[[]] Max     [^] Min ",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-35,JGETEXT_CENTER);
				gFont->DrawShadowedString("[X] Start Game     [O] Return to Menu     [<] [>] Change",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
			}
			else {
				gFont->SetColor(ARGB(255,255,255,255));
				gFont->SetScale(1.0f);
				gFont->DrawShadowedString("Bots",150,65,JGETEXT_RIGHT);
				gFont->SetColor(ARGB(255,255,255,255));
				if (MaxBots == -1) {
					gFont->DrawShadowedString("Default",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 0) {
					gFont->DrawShadowedString("0",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 1) {
					gFont->DrawShadowedString("1",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 2) {
					gFont->DrawShadowedString("3",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 3) {
					gFont->DrawShadowedString("5",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 4) {
					gFont->DrawShadowedString("7",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 5) {
					gFont->DrawShadowedString("9",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 6) {
					gFont->DrawShadowedString("11",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 7) {
					gFont->DrawShadowedString("13",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 8) {
					gFont->DrawShadowedString("15",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 9) {
					gFont->DrawShadowedString("17",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 10) {
					gFont->DrawShadowedString("19",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 11) {
					gFont->DrawShadowedString("21",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 12) {
					gFont->DrawShadowedString("23",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 13) {
					gFont->DrawShadowedString("25",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 14) {
					gFont->DrawShadowedString("27",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 15) {
					gFont->DrawShadowedString("29",170,65,JGETEXT_LEFT);
				}
				if (MaxBots == 16) {
					gFont->DrawShadowedString("31",170,65,JGETEXT_LEFT);
				}
			}
			
			
			
			
			
			if (NGmBots == NGAllowCustomName) {
				gFont->SetColor(ARGB(255,255,244,53));
				mRenderer->FillRect(0,85,SCREEN_WIDTH,25,ARGB(175,0,0,0));
				gFont->SetScale(1.0f);
				gFont->DrawShadowedString("Custom Nick",150,90,JGETEXT_RIGHT);
				gFont->SetColor(ARGB(255,255,255,255));
				if (CustomNames == true) {
					gFont->DrawShadowedString("Enabled",170,90,JGETEXT_LEFT);
				}
				if (CustomNames == false) {
					gFont->DrawShadowedString("Disabled",170,90,JGETEXT_LEFT);
				}
				
				gFont->SetColor(ARGB(255,255,255,255));
				gFont->SetScale(0.75f);
				gFont->DrawShadowedString("[X] Select & Play     [O] Return to Menu     [<] [>] Change",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
			}
			else {
				gFont->SetColor(ARGB(255,255,255,255));
				gFont->SetScale(1.0f);
				gFont->DrawShadowedString("Custom Nick",150,90,JGETEXT_RIGHT);
				gFont->SetColor(ARGB(255,255,255,255));
				if (CustomNames == true) {
					gFont->DrawShadowedString("Enabled",170,90,JGETEXT_LEFT);
				}
				if (CustomNames == false) {
					gFont->DrawShadowedString("Disabled",170,90,JGETEXT_LEFT);
				}
			}
			
			
			
			
			
			if (NGmBots == NGDifficulty) {
				gFont->SetColor(ARGB(255,255,244,53));
				/*
				if ( Later ) {
					mRenderer->FillRect(0,110,SCREEN_WIDTH,25,ARGB(175,0,0,0));
					gFont->SetScale(1.0f);
					gFont->DrawShadowedString("Custom Names",150,90,JGETEXT_RIGHT);
					gFont->SetColor(ARGB(255,255,255,255));
				}
				*/
			}
		}
		
		
		
		
		
		
		
		if(NGmPage == NGRules) {
			gFont->SetScale(1.0f);
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->DrawShadowedString("Game",135,35,JGETEXT_CENTER);
			gFont->SetColor(ARGB(255,255,255,255));
			gFont->DrawShadowedString("Bots",238,35,JGETEXT_CENTER);
			gFont->SetColor(ARGB(255,255,244,53));
			gFont->DrawShadowedString("Rules",335,35,JGETEXT_CENTER);
			
			
			//////////////////////////// R E G E N E R A T I O N === T R U E ////////////////////////////
			if (NGmRules = NGAllowRegen) {
				gFont->SetColor(ARGB(255,255,244,53));
				mRenderer->FillRect(0,85,SCREEN_WIDTH,25,ARGB(175,0,0,0));
				gFont->SetScale(1.0f);
				gFont->DrawShadowedString("Regeneration",150,65,JGETEXT_RIGHT);
				gFont->SetColor(ARGB(255,255,255,255));
				if (AllowRegeneration == true) {
					gFont->DrawShadowedString("Enabled",170,65,JGETEXT_LEFT);
				}
				if (AllowRegeneration == false) {
					gFont->DrawShadowedString("Disabled",170,65,JGETEXT_LEFT);
				}
				
				gFont->SetColor(ARGB(255,255,255,255));
				gFont->SetScale(0.75f);
				gFont->DrawShadowedString("[X] Select & Play     [O] Return to Menu     [<] [>] Switch",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
			}
			else {
				gFont->SetColor(ARGB(255,255,255,255));
				gFont->SetScale(1.0f);
				gFont->DrawShadowedString("Regeneration",150,65,JGETEXT_RIGHT);
				gFont->SetColor(ARGB(255,255,255,255));
				if (AllowRegeneration == true) {
					gFont->DrawShadowedString("Enabled",170,65,JGETEXT_LEFT);
				}
				if (AllowRegeneration == false) {
					gFont->DrawShadowedString("Disabled",170,65,JGETEXT_LEFT);
				}
			}
			
			// Other Regeneration Settings
				
			

			gFont->SetColor(ARGB(255,255,255,255));
			gFont->SetScale(0.75f);
			gFont->DrawShadowedString("[X] Select & Play     [O] Return to Menu     [<] [>] Move",SCREEN_WIDTH_2,SCREEN_HEIGHT_F-20,JGETEXT_CENTER);
		}
		
		// White Line to cross name and name's param.
		mRenderer->DrawLine(160,65,160,35 + 175 + 35,ARGB(255,255,255,255));
		
	} // NGmState == Set END
} // Render Function END