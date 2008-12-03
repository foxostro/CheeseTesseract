#ifndef DEATH_BEHAVIOR_H
#define DEATH_BEHAVIOR_H

enum DeathBehavior
{
	Corpse,    /** Lay on the ground, dead */
	Ghost,     /** Able to walk around as a ghost; cannot interact */
	Disappear  /** Disappear entirely */
};

#endif
