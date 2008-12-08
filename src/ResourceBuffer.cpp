#include "stdafx.h"
#include "ResourceBuffer.h"

template<typename ELEMENT>
ResourceBuffer<ELEMENT>::~ResourceBuffer() {
	glDeleteBuffers(1, &handle);
	delete [] buffer;
}

template<typename ELEMENT>
ResourceBuffer<ELEMENT>::ResourceBuffer()
		: locked(false),
		numElements(0),
		buffer(0),
		handle(0),
		usage(STREAM_DRAW) {
	// Do Nothing
}

template<typename ELEMENT>
ResourceBuffer<ELEMENT>::ResourceBuffer(int numElements,
                                        const ELEMENT *buffer)
		: locked(false),
		numElements(0),
		buffer(0),
		handle(0),
		usage(STREAM_DRAW) {
	recreate(numElements, buffer, STREAM_DRAW);
}

template<typename ELEMENT>
ResourceBuffer<ELEMENT>::ResourceBuffer(const ResourceBuffer &copyMe)
		: locked(false),
		numElements(0),
		buffer(0),
		handle(0),
		usage(STREAM_DRAW) {
	recreate(copyMe.numElements, copyMe.buffer, copyMe.usage);
}

template<typename ELEMENT>
shared_ptr< ResourceBuffer<ELEMENT> > ResourceBuffer<ELEMENT>::clone() const {
	ResourceBuffer<ELEMENT> *ptr = new ResourceBuffer<ELEMENT>(*this);
	shared_ptr< ResourceBuffer<ELEMENT> > buffer(ptr);
	return buffer;
}

template<typename ELEMENT>
void ResourceBuffer<ELEMENT>::recreate(int numElements,
                                       const ELEMENT *buffer,
                                       BUFFER_USAGE usage) {
	ASSERT(!locked, "Cannot realloc a locked buffer!");
	ASSERT(numElements>=0, "Parameter \'numElements\' < 0");
	
	this->usage = usage;
	
	create_cpu_buffer(numElements, buffer);
	create_gpu_buffer(numElements, buffer, getGLUsageToken(usage));
}

template<typename ELEMENT>
int ResourceBuffer<ELEMENT>::getNumber() const {
	return numElements;
}

template<typename ELEMENT>
void ResourceBuffer<ELEMENT>::bind() const {
	ASSERT(!locked, "Cannot bind buffer for use when the buffer is locked!");
	CHECK_GL_ERROR();
	glBindBuffer(getTarget(), handle);
	CHECK_GL_ERROR();
}

template<typename ELEMENT>
void* ResourceBuffer<ELEMENT>::lock() {
	void *mapped_buffer=0;
	
	ASSERT(!locked, "Cannot lock a buffer that is already locked!");
	locked=true;
	
	CHECK_GL_ERROR();
	glBindBuffer(getTarget(), handle);
	mapped_buffer = glMapBuffer(getTarget(), GL_READ_WRITE);
	CHECK_GL_ERROR();
	
	return mapped_buffer;
}

template<typename ELEMENT>
void* ResourceBuffer<ELEMENT>::read_lock() {
	void *mapped_buffer=0;
	
	ASSERT(!locked, "Cannot lock a buffer that is already locked!");
	locked=true;
	
	CHECK_GL_ERROR();
	glBindBuffer(getTarget(), handle);
	mapped_buffer = glMapBuffer(getTarget(), GL_READ_ONLY);
	CHECK_GL_ERROR();
	
	return mapped_buffer;
}

template<typename ELEMENT>
void ResourceBuffer<ELEMENT>::unlock() const {
	ASSERT(locked, "Cannot unlock a buffer that is not locked!");
	locked=false;
	
	CHECK_GL_ERROR();
	glBindBuffer(getTarget(), handle);
	glUnmapBuffer(getTarget());
	CHECK_GL_ERROR();
}

template<typename ELEMENT>
void ResourceBuffer<ELEMENT>::create_cpu_buffer( int numElements, const ELEMENT * buffer ) {
	delete [] (this->buffer);
	this->buffer = 0;
	this->numElements = numElements;
	
	if (numElements>0) {
		this->buffer = new ELEMENT[numElements];
		this->numElements = numElements;
		
		if (buffer != 0) {
			memcpy(this->buffer, buffer, sizeof(ELEMENT)*numElements);
		} else {
			memset(this->buffer, 0, sizeof(ELEMENT)*numElements);
		}
	}
}

template<typename ELEMENT>
void ResourceBuffer<ELEMENT>::create_gpu_buffer(int numElements,
  const ELEMENT * buffer,
  GLenum usage) {
	GLenum target = getTarget();
	
	CHECK_GL_ERROR();
	
	// Create and fill a buffer object on the GPU
	glGenBuffers(1, &handle);
	glBindBuffer(target, handle);
	glBufferData(target,
	             sizeof(ELEMENT) * numElements,
	             buffer,
	             usage);
	             
	CHECK_GL_ERROR();
}

template<typename ELEMENT>
GLenum ResourceBuffer<ELEMENT>::getTarget() {
	return GL_ARRAY_BUFFER;
}

/**
Element array buffers have a different target
specification from vertex array buffers.
*/
template<> GLenum ResourceBuffer<index_t>::getTarget() {
	return GL_ELEMENT_ARRAY_BUFFER;
}

template<typename ELEMENT>
GLenum ResourceBuffer<ELEMENT>::getGLUsageToken(BUFFER_USAGE usage) {
	switch (usage) {
	case STREAM_DRAW:
		return GL_STREAM_DRAW;
	case STREAM_READ:
		return GL_STREAM_READ;
	case STREAM_COPY:
		return GL_STREAM_COPY;
	case STATIC_DRAW:
		return GL_STATIC_DRAW;
	case STATIC_READ:
		return GL_STATIC_READ;
	case STATIC_COPY:
		return GL_STATIC_COPY;
	case DYNAMIC_DRAW:
		return GL_DYNAMIC_DRAW;
	case DYNAMIC_READ:
		return GL_DYNAMIC_READ;
	case DYNAMIC_COPY:
		return GL_DYNAMIC_COPY;
	default:
		FAIL("Invalid enumerant");
		return 0;
	}
}

// template class instantiations
// (see http://www.codeproject.com/cpp/templatesourceorg.asp)
template class ResourceBuffer<vec3>;
template class ResourceBuffer<color>;
template class ResourceBuffer<vec2>;
template class ResourceBuffer<index_t>;
