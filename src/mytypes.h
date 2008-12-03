#ifndef _MYTYPES_H_
#define _MYTYPES_H_

/** Signed eight-bit character */
typedef signed char S8;
BOOST_STATIC_ASSERT(sizeof(S8)==1);

/** Signed 16-bit integer */
typedef signed short int S16;
BOOST_STATIC_ASSERT(sizeof(S16)==2);

/** Signed 32-bit integer */
typedef signed int S32;
BOOST_STATIC_ASSERT(sizeof(S32)==4);



/** Unsigned eight-bit character */
typedef unsigned char U8;
BOOST_STATIC_ASSERT(sizeof(U8)==1);

/** Unsigned 16-bit integer */
typedef unsigned short int U16;
BOOST_STATIC_ASSERT(sizeof(U16)==2);

/** Unsigned 32-bit integer */
typedef unsigned int U32;
BOOST_STATIC_ASSERT(sizeof(U32)==4);



/** IEEE-754 floating point value */
typedef float F32;
BOOST_STATIC_ASSERT(sizeof(F32)==4);



/** Triple of floating point values */
#pragma pack(4)
struct VEC3
{
	F32 x, y, z;
};
#pragma pack()
BOOST_STATIC_ASSERT(sizeof(struct VEC3)==12);

/** Unique identifier for an object within some set */
typedef int UID;

/** Unique identifier for an Actor */
typedef UID ActorID;

/** Special value for an actor uid that indicates the uid is invalid */
const ActorID INVALID_ID = 0;

#endif
