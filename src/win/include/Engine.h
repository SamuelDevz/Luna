#pragma once

#include "Graphics.h"
#include "Window.h"
#include "Input.h"
#include "Timer.h"
#include "Game.h"

namespace Luna
{
    class Engine
    {
    private:
        static Timer timer;
        static bool paused;

        double FrameTime() noexcept;
        int Loop();

    public:
        static Graphics* graphics;
        static Window*   window;
        static Input*    input;
        static Game*     game;
        static double    frameTime;

        explicit Engine() noexcept;
        ~Engine() noexcept;

        int Start(Game * const game);
        
        static void Pause() noexcept;
        static void Resume() noexcept;

        static LRESULT CALLBACK EngineProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };

    inline void Engine::Pause() noexcept
    { paused = true; timer.Stop(); }

    inline void Engine::Resume() noexcept
    { paused = false; timer.Start(); }
}