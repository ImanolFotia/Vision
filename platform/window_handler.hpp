#pragma once

#include <functional>
#include "../core/log.hpp"

class WindowHandler {
    public:
        virtual void Create(const char*, int, int, int) = 0;
        virtual void Resize(int, int) = 0;
        virtual void Destroy() = 0;

        virtual void Handler() = 0;
        virtual void Update() = 0;
        virtual void SwapBuffers() = 0;

        virtual void setClickCallback(std::function<void()>) = 0;
        virtual void setDestroyCallback(std::function<void()>) = 0;
        virtual void setResizeCallback(std::function<void()>) = 0;

    private:
        std::function<void()> mOnClick;
        std::function<void()> mOnDestroy;
        std::function<void()> mOnResize;
};