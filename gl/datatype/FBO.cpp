#include "FBO.h"

#include "GL/glew.h"

#include "gl/GLDebug.h"
#include "gl/textures/RenderBuffer.h"
#include "gl/textures/DepthBuffer.h"
#include "gl/textures/Texture2D.h"
#include "gl/textures/TextureParametersBuilder.h"

using namespace CS123::GL;

FBO::FBO(int numberOfColorAttachments, DEPTH_STENCIL_ATTACHMENT attachmentType, int width, int height,
         TextureParameters::WRAP_METHOD wrapMethod,
         TextureParameters::FILTER_METHOD filterMethod, GLenum type) :
    m_depthStencilAttachmentType(attachmentType),
    m_handle(0),
    m_width(width),
    m_height(height)
{
    // TODO [Task 2]
    // Generate a new framebuffer using m_handle
    // begin ta code
    glGenFramebuffers(1, &m_handle);
    // end ta code

    // TODO [Task 3]
    // Call bind() and fill it in with glBindFramebuffer
    // Call generateColorAttachments() and fill in generateColorAttachment()
    // begin ta code
    bind();
    generateColorAttachments(numberOfColorAttachments, wrapMethod, filterMethod, type);
    // end ta code

    // TODO [Task 8] Call generateDepthStencilAttachment()
    // begin ta code
    generateDepthStencilAttachment();
    // end ta code

    // This will make sure your framebuffer was generated correctly!
    checkFramebufferStatus();

    // TODO [Task 3] Call unbind() and fill it in
    // begin ta code
    unbind();
    // end ta code
}

FBO::~FBO()
{
    // TODO Don't forget to delete!
    // begin ta code
    glDeleteFramebuffers(1, &m_handle);
    // end ta code
}

void FBO::generateColorAttachments(int count, TextureParameters::WRAP_METHOD wrapMethod,
                                   TextureParameters::FILTER_METHOD filterMethod, GLenum type) {
    std::vector<GLenum> buffers;
    for (int i = 0; i < count; i++) {
        generateColorAttachment(i, wrapMethod, filterMethod, type);
        buffers.push_back(GL_COLOR_ATTACHMENT0 + i);
    }
    // TODO [Task 3] Call glDrawBuffers
    // begin ta code
    glDrawBuffers(count, &buffers[0]);
    // end ta code
}

void FBO::generateDepthStencilAttachment() {
    switch(m_depthStencilAttachmentType) {
        case DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY:
            m_depthAttachment = std::make_unique<DepthBuffer>(m_width, m_height);
            // TODO [Task 8]
            // begin ta code
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthAttachment->id());
            // end ta code
            break;
        case DEPTH_STENCIL_ATTACHMENT::DEPTH_STENCIL:
            // Left as an exercise to students
            break;
        case DEPTH_STENCIL_ATTACHMENT::NONE:
            break;
    }
}

void FBO::generateColorAttachment(int i, TextureParameters::WRAP_METHOD wrapMethod,
                                  TextureParameters::FILTER_METHOD filterMethod, GLenum type) {
    Texture2D tex(nullptr, m_width, m_height, type);
    TextureParametersBuilder builder;

    // TODO [Task 2]
    // - Set the filter method using builder.setFilter() with filterMethod
    // - Set the wrap method using builder.setWrap() with wrapMethod
    // begin ta code
    builder.setFilter(filterMethod);
    builder.setWrap(wrapMethod);
    // end ta code

    TextureParameters parameters = builder.build();
    parameters.applyTo(tex);

    // TODO [Task 3] Call glFramebufferTexture2D using tex.id()
    // begin ta code
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, tex.id(), 0);
    // end ta code

    m_colorAttachments.push_back(std::move(tex));
}

void FBO::bind() {
    // TODO [Task 3]
    // begin ta code
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    // end ta code

    // TODO [Task 4] // Resize the viewport to our FBO's size
    // begin ta code
    glViewport(0, 0, m_width, m_height);
    // end ta code
}

void FBO::unbind() {
    // TODO [Task 3]
    // begin ta code
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // end ta code
}

const Texture2D& FBO::getColorAttachment(int i) const {
    return m_colorAttachments.at(i);
}

const RenderBuffer& FBO::getDepthStencilAttachment() const {
    return *m_depthAttachment.get();
}
