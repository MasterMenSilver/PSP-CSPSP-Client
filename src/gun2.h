#ifndef _GUN_H_
#define _GUN_H_

#define PRIMARY 0
#define SECONDARY 1
#define KNIFE 2
#define BOMB 4

#include "JSoundSystem.h"
//------------------------------------------------------------------------------------------------

class Gun
{
private:
	static JSoundSystem* mSoundSystem;
protected:

public:
	JQuad* mHandQuad;
	JQuad* mGroundQuad;
	int mId;
	int mDelay;
	int mDamage;
	float mSpread;
	int mClip;
	int mNumClips;
	int mReloadDelay;
	float mSpeed;
	//int mHasSilencer;
	int mCost;
	int mType;
	char mName[15];
	JSample* mFireSound;
	JSample* mReloadSound;
	JSample* mDryFireSound;

public:

	Gun(JQuad *handquad, JQuad *groundquad, int id, int delay, int damage, float spread, int clip, int numclips, int reloaddelay, float speed, /*int hassilencer */ int cost, int type, char* name);
	~Gun();

	void SetQuad(JQuad *quad);
};
#endif
