#include "camera.hpp"

namespace OZ {
  Camera& Camera::init(GLuint type) {
    this->setType(type);
    switch(this->type) {
      case CAMERA_TYPE_FPS:
        this->initFPS();
        break;
      case CAMERA_TYPE_TPS:
        break;
      case CAMERA_TYPE_STRATEGY:
        break;
      case CAMERA_TYPE_SIDE_SCROLLER:
        break;
    }

    return *this;
  }

  Camera& Camera::initFPS() {
    this->position = glm::vec3(0.0f, 0.0f,  3.0f);
    this->direction = glm::vec3(0.0f, 0.0f, -1.0f);
    this->up = glm::vec3(0.0f, 1.0f,  0.0f);
    this->pitch = 0.0f;
    this->yaw = 270.0f;
    this->velocity = 5.0f;
    this->sensitivity = 0.1f;

    std::function<void (double ,double)> mouseCB = [this](double xdiff, double ydiff) {
      yaw += (xdiff * sensitivity);
      pitch += (ydiff * sensitivity);
      if(pitch > 89.9f)
        pitch = 89.9f;
      if(pitch < -89.9f)
        pitch = -89.9f;
    };

    std::function<void (int ,int)> keyboardCB = [this](int key, int action) {
      if(action == GLFW_PRESS) {
        if(key == GLFW_KEY_W)
          moveDirections[MOVE_FORWARD] = true;
        if(key == GLFW_KEY_S)
          moveDirections[MOVE_BACK] = true;
        if(key == GLFW_KEY_A)
          moveDirections[MOVE_LEFT] = true;
        if(key == GLFW_KEY_D)
          moveDirections[MOVE_RIGHT] = true;
      } else if(action == GLFW_RELEASE) {
        if(key == GLFW_KEY_W)
          moveDirections[MOVE_FORWARD] = false;
        if(key == GLFW_KEY_S)
          moveDirections[MOVE_BACK] = false;
        if(key == GLFW_KEY_A)
          moveDirections[MOVE_LEFT] = false;
        if(key == GLFW_KEY_D)
          moveDirections[MOVE_RIGHT] = false;
      }
    };

    eventEmitter.on<double, double>(0, mouseCB);
    eventEmitter.on<int, int>(1, keyboardCB);

    return *this;
  }

  Camera& Camera::setType(GLuint t) {
    this->type = t;
    return *this;
  }

  Camera& Camera::setVelocity(GLfloat v) {
    this->velocity = v;
    return *this;
  }

  Camera& Camera::setSensitivity(GLfloat s) {
    this->sensitivity = s;
    return *this;
  }

  Camera& Camera::setPosition(GLfloat x, GLfloat y, GLfloat z) {
    this->position = glm::vec3(x, y ,z);
    return *this;
  }

  Camera& Camera::setPosition(glm::vec3 posVec) {
    this->position = posVec;
    return *this;
  }

  Camera& Camera::setDirection(GLfloat x, GLfloat y, GLfloat z) {
    this->direction = glm::vec3(x, y ,z);
    return *this;
  }

  Camera& Camera::setDirection(glm::vec3 directionVec) {
    this->direction = directionVec;
    return *this;
  }

  Camera& Camera::setUp(GLfloat x, GLfloat y, GLfloat z) {
    this->up = glm::vec3(x, y ,z);
    return *this;
  }

  Camera& Camera::setUp(glm::vec3 upVec) {
    this->up = upVec;
    return *this;
  }

  Camera& Camera::calculateLinearDisplacement(GLfloat dt) {
    if(this->moveDirections[MOVE_FORWARD])
      this->position += (this->velocity * dt) * this->direction;
    if(this->moveDirections[MOVE_BACK])
      this->position -= (this->velocity * dt) * this->direction;
    if(this->moveDirections[MOVE_LEFT])
      this->position -= glm::normalize(glm::cross(this->direction, this->up)) * (this->velocity * dt);
    if(this->moveDirections[MOVE_RIGHT])
      this->position += glm::normalize(glm::cross(this->direction, this->up)) * (this->velocity * dt);
    return *this;
  }

  Camera& Camera::calculateAngularDisplacement() {
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->direction = glm::normalize(front);
    return *this;
  }

  glm::mat4 Camera::getViewMatrix() {
    glm::mat4 view = glm::lookAt(this->position, (this->position + this->direction), this->up);
    return view;
  }
}
