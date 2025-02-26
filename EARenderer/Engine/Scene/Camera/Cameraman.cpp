//
//  Cameraman.cpp
//  EARenderer
//
//  Created by Pavlo Muratov on 13.07.17.
//  Copyright © 2017 MPO. All rights reserved.
//

#include "Cameraman.hpp"

#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace EARenderer {

#pragma mark - Lifecycle

    Cameraman::Cameraman(Camera *camera, Input *userInput, const GLViewport *viewport)
            :
            mCamera(camera),
            mUserInput(userInput),
            mViewport(viewport) {
        mUserInput->keyboardEvent()[Input::KeyboardAction::KeyDown] += {"cameraman.key.down", this, &Cameraman::handleKeyDown};
        mUserInput->keyboardEvent()[Input::KeyboardAction::KeyUp] += {"cameraman.key.up", this, &Cameraman::handleKeyUp};
        mUserInput->simpleMouseEvent()[Input::SimpleMouseAction::Drag] += {"cameraman.mouse.drag", this, &Cameraman::handleMouseDrag};
        mUserInput->simpleMouseEvent()[Input::SimpleMouseAction::PressDown] += {"cameraman.mouse.down", this, &Cameraman::handleMouseDown};
        mUserInput->scrollMouseEvent() += {"cameraman.mouse.scroll", this, &Cameraman::handleMouseScroll};
    }

    Cameraman::~Cameraman() {
        mUserInput->keyboardEvent()[Input::KeyboardAction::KeyDown] -= "cameraman.key.down";
        mUserInput->keyboardEvent()[Input::KeyboardAction::KeyUp] -= "cameraman.key.up";
        mUserInput->simpleMouseEvent()[Input::SimpleMouseAction::Drag] -= "cameraman.mouse.drag";
        mUserInput->simpleMouseEvent()[Input::SimpleMouseAction::PressDown] -= "cameraman.mouse.down";
        mUserInput->scrollMouseEvent() -= "cameraman.mouse.scroll";
    }

#pragma mark - Getters

    bool Cameraman::isEnabled() const {
        return mIsEnabled;
    }

#pragma mark - Setters

    void Cameraman::setIsEnabled(bool enabled) {
        mIsEnabled = enabled;
    }

#pragma mark - Event Handlers

    void Cameraman::handleKeyDown(const Input *input) {
        glm::vec3 direction = glm::zero<glm::vec3>();

        if (input->isKeyPressed(Input::Key::W)) {direction += mCamera->front();}
        if (input->isKeyPressed(Input::Key::A)) {direction -= mCamera->right();}
        if (input->isKeyPressed(Input::Key::S)) {direction -= mCamera->front();}
        if (input->isKeyPressed(Input::Key::D)) {direction += mCamera->right();}

        mKeyboardMoveDirection = glm::length(direction) > std::numeric_limits<float>::epsilon() ? glm::normalize(direction) : direction;
        mKeyboardMoveDirection *= 0.05f;
    }

    void Cameraman::handleKeyUp(const Input *input) {
        if (input->isKeyPressed(Input::Key::W)) {return;}
        if (input->isKeyPressed(Input::Key::A)) {return;}
        if (input->isKeyPressed(Input::Key::S)) {return;}
        if (input->isKeyPressed(Input::Key::D)) {return;}

        mKeyboardMoveDirection = glm::zero<glm::vec3>();
    }

    void Cameraman::handleMouseDrag(const Input *input) {
        glm::vec2 mouseDirection = input->mousePosition() - mPreviousMousePosition;

        if (input->isMouseButtonPressed(0) && input->isMouseButtonPressed(1)) {
            if (mMouseLockDirection == glm::zero<glm::vec2>()) {
                mMouseLockDirection = isMouseMovingVertically(mouseDirection) ? glm::vec2(0.0, 1.0) : glm::vec2(1.0, 0.0);
            }
            if (mMouseLockDirection.x == 0.0) {
                mMouseMoveDirection = mCamera->up() * mouseDirection.y * 0.005f;
            } else {
                mMouseMoveDirection = mCamera->right() * mouseDirection.x * 0.005f;
            }
        } else if (input->pressedMouseButtonsMask() && mKeyboardMoveDirection != glm::zero<glm::vec3>()) {
            // Acting like FPS-style camera with 'noclip' enabled
            mRotation = mouseDirection;
        } else if (input->isMouseButtonPressed(0)) {
            if (mMouseLockDirection == glm::zero<glm::vec2>()) {
                mMouseLockDirection = isMouseMovingVertically(mouseDirection) ? glm::vec2(0.0, 1.0) : glm::vec2(1.0, 0.0);
            }
            if (mMouseLockDirection.x == 0.0) {
                mMouseMoveDirection = mCamera->front() * mouseDirection.y * 0.005f;
            } else {
                mRotation = mouseDirection;
                mRotation.y = 0.0;
            }
        } else if (input->isMouseButtonPressed(1)) {
            mRotation = mouseDirection;
        } else if (input->isMouseButtonPressed(2)) {
            glm::vec3 up = mCamera->up() * mouseDirection.y * 0.005f;
            glm::vec3 right = mCamera->right() * mouseDirection.x * 0.005f;
            mMouseMoveDirection = up + right;
        }

        mPreviousMousePosition = input->mousePosition();
    }

    void Cameraman::handleMouseScroll(const Input *input) {
        glm::vec2 scrollDelta = input->scrollDelta();
        glm::vec3 front = mCamera->front() * scrollDelta.y * -0.005f;
        glm::vec3 right = mCamera->right() * scrollDelta.x * 0.005f;
        mMouseMoveDirection = front + right;
    }

    void Cameraman::handleMouseDown(const Input *input) {
        mPreviousMousePosition = input->mousePosition();
        mMouseLockDirection = glm::zero<glm::vec2>();
    }

    bool Cameraman::isMouseMovingVertically(const glm::vec2 &mouseDirection) const {
        return glm::angle(mouseDirection, glm::vec2(0.0, -1.0)) < M_PI_4 || glm::angle(mouseDirection, glm::vec2(0.0, 1.0)) < M_PI_4;
    }

    bool Cameraman::isMouseMovingHorizontally(const glm::vec2 &mouseDirection) const {
        return glm::angle(mouseDirection, glm::vec2(-1.0, 0.0)) < M_PI_4 || glm::angle(mouseDirection, glm::vec2(1.0, 0.0)) < M_PI_4;
    }

#pragma mark - Public

    void Cameraman::updateCamera() {
        if (mIsEnabled) {
            mCamera->moveBy(mKeyboardMoveDirection);
            mCamera->moveBy(mMouseMoveDirection);
            mCamera->rotateBy(mRotation.y, mRotation.x);
            mCamera->setViewportAspectRatio(mViewport->aspectRatio());
        }

        mRotation = glm::zero<glm::vec2>();
        mMouseMoveDirection = glm::zero<glm::vec3>();
    }

}
