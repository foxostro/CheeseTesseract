#ifndef SCORE_H
#define SCORE_H

#include "ScopedEventHandler.h"
#include "ActionChangeScore.h"

/** in-game score value */
class Score : public ScopedEventHandlerSubscriber {
public:

	/* -- Functions -- */
	/** Constructors */
	Score(int initScore, UID uid, ScopedEventHandler *parentBlackBoard);
	Score(UID uid, ScopedEventHandler *parentBlackBoard);
	
	/** Destructor */
	~Score(void);
	
	/**
	Updates any necessary scoring data.
	@param milliseconds the time since the last update
	*/
	void update(float milliseconds);
	
	/** Returns the current score */
	int getScore();
	
	/**
	Sets the number of recent changes to track (default is 5)
	@param t the number of changes to track
	*/
	void setTrackLen(unsigned int t);
	
	/**
	Sets the amount by which new changes to the score are multiplied.
	@param m the amount by which to multiply future scores
	@param t the time for which the multiplier is valid (0 for unlimited)
	*/
	void setMultiplier(float m, unsigned int t);
	
	/**
	Returns the current score multiplier.
	@return the current score multiplier
	*/
	float getMultiplier();
	
private:
	/* -- Functions -- */
	/** Callback handlers for messages */
	void handleActionChangeScore(const ActionChangeScore *action);
	
	/* -- Instance Variables -- */
	int score;					    /** the integral score */
	float multiplier;				/** the amount by which changes to the score are currently multiplied (only the + operation is supported */
	unsigned int multTTL;			/** the amount of time the multiplier is valid for (in milliseconds) */
	
	//miscellaneous stats
	unsigned int numMonstersKilled;	/** the number of monsters killed */
	unsigned int numCoinsCollected;	/** the number of coins collected */
	double timeToComplete;			/** the time taken to complete the current level (in seconds) */
};

#endif
