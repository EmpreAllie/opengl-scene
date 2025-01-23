#include "Callbacks.h"

float yaw = -90.0f;
float pitch = 0.0f;
bool firstTimeUsingMouse = true;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// не использовать по крайней мере для передвижения камеры, потому что вызывается не каждый кадр, а только когда нажата кнопка на клавиатуре
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    /*
    float speed = CScene::GetInstance().GetCameraSpeed();
    glm::vec3 pos = CScene::GetInstance().GetCameraPosition();
    glm::vec3 front = CScene::GetInstance().GetCameraFront();
    glm::vec3 up = CScene::GetInstance().GetCameraUp();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        CScene::GetInstance().SetCameraPosition(pos + speed * front);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        CScene::GetInstance().SetCameraPosition(pos - speed * front);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        CScene::GetInstance().SetCameraPosition(pos - glm::normalize(glm::cross(front, up)) * speed);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        CScene::GetInstance().SetCameraPosition(pos + glm::normalize(glm::cross(front, up)) * speed);
    }
    */
}

void CursorPosCallback(GLFWwindow* window, double xPosIn, double yPosIn) {
    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    if (firstTimeUsingMouse) {
        firstTimeUsingMouse = false;
        CScene::GetInstance().SetLastX(xPos);
        CScene::GetInstance().SetLastY(yPos);
    }

    float xoffset = xPos - CScene::GetInstance().GetLastX();
    float yoffset = CScene::GetInstance().GetLastY() - yPos;

    CScene::GetInstance().SetLastX(xPos);
    CScene::GetInstance().SetLastY(yPos);

    CScene::GetInstance().GetCamera()->ProcessMouseMovement(xoffset, yoffset, GL_TRUE);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    CScene::GetInstance().GetCamera()->ProcessMouseScroll(static_cast<float>(yoffset));
}

void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data) {
    const char* _source;
    const char* _type;
    const char* _severity;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        _source = "API";
        break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        _source = "WINDOW SYSTEM";
        break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        _source = "SHADER COMPILER";
        break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        _source = "THIRD PARTY";
        break;

    case GL_DEBUG_SOURCE_APPLICATION:
        _source = "APPLICATION";
        break;

    case GL_DEBUG_SOURCE_OTHER:
        _source = "UNKNOWN";
        break;

    default:
        _source = "UNKNOWN";
        break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        _type = "ERROR";
        break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = "DEPRECATED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = "UDEFINED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
        _type = "PORTABILITY";
        break;

    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = "PERFORMANCE";
        break;

    case GL_DEBUG_TYPE_OTHER:
        _type = "OTHER";
        break;

    case GL_DEBUG_TYPE_MARKER:
        _type = "MARKER";
        break;

    default:
        _type = "UNKNOWN";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        _severity = "HIGH";
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = "MEDIUM";
        break;

    case GL_DEBUG_SEVERITY_LOW:
        _severity = "LOW";
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = "NOTIFICATION";
        break;

    default:
        _severity = "UNKNOWN";
        break;
    }

    printf("%d: %s of %s severity, raised from %s: %s\n",
        id, _type, _severity, _source, msg);
}