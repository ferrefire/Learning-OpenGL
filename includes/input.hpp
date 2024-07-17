#ifndef INPUT_HPP
#define INPUT_HPP

#include "glad.h"
#include <GLFW/glfw3.h>
#include <map>
#include "camera.hpp"

class Input
{
    private:
        

    public:
        struct KeyStatus
        {
            bool down = false;
            float downDuration = 0;

            bool pressed = false;
            bool released = false;
        };

        static std::map<int, KeyStatus> keys;

		static bool canMove;
		static bool canLook;

		static float lastX;
        static float lastY;
        static float sensitivity;
        static float height;
        static float width;

		static void ProcessInput();
		static void CameraMovement();
		static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

        static void AddKey(int keyCode);
        static KeyStatus GetKey(int keyCode);
        static void SetKeyStatus();
};

#endif