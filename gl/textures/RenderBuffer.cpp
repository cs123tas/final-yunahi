#include "RenderBuffer.h"

#include "GL/glew.h"

using namespace CS123::GL;

RenderBuffer::RenderBuffer() :
    m_handle(0)
{
    // TODO [Task 8] Call glGenRenderbuffers
    // begin ta code
    glGenRenderbuffers(1, &m_handle);
    // end ta code
}

RenderBuffer::RenderBuffer(RenderBuffer &&that) :
    m_handle(that.m_handle)
{
    that.m_handle = 0;
}

RenderBuffer& RenderBuffer::operator=(RenderBuffer &&that) {
    this->~RenderBuffer();
    m_handle = that.m_handle;
    that.m_handle = 0;
    return *this;
}

RenderBuffer::~RenderBuffer()
{
    // TODO Don't forget to delete!
    // begin ta code
    glDeleteRenderbuffers(1, &m_handle);
    // end ta code
}

void RenderBuffer::bind() const {
    // TODO [Task 8] Bind the renderbuffer
    // begin ta code
    glBindRenderbuffer(GL_RENDERBUFFER, m_handle);
    // end ta code
}

unsigned int RenderBuffer::id() const {
    return m_handle;
}

void RenderBuffer::unbind() const {
    // TODO Don't forget to unbind!
    // begin ta code
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // end ta code
}
