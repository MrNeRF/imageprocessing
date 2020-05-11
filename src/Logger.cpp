#include "Logger.h"
#include <GL/glew.h>
#include "GLFW/glfw3.h"

void Logger::Check_GL_ErrorCode()
{
	GLenum errorCode;
	while((errorCode = glGetError()) != GL_NO_ERROR)
	{
		switch(errorCode)
		{
			case GL_INVALID_ENUM:
				m_logger.error("{}, {}: INVALID_ENUM", __FILE__, __LINE__);
				break;
			case GL_INVALID_VALUE:
				m_logger.error("{}, {}: INVALID_VALUE", __FILE__, __LINE__);
				break;
			case GL_INVALID_OPERATION:
				m_logger.error("{}, {}: INVALID_OPERATION", __FILE__, __LINE__);
				break;
			case GL_STACK_OVERFLOW:
				m_logger.error("{}, {}: STACK_OVERFLOW", __FILE__, __LINE__);
				break;
			case GL_STACK_UNDERFLOW:
				m_logger.error("{}, {}: STACK_UNDERFLOW", __FILE__, __LINE__);
				break;
			case GL_OUT_OF_MEMORY:
				m_logger.error("{}, {}: OUT_OF_MEMORY", __FILE__, __LINE__);
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				m_logger.error("{}, {}: INVALID_FRAME_BUFFER_OPERATION", __FILE__, __LINE__);
				break;
		}

	}
}

