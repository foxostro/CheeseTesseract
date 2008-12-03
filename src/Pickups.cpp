#include "stdafx.h"
#include "World.h"
#include "ActionPlaySound.h"
#include "Pickups.h"

PickupItem::PickupItem( World *_world ) : world(_world)
{
	ASSERT(world, "Null member: world");
}

void PickupItem::playSound( const FileName &fileName )
{
	ActionPlaySound m(fileName);
	getWorld().sendGlobalAction(&m);
}

void PickupItem::sendMessage( ActorID id, Event *event )
{
	ActorPtr actor = getWorld().getObjects().get(id);
	actor->recvMessage(event);
}

void PickupItem::spawnParticleSystem(const FileName &fileName,
									 const vec3 &position)
{
	TextureFactory &textureFactory = getWorld().getTextureFactory();
	
	getWorld().particleEngine->add(fileName,
	                               position,
						 		   0.0f,
								   textureFactory);
}
