#include "Logger.h"
#include <GL/glew.h>
#include "GLFW/glfw3.h"

bool Logger::Check_GL_ErrorCode()
{
	GLenum errorCode;
	while((errorCode = glGetError()) != GL_NO_ERROR)
	{
		switch(errorCode)
		{
			case GL_INVALID_ENUM:
                            m_logger.error("INVALID_ENUM");
                            break;
			case GL_INVALID_VALUE:
                            m_logger.error("INVALID_VALUE");
                            break;
			case GL_INVALID_OPERATION:
                            m_logger.error("INVALID_OPERATION");
                            break;
			case GL_STACK_OVERFLOW:
                            m_logger.error("STACK_OVERFLOW");
                            break;
			case GL_STACK_UNDERFLOW:
                            m_logger.error("STACK_UNDERFLOW");
                            break;
			case GL_OUT_OF_MEMORY:
                            m_logger.error("OUT_OF_MEMORY");
                            break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
                            m_logger.error("INVALID_FRAME_BUFFER_OPERATION");
                            break;
		}

	}

        return errorCode == GL_NO_ERROR ? true : false;
}

