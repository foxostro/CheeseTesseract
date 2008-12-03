#include "stdafx.h"
#include "AnimationSequence.h"

AnimationSequence::AnimationSequence(const vector<KeyFrame> &_keyFrames,
                                     const string &name,
                                     float priority,
                                     bool looping,
                                     size_t start,
                                     size_t length,
                                     float _fps)
: m_strName(name),
  m_Priority(priority),
  m_Time(0.0f),
  m_TimeScalar(1.0f),
  m_bLooping(looping),
  fps(_fps),
  m_bFinished(false)
{
    keyFrames.reserve(length);
    for(size_t i=0; i<length; ++i)
        keyFrames.push_back(_keyFrames[start+i]);

	createWorkingSetOfMeshes(keyFrames[0].getMeshes());
}

AnimationSequence::AnimationSequence(const vector<KeyFrame> &_keyFrames,
                                     const string &name,
                                     float priority,
                                     bool looping,
                                     float _fps)
: keyFrames(_keyFrames),
  m_strName(name),
  m_Priority(priority),
  m_Time(0.0f),
  m_TimeScalar(1.0f),
  m_bLooping(looping),
  fps(_fps),
  m_bFinished(false)
{
    createWorkingSetOfMeshes(keyFrames[0].getMeshes());
}

AnimationSequence::AnimationSequence(const AnimationSequence &animation)
: keyFrames(animation.keyFrames),
  m_strName(animation.m_strName),
  m_Priority(animation.m_Priority),
  m_Time(animation.m_Time),
  m_TimeScalar(animation.m_TimeScalar),
  m_bLooping(animation.m_bLooping),
  fps(animation.fps),
  m_bFinished(animation.m_bFinished)
{
	createWorkingSetOfMeshes(animation.meshes);
}

AnimationSequence::~AnimationSequence()
{
	for(size_t i=0; i<meshes.size(); ++i) delete meshes[i];
	meshes.clear();
}

void AnimationSequence::createWorkingSetOfMeshes(const vector<Mesh*> &model)
{
    for(vector<Mesh*>::const_iterator i=model.begin();
        i!=model.end();
        ++i)
    {
        meshes.push_back(new Mesh(*i));
    }
}

void AnimationSequence::update(float millisecondsDelta)
{
	m_Time += millisecondsDelta * m_TimeScalar;

	// Loop the animation if it goes past the end
	if(m_Time > getLength())
	{
		if(m_bLooping == true)
		{
			while(m_Time > getLength()) m_Time -= getLength();
			m_bFinished = false;
		}
		else
		{
			m_bFinished = true;
			m_Time = getLength();
		}
	}
}

void AnimationSequence::setTime(float Time)
{
	m_Time = Time;
	m_bFinished = false;
}

float AnimationSequence::calculateRadius(function<float(Mesh *)> fn) const
{
	const vector<Mesh*> meshes = getFrame(0.0f);

	if(meshes.empty())
		return 0.0f;

    vector<float> top(meshes.size());
    transform(meshes.begin(), meshes.end(), top.begin(), fn);
	sort(top.begin(), top.end(), greater<float>());
	return top[0];
}

const vector<Mesh*>& AnimationSequence::getFrame(float milliseconds) const
{
	ASSERT(!keyFrames.empty(), "no keyframes in the animation sequence");
	ASSERT(milliseconds>=0.0f, "Time is before beginning of the animation");

	if(keyFrames.size() == 1)
		return(keyFrames[0].getMeshes());

	const float length = getLength();

	if(milliseconds > length)
		milliseconds = length;

	const float frameOfAnimation = (milliseconds / length) * (keyFrames.size() - 1);
	const size_t lowerFrame = (size_t)floor(frameOfAnimation);
	const size_t upperFrame = (size_t)ceil(frameOfAnimation);
	const float bias = frameOfAnimation - lowerFrame;

	return getFrame(lowerFrame, upperFrame, bias);
}

const vector<Mesh*>& AnimationSequence::getFrame(size_t lowerFrame, size_t upperFrame, float bias) const
{
	ASSERT(!keyFrames.empty(), "no keyframes in the animation sequence");

	if(keyFrames.size()==1)
		return keyFrames[0].getMeshes();

	ASSERT(lowerFrame < keyFrames.size(), "lower keyframe out of range:" + sizet_to_string(lowerFrame));
	ASSERT(upperFrame < keyFrames.size(), "upper keyframe out of range: "+ sizet_to_string(upperFrame));
	ASSERT(bias >= 0.0f && bias <= 1.0f, "bias is out of range: " + ftos(bias));

	if(lowerFrame == upperFrame)
		return(keyFrames[lowerFrame].getMeshes());

	const vector<Mesh*> &meshesA = keyFrames[lowerFrame].getMeshes();
	const vector<Mesh*> &meshesB = keyFrames[upperFrame].getMeshes();

	for(size_t i=0; i<meshes.size(); ++i)
	{
		Mesh*const mesh = meshes[i];
		const Mesh*const a = meshesA[i];
		const Mesh*const b = meshesB[i];

		interpolate(bias, *mesh, *a, *b);
	}

	return meshes;
}

float AnimationSequence::calculateHeight() const
{
	const vector<Mesh*> meshes = keyFrames[0].getMeshes();

	if(meshes.empty())
		return 0.0f;

	vector<float> top(meshes.size());
	transform(meshes.begin(), meshes.end(), top.begin(), bind(&Mesh::calculateHeight, _1));
	sort(top.begin(), top.end(), greater<float>());
	return top[0];
}

void AnimationSequence::getGeometryChunks( vector<GeometryChunk> &m ) const
{
	GeometryChunk gc;
	const vector<Mesh*> &frame = getFrame();

	m.reserve(frame.size());

	for(vector<Mesh*>::const_iterator i=frame.begin();
		i!=frame.end(); ++i)
	{
		(*i)->getGeometryChunk(gc);
		m.push_back(gc);
	}
}

void AnimationSequence::uniformScale( float scale )
{
	for(vector<KeyFrame>::iterator i=keyFrames.begin();
		i!=keyFrames.end(); ++i)
	{
		vector<Mesh*> &meshes = i->getMeshes();
		for(vector<Mesh*>::iterator j=meshes.begin(); j!=meshes.end();++j)
		{
			(*j)->uniformScale(scale);
		}
	}
}