#include "stdafx.h"
#include "color.h"
#include "PhysicsEngine.h"
#include "EventCollisionOccurred.h"

static void _nearCallback(void *physicsEngine, dGeomID o1, dGeomID o2) {
	ASSERT(physicsEngine, "Parameter \"physicsEngine\" is null!");
	reinterpret_cast<PhysicsEngine*>(physicsEngine)->nearCallback(o1, o2);
}

static void _rayCallback(void *physicsEngine, dGeomID ray, dGeomID o) {
	ASSERT(physicsEngine, "Parameter \"physicsEngine\" is null!");
	reinterpret_cast<PhysicsEngine*>(physicsEngine)->rayCallback(ray, o);
}

void PhysicsEngine::rayCallback(dGeomID ray, dGeomID geom) {
	const int MAX_CONTACTS = 8;
	dContactGeom contacts[MAX_CONTACTS];
	for (int i=0; i<MAX_CONTACTS; ++i) {
		dContactGeom &contact = contacts[i];
		contact.depth = 0.0f;
		contact.g1 = 0;
		contact.g2 = 0;
		memset(&contact.normal, 0, sizeof(dVector3));
		memset(&contact.pos, 0, sizeof(dVector3));
		contact.side1 = 0;
		contact.side2 = 0;
	}
	
	int numCollisions = dCollide(ray,
	                             geom,
	                             MAX_CONTACTS,
	                             contacts,
	                             sizeof(dContactGeom));
	                             
	for (int i=0; i<numCollisions; ++i) {
		rayCastResults.push_back(contacts[i]);
	}
}

void PhysicsEngine::nearCallback(dGeomID o1, dGeomID o2) {
	// exit without doing anything if the two bodies are connected by a joint
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	
	if (b1 && b2 && dAreConnectedExcluding(b1,b2,dJointTypeContact)) {
		return;
	}
	
	const int MAX_CONTACTS = 32;
	dContact contact[MAX_CONTACTS];
	for (int i=0; i<MAX_CONTACTS; ++i) {
		contact[i].surface.mode = dContactBounce | dContactSoftCFM;
		contact[i].surface.mu = 500.0f;
		contact[i].surface.mu2 = 0.0f;
		contact[i].surface.bounce = 0.1f;
		contact[i].surface.bounce_vel = 0.1f;
		contact[i].surface.soft_cfm = 0.001f;
	}
	
	if (int numCollisions = dCollide(o1, o2,
	                                 MAX_CONTACTS,
	                                 &contact[0].geom,
	                                 sizeof(dContact))) {
		for (int i=0; i<numCollisions; ++i) {
			dJointID c = dJointCreateContact(getWorld(),
			                                 getContactGroup(),
			                                 &contact[i]);
			dJointAttach(c,b1,b2);
			
			passActorCollisionMessages(o1, o2, contact[i]);
		}
	}
}

PhysicsEngine::~PhysicsEngine() {
	dJointGroupDestroy(contactGroup);
	dSpaceDestroy(space);
	dWorldDestroy(world);
}

PhysicsEngine::PhysicsEngine(ActorSet *_actorSet)
		: actorSet(_actorSet),
		world(0),
		space(0),
		contactGroup(0) {
	world = dWorldCreate();
	space = dHashSpaceCreate(0);
	contactGroup = dJointGroupCreate(0);
	
	dWorldSetGravity(world, 0.0f, 0.0f, -9.81f);
	
	dCreatePlane(space, 0, 0, 1, 0); // catch spills
	
	ray = dCreateRay(0, 10.0f);
}

void PhysicsEngine::update(float deltaTime) {
	dSpaceCollide(space, this, _nearCallback);
	dWorldQuickStep(world, deltaTime/1000.0f);
	dJointGroupEmpty(contactGroup);
}

void PhysicsEngine::drawGeom(dGeomID geom) {
	CHECK_GL_ERROR();
	
	switch (dGeomGetClass(geom)) {
	case dSphereClass:
		drawGeomSphere(geom);
		break;
	case dBoxClass:
		drawGeomBox(geom);
		break;
	case dCCylinderClass:
		drawGeomCappedCylinder(geom);
		break;
	case dCylinderClass:
		drawGeomCylinder(geom);
		break;
	case dPlaneClass:
		drawGeomPlane(geom);
		break;
	case dRayClass:
		drawGeomRay(geom);
		break;
	case dTriMeshClass:
		drawGeomTriMesh(geom);
		break;
	case dGeomTransformClass: /* ignore */
		break;
	case dSimpleSpaceClass:   /* ignore */
		break;
	case dHashSpaceClass:     /* ignore */
		break;
	default:                  /* ignore */
		break;
	};
	
	CHECK_GL_ERROR();
}

void PhysicsEngine::drawGeomSphere(dGeomID geom) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor4fv(white);
	
	const mat3 orientation = getOrientation(geom);
	mat4 transformation(getPosition(geom),
	                    orientation.getAxisX(),
	                    orientation.getAxisY(),
	                    orientation.getAxisZ());
	glPushMatrix();
	glMultMatrixf(transformation);
	
	const float sphereRadius = (float)dGeomSphereGetRadius(geom);
	glScalef(sphereRadius, sphereRadius, sphereRadius);
	drawSphere();
	
	glPopMatrix();
	glPopAttrib();
}

void PhysicsEngine::drawGeomBox(dGeomID geom) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor4fv(white);
	
	const mat3 orientation = getOrientation(geom);
	mat4 transformation(getPosition(geom),
	                    orientation.getAxisX(),
	                    orientation.getAxisY(),
	                    orientation.getAxisZ());
	glPushMatrix();
	glMultMatrixf(transformation);
	
	dVector3 sides;
	dGeomBoxGetLengths(geom, sides);
	drawBox(sides);
	
	glPopMatrix();
	glPopAttrib();
}

void PhysicsEngine::drawGeomPlane(dGeomID) {
	/* ignore */
}

void PhysicsEngine::drawGeomRay(dGeomID) {
	/* ignore */
}

void PhysicsEngine::drawGeomCappedCylinder(dGeomID geom) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor4fv(white);
	
	const mat3 orientation = getOrientation(geom);
	mat4 transformation(getPosition(geom),
	                    orientation.getAxisX(),
	                    orientation.getAxisY(),
	                    orientation.getAxisZ());
	glPushMatrix();
	glMultMatrixf(transformation);
	
	dReal radius=0, length=0;
	dGeomCCylinderGetParams(geom, &radius, &length);
	drawCapsule((float)length, (float)radius);
	
	glPopMatrix();
	glPopAttrib();
}

void PhysicsEngine::drawGeomCylinder(dGeomID geom) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor4fv(white);
	
	const mat3 orientation = getOrientation(geom);
	mat4 transformation(getPosition(geom),
	                    orientation.getAxisX(),
	                    orientation.getAxisY(),
	                    orientation.getAxisZ());
	glPushMatrix();
	glMultMatrixf(transformation);
	
	dReal radius=0, length=0;
	dGeomCylinderGetParams(geom, &radius, &length);
	drawCylinder((float)length, (float)radius, 0.0f);
	
	glPopMatrix();
	glPopAttrib();
}

void PhysicsEngine::drawGeomTriMesh(dGeomID geom) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor4fv(white * 0.5f);
	
	const mat3 orientation = getOrientation(geom);
	mat4 transformation(getPosition(geom),
	                    orientation.getAxisX(),
	                    orientation.getAxisY(),
	                    orientation.getAxisZ());
	glPushMatrix();
	glMultMatrixf(transformation);
	
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(1.0f, 1.0f - FLT_EPSILON);
	
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	
	const int numOfVertices = dGeomTriMeshGetTriangleCount(geom); //63*63*2
	for (int i = 0; i < numOfVertices; ++i) {
		dVector3 v1, v2, v3;
		
		dGeomTriMeshGetTriangle(geom, i, &v1, &v2, &v3);
		
		glVertex3f((float)v1[0], (float)v1[1], (float)v1[2]);
		glVertex3f((float)v2[0], (float)v2[1], (float)v2[2]);
		
		glVertex3f((float)v2[0], (float)v2[1], (float)v2[2]);
		glVertex3f((float)v3[0], (float)v3[1], (float)v3[2]);
		
		glVertex3f((float)v3[0], (float)v3[1], (float)v3[2]);
		glVertex3f((float)v1[0], (float)v1[1], (float)v1[2]);
	}
	glEnd();
	
	glPopMatrix();
	glPopAttrib();
}

void PhysicsEngine::drawGeomAABB(dGeomID geom) {
	CHECK_GL_ERROR();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glColor4fv(cyan);
	
	dReal aabb[6];
	dGeomGetAABB(geom, aabb);
	
	dVector3 pos;
	for (int i=0; i<3; i++) {
		pos[i] = REAL(0.5) * (aabb[i*2] + aabb[i*2+1]);
	}
	
	dVector3 sides;
	for (int i=0; i<3; i++) {
		sides[i] = aabb[i*2+1] - aabb[i*2];
	}
	
	glPushMatrix();
	glTranslatef((float)pos[0], (float)pos[1], (float)pos[2]);
	drawBox(sides);
	glPopMatrix();
	
	glPopAttrib();
	CHECK_GL_ERROR();
}

vec3 PhysicsEngine::getPosition(dGeomID geom) {
	const dReal* p = dGeomGetPosition(geom);
	return vec3((float)p[0], (float)p[1], (float)p[2]);
}

mat3 PhysicsEngine::getOrientation(dGeomID geom) {
	const dReal* r = dGeomGetRotation(geom);
	return mat3(vec3((float)r[0], (float)r[1], (float)r[2]),
	            vec3((float)r[4], (float)r[5], (float)r[6]),
	            vec3((float)r[8], (float)r[9], (float)r[10]));
}

void PhysicsEngine::drawBox(dVector3 sides) {
	const float lx = (float)sides[0] * 0.5f;
	const float ly = (float)sides[1] * 0.5f;
	const float lz = (float)sides[2] * 0.5f;
	
	// sides
	glBegin (GL_TRIANGLE_STRIP);
	glNormal3f (-1,0,0);
	glVertex3f (-lx,-ly,-lz);
	glVertex3f (-lx,-ly,lz);
	glVertex3f (-lx,ly,-lz);
	glVertex3f (-lx,ly,lz);
	glNormal3f (0,1,0);
	glVertex3f (lx,ly,-lz);
	glVertex3f (lx,ly,lz);
	glNormal3f (1,0,0);
	glVertex3f (lx,-ly,-lz);
	glVertex3f (lx,-ly,lz);
	glNormal3f (0,-1,0);
	glVertex3f (-lx,-ly,-lz);
	glVertex3f (-lx,-ly,lz);
	glEnd();
	
	// top face
	glBegin (GL_TRIANGLE_FAN);
	glNormal3f (0,0,1);
	glVertex3f (-lx,-ly,lz);
	glVertex3f (lx,-ly,lz);
	glVertex3f (lx,ly,lz);
	glVertex3f (-lx,ly,lz);
	glEnd();
	
	// bottom face
	glBegin (GL_TRIANGLE_FAN);
	glNormal3f (0,0,-1);
	glVertex3f (-lx,-ly,-lz);
	glVertex3f (-lx,ly,-lz);
	glVertex3f (lx,ly,-lz);
	glVertex3f (lx,-ly,-lz);
	glEnd();
}

void PhysicsEngine::drawPatch(float p1[3], float p2[3], float p3[3], int level) {
	if (level > 0) {
		float q1[3],q2[3],q3[3];		 // sub-vertices
		for (int i=0; i<3; ++i) {
			q1[i] = 0.5f*(p1[i]+p2[i]);
			q2[i] = 0.5f*(p2[i]+p3[i]);
			q3[i] = 0.5f*(p3[i]+p1[i]);
		}
		
		float length1 = (float)(1.0/sqrt(q1[0]*q1[0]+q1[1]*q1[1]+q1[2]*q1[2]));
		float length2 = (float)(1.0/sqrt(q2[0]*q2[0]+q2[1]*q2[1]+q2[2]*q2[2]));
		float length3 = (float)(1.0/sqrt(q3[0]*q3[0]+q3[1]*q3[1]+q3[2]*q3[2]));
		
		for (int i=0; i<3; ++i) {
			q1[i] *= length1;
			q2[i] *= length2;
			q3[i] *= length3;
		}
		
		drawPatch(p1,q1,q3,level-1);
		drawPatch(q1,p2,q2,level-1);
		drawPatch(q1,q2,q3,level-1);
		drawPatch(q3,q2,p3,level-1);
	} else {
		glNormal3f(p1[0],p1[1],p1[2]);
		glVertex3f(p1[0],p1[1],p1[2]);
		glNormal3f(p2[0],p2[1],p2[2]);
		glVertex3f(p2[0],p2[1],p2[2]);
		glNormal3f(p3[0],p3[1],p3[2]);
		glVertex3f(p3[0],p3[1],p3[2]);
	}
}

void PhysicsEngine::drawSphere() {
	static int sphere_quality = 1;
	
	// icosahedron data for an icosahedron of radius 1.0
	const float ICX = 0.525731112119133606f;
	const float ICZ = 0.850650808352039932f;
	static GLfloat idata[12][3] = {
		{-ICX, 0, ICZ},
		{ICX, 0, ICZ},
		{-ICX, 0, -ICZ},
		{ICX, 0, -ICZ},
		{0, ICZ, ICX},
		{0, ICZ, -ICX},
		{0, -ICZ, ICX},
		{0, -ICZ, -ICX},
		{ICZ, ICX, 0},
		{-ICZ, ICX, 0},
		{ICZ, -ICX, 0},
		{-ICZ, -ICX, 0}
	};
	
	static int index[20][3] = {
		{0, 4, 1},	  {0, 9, 4},
		{9, 5, 4},	  {4, 5, 8},
		{4, 8, 1},	  {8, 10, 1},
		{8, 3, 10},   {5, 3, 8},
		{5, 2, 3},	  {2, 7, 3},
		{7, 10, 3},   {7, 6, 10},
		{7, 11, 6},   {11, 0, 6},
		{0, 1, 6},	  {6, 1, 10},
		{9, 0, 11},   {9, 11, 2},
		{9, 2, 5},	  {7, 2, 11},
	};
	
	static GLuint listnum = 0;
	if (listnum==0) {
		listnum = glGenLists(1);
		glNewList(listnum,GL_COMPILE);
		glBegin(GL_TRIANGLES);
		for (int i=0; i<20; i++) {
			drawPatch(&idata[index[i][2]][0],
			          &idata[index[i][1]][0],
			          &idata[index[i][0]][0],
			          sphere_quality);
		}
		glEnd();
		glEndList();
	}
	glCallList(listnum);
}

void PhysicsEngine::drawCapsule(float length, float radius) {
	CHECK_GL_ERROR();
	
	static int capped_cylinder_quality = 3;
	
	int i,j;
	float tmp,nx,ny,nz,start_nx,start_ny,a,ca,sa;
	// number of sides to the cylinder (divisible by 4):
	const int n = capped_cylinder_quality*4;
	
	length *= 0.5;
	a = float(M_PI*2.0)/float(n);
	sa = (float) sin(a);
	ca = (float) cos(a);
	
	// draw cylinder body
	ny=1;
	nz=0;		  // normal vector = (0,ny,nz)
	glBegin (GL_TRIANGLE_STRIP);
	for (i=0; i<=n; i++) {
		glNormal3d (ny,nz,0);
		glVertex3d (ny*radius,nz*radius,length);
		glNormal3d (ny,nz,0);
		glVertex3d (ny*radius,nz*radius,-length);
		// rotate ny,nz
		tmp = ca*ny - sa*nz;
		nz = sa*ny + ca*nz;
		ny = tmp;
	}
	glEnd();
	
	// draw first cylinder cap
	start_nx = 0;
	start_ny = 1;
	for (j=0; j<(n/4); j++) {
		// get start_n2 = rotated start_n
		float start_nx2 =  ca*start_nx + sa*start_ny;
		float start_ny2 = -sa*start_nx + ca*start_ny;
		// get n=start_n and n2=start_n2
		nx = start_nx;
		ny = start_ny;
		nz = 0;
		float nx2 = start_nx2, ny2 = start_ny2, nz2 = 0;
		glBegin (GL_TRIANGLE_STRIP);
		for (i=0; i<=n; i++) {
			glNormal3d (ny2,nz2,nx2);
			glVertex3d (ny2*radius,nz2*radius,length+nx2*radius);
			glNormal3d (ny,nz,nx);
			glVertex3d (ny*radius,nz*radius,length+nx*radius);
			// rotate n,n2
			tmp = ca*ny - sa*nz;
			nz = sa*ny + ca*nz;
			ny = tmp;
			tmp = ca*ny2- sa*nz2;
			nz2 = sa*ny2 + ca*nz2;
			ny2 = tmp;
		}
		glEnd();
		start_nx = start_nx2;
		start_ny = start_ny2;
	}
	
	// draw second cylinder cap
	start_nx = 0;
	start_ny = 1;
	for (j=0; j<(n/4); j++) {
		// get start_n2 = rotated start_n
		float start_nx2 = ca*start_nx - sa*start_ny;
		float start_ny2 = sa*start_nx + ca*start_ny;
		// get n=start_n and n2=start_n2
		nx = start_nx;
		ny = start_ny;
		nz = 0;
		float nx2 = start_nx2, ny2 = start_ny2, nz2 = 0;
		glBegin (GL_TRIANGLE_STRIP);
		for (i=0; i<=n; i++) {
			glNormal3d (ny,nz,nx);
			glVertex3d (ny*radius,nz*radius,-length+nx*radius);
			glNormal3d (ny2,nz2,nx2);
			glVertex3d (ny2*radius,nz2*radius,-length+nx2*radius);
			// rotate n,n2
			tmp = ca*ny - sa*nz;
			nz = sa*ny + ca*nz;
			ny = tmp;
			tmp = ca*ny2- sa*nz2;
			nz2 = sa*ny2 + ca*nz2;
			ny2 = tmp;
		}
		glEnd();
		start_nx = start_nx2;
		start_ny = start_ny2;
	}
	CHECK_GL_ERROR();
}

void PhysicsEngine::drawCylinder(float l, float r, float zoffset) {
	int i;
	float tmp,ny,nz,a,ca,sa;
	const int n = 24;	// number of sides to the cylinder (divisible by 4)
	
	l *= 0.5;
	a = float(M_PI*2.0)/float(n);
	sa = (float) sin(a);
	ca = (float) cos(a);
	
	// draw cylinder body
	ny=1;
	nz=0;		  // normal vector = (0,ny,nz)
	glBegin (GL_TRIANGLE_STRIP);
	for (i=0; i<=n; i++) {
		glNormal3d (ny,nz,0);
		glVertex3d (ny*r,nz*r,l+zoffset);
		glNormal3d (ny,nz,0);
		glVertex3d (ny*r,nz*r,-l+zoffset);
		// rotate ny,nz
		tmp = ca*ny - sa*nz;
		nz = sa*ny + ca*nz;
		ny = tmp;
	}
	glEnd();
	
	// draw top cap
	glShadeModel (GL_FLAT);
	ny=1;
	nz=0;		  // normal vector = (0,ny,nz)
	glBegin (GL_TRIANGLE_FAN);
	glNormal3d (0,0,1);
	glVertex3d (0,0,l+zoffset);
	for (i=0; i<=n; i++) {
		glNormal3d (0,0,1);
		glVertex3d (ny*r,nz*r,l+zoffset);
		
		// rotate ny,nz
		tmp = ca*ny - sa*nz;
		nz = sa*ny + ca*nz;
		ny = tmp;
	}
	glEnd();
	
	// draw bottom cap
	ny=1;
	nz=0;		  // normal vector = (0,ny,nz)
	glBegin (GL_TRIANGLE_FAN);
	glNormal3d (0,0,-1);
	glVertex3d (0,0,-l+zoffset);
	for (i=0; i<=n; i++) {
		glNormal3d (0,0,-1);
		glVertex3d (ny*r,nz*r,-l+zoffset);
		
		// rotate ny,nz
		tmp = ca*ny + sa*nz;
		nz = -sa*ny + ca*nz;
		ny = tmp;
	}
	glEnd();
}

vector<dContactGeom> PhysicsEngine::rayCast(const vec3 &p,
  const vec3 &d,
  float length) {
	dGeomRaySetLength(ray, (dReal)length);
	dGeomRaySet(ray, p.x, p.y, p.z, d.x, d.y, d.z);
	rayCastResults.clear();
	dSpaceCollide2(ray, (dGeomID)space, this, _rayCallback);
	return rayCastResults;
}

bool PhysicsEngine::rayCast(dGeomID geom,
                            dGeomID ignore,
                            const vec3 &p,
                            const vec3 &d,
                            float length) {
	vector<dContactGeom> results = rayCast(p, d, length);
	
	float closest_distance = length*2;
	dGeomID closest_geom = 0;
	
	for (size_t i=0; i<results.size(); ++i) {
		dContactGeom &contact = results[i];
		
		if (contact.depth < closest_distance) {
			ASSERT(contact.g1==ray || contact.g2==ray,
			       "Neither ray cast contact in this pair was actually the ray itself");
			       
			if (contact.g1 == ray  && contact.g2 != ignore) {
				closest_geom = contact.g2;
				closest_distance = (float)contact.depth;
			} else if (contact.g2 == ray  && contact.g1 != ignore) {
				closest_geom = contact.g1;
				closest_distance = (float)contact.depth;
			}
		}
	}
	
	return geom == closest_geom;
}

bool PhysicsEngine::rayCast( const vec3 &p, const vec3 &d, float length, dGeomID potentialCollider, dContactGeom &contact ) {
	dGeomRaySetLength(ray, (dReal)length);
	dGeomRaySet(ray, p.x, p.y, p.z, d.x, d.y, d.z);
	int num = dCollide(ray, potentialCollider, 1, &contact, sizeof(dContactGeom));
	return num != 0;
}

void PhysicsEngine::passActorCollisionMessages(dGeomID o1,
  dGeomID o2,
  dContact contact) const {
	ASSERT(actorSet, "actorSet set was null");
	
	ActorID a1id = PhysicsEngine::getActorFromGeom(o1);
	ActorID a2id = PhysicsEngine::getActorFromGeom(o2);
	
	if (actorSet->isMember(a1id)) {
		passActorCollisionMessages(actorSet->get(a1id), o1, o2, contact);
	}
	
	if (actorSet->isMember(a2id)) {
		passActorCollisionMessages(actorSet->get(a2id), o2, o1, contact);
	}
}

void PhysicsEngine::passActorCollisionMessages(ActorPtr actor,
  dGeomID o1,
  dGeomID o2,
  dContact contact) const {
	ASSERT(actor, "Parameter \"actor\" was null");
	EventCollisionOccurred m(o1, o2, contact);
	actor->recvMessage(&m);
}

void PhysicsEngine::handleCollisionCallBacks(dGeomID o1,
  dGeomID o2,
  dContact contact) const {
	CollisionCallBackMap::const_iterator i = collisionCallBacks.find(o1);
	CollisionCallBackMap::const_iterator j = collisionCallBacks.find(o2);
	
	if (i != collisionCallBacks.end()) {
		CollisionFn fn = i->second;
		fn(o2, contact);
	}
	
	if (j != collisionCallBacks.end()) {
		CollisionFn fn = j->second;
		fn(o1, contact);
	}
}

ActorID PhysicsEngine::getActorFromGeom(dGeomID geom) {
	ActorID id = INVALID_ID;
	
	void *data = dGeomGetData(geom);
	
	if (data) {
		id = *reinterpret_cast<ActorID*>(data);
	}
	
	return id;
}

void PhysicsEngine::draw() const {
	int numOfGeoms = dSpaceGetNumGeoms(getSpace());
	
	for (int i=0; i<numOfGeoms; ++i) {
		drawGeom(dSpaceGetGeom(getSpace(), i));
	}
}
