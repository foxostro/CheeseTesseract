#include "stdafx.h"
#include "World.h"
#include "ActorSet.h"
#include "PropertyBag.h"
#include "ComponentHealth.h"
#include "ComponentPhysics.h"
#include "ComponentMonsterSpawn.h"

ComponentMonsterSpawn::
ComponentMonsterSpawn(UID uid, ScopedEventHandler *parentScope) 
: Component(uid, parentScope)
{
	resetMembers();

	REGISTER_HANDLER(ComponentMonsterSpawn::handleMessagePassWorld);
	REGISTER_HANDLER(ComponentMonsterSpawn::handleEventCharacterHasDied);
	REGISTER_HANDLER(ComponentMonsterSpawn::handleEventApproachActor);
	REGISTER_HANDLER(ComponentMonsterSpawn::handleEventRecedesFromActor);
	REGISTER_HANDLER(ComponentMonsterSpawn::handleEventPositionUpdate);
	REGISTER_HANDLER(ComponentMonsterSpawn::handleEventRadiusUpdate);
}

ComponentMonsterSpawn::~ComponentMonsterSpawn()
{
	// Do Nothing
}

void ComponentMonsterSpawn::resetMembers()
{
	timeSinceLastSpawn = 0.0;
	totalSpawned = 0;
	spawnRate = 0.0;
	maxSpawnNum = 0;
	maxSimulNum = 0;
	bKillChildrenOnDeath = false;
	bSpawnProximityOnly = false;
	bPlayerInProximity = false;
	bIsOn = false;
	spawnInterval = 0.0;
	children.clear();
	lastReportedPosition.zero();
	lastReportedRadius = 1.0f;
}

void ComponentMonsterSpawn::draw() const
{
	/* Does nothing */
}

void ComponentMonsterSpawn::load(const PropertyBag &data)
{
	//clear data before loading
	resetMembers();

	//load appropriate data
	spawnRate = data.getFloat("spawnRate");
	//typeToSpawn = getMonsterTypeFromString(data.getString("typeToSpawn"));
	maxSpawnNum = data.getInt("maxSpawnNum");
	maxSimulNum = data.getInt("maxSimulNum");
	bKillChildrenOnDeath = data.getBool("bKillChildrenOnDeath");
	bSpawnProximityOnly = data.getBool("bSpawnProximityOnly");
	bIsOn = data.getBool("bIsOn");
	templateFile = data.getFileName("monsterTemplateFile");

	//calculate spawn interval (in milliseconds) and set time
	spawnInterval = (60000.0f) / spawnRate;
	timeSinceLastSpawn = 0.0f;
}

void ComponentMonsterSpawn::update(float milliseconds)
{
	timeSinceLastSpawn += milliseconds;

	removeDeadChildren();

	if(timeSinceLastSpawn >= spawnInterval && spawnOK())
	{
		timeSinceLastSpawn = 0.0f;
		children.insert(children.begin(), spawnMonster());
	}
}

bool ComponentMonsterSpawn::spawnOK()
{
	if (maxSpawnNum>=0 && totalSpawned >= maxSpawnNum)
	{
		// lifetime spawn limit reached
		return false;
	}
	else if (children.size() >= (unsigned int)maxSimulNum && maxSimulNum >= 0)
	{
		// max simultaneous monsters reached
		return false;
	}
	else if (bSpawnProximityOnly)
	{
		return bIsOn && bPlayerInProximity;	//check for player proximity
	}	
	else
	{
		return bIsOn;
	}
}

ActorPtr ComponentMonsterSpawn::spawnMonster()
{
	return spawnMonster(loadActorDefinition(templateFile),
	                    generateSpawnLocation());
}

ActorPtr ComponentMonsterSpawn::spawnMonster(const ComponentDataSet &s,
											 const vec3 &spawnLoc)
{
	ASSERT(world, "world has not been set yet");
	const tuple<ActorID, ActorPtr> t = world->getObjects().create();
	ActorPtr newMonster = t.get<1>();
	ASSERT(newMonster, "Failed to create monster actor");
	newMonster->load(s, spawnLoc, vec3(0.0, 0.0, 0.0), world);

	if(isDebugDisplayEnabled())
	{
		ActionDebugEnable m;
		newMonster->recvMessage(&m);
	}
	else
	{
		ActionDebugDisable m;
		newMonster->recvMessage(&m);
	}

	totalSpawned++;
	return newMonster;
}

void ComponentMonsterSpawn::handleEventCharacterHasDied(const EventCharacterHasDied *)
{
	//if spawn point is set to destroy all children on its death,
	//iterate through list of children and kill them all, then clear list
	if (bKillChildrenOnDeath)
	{
		list<ActorPtr>::iterator iter;
		for (iter = children.begin(); iter != children.end(); iter++)
		{
			EventCharacterHasDied m;
			(*iter)->recvEvent(&m);
		}
	}

	children.clear();
	bIsOn = false;	//turn off spawn point on death
}

void ComponentMonsterSpawn::handleEventApproachActor(const EventApproachActor*)
{
	bPlayerInProximity = true;
}

void ComponentMonsterSpawn::handleEventRecedesFromActor(const EventRecedesFromActor*)
{
	bPlayerInProximity = false;
}

void ComponentMonsterSpawn::removeDeadChildren()
{
	/*
	list<ActorPtr>::iterator iter = children.begin();
	while(iter != children.end())
	{
		ActorPtr actor = *iter;
		Component *c = actor->getComponent("Health");
		ComponentHealth *h = dynamic_cast<ComponentHealth*>(c);

		if(!h || (h && h->isDead()))
		{
			iter = children.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	*/
}

ComponentDataSet ComponentMonsterSpawn::loadActorDefinition( const FileName &file )
{
	const PropertyBag templateData = PropertyBag::fromFile(file);
	const PropertyBag base = templateData.getBag("components");
	return ComponentDataSet::parse(base);
}

vec3 ComponentMonsterSpawn::generateSpawnLocation() const
{
	float height = 0.0f;

	float bufferZone = 1.5f;
	float minRadius = lastReportedRadius * bufferZone;
	float maxRadius = minRadius*2.0f;

	float radius = FRAND_RANGE(minRadius, maxRadius);
	float angle = FRAND_RANGE(0.0f, (float)M_PI / 2.0f);
	vec2 offset = vec2(radius * cosf(angle), radius * sinf(angle));	

	vec2 p = offset + lastReportedPosition.xy();

	return vec3(p, 0.2f);
}

void ComponentMonsterSpawn::handleMessagePassWorld( const MessagePassWorld *message )
{
	world = message->world;
}

void ComponentMonsterSpawn::handleEventPositionUpdate( const EventPositionUpdate *message )
{
	lastReportedPosition = message->position;
}

void ComponentMonsterSpawn::handleEventRadiusUpdate( const EventRadiusUpdate *message )
{
	lastReportedRadius = message->radius;
}
