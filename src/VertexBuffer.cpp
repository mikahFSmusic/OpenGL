#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    // generate one (1) buffer, output to buffer (&buffer is the pointer to our location in memory)
    GLCall(glGenBuffers(1, &m_RendererID));

    // bind array info to our buffer
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

    // size in bytes, position data, and static draw (single buffer, used many times)
    // 6 * 2 * sizeof(float), 6 floats times 2 for each triangles times the size of a float for byte conversion
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
