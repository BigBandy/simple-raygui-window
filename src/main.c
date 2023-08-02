#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

const int screen_width = 640;
const int screen_height = 480;

Rectangle windowBounds = {
    0.0f, 0.0f,
    120.0f, 240.0f
};
bool windowActive = true;
bool windowGrabbed = false;
bool windowStretching = false;
const char *windowTitle = "raygui";

Rectangle windowOpenBounds = {      // in case you close the window like a doofus.
    0.0f, 0.0f,
    24.0f, 24.0f
};
const char *windowOpenText = "#214#";

int main(void) {
    InitWindow(screen_width, screen_height, windowTitle);
    SetExitKey(KEY_NULL);

    GuiLoadStyle("resources/dark.rgs");

    while (!WindowShouldClose()) {
        const Vector2 mouse_position = GetMousePosition();
        const Vector2 mouse_delta = GetMouseDelta();

        const bool m1 = IsMouseButtonDown(MOUSE_BUTTON_LEFT);   // Is left mouse down?
        
        // Update

        if (windowActive) {
            // Window dragging logic
            if ((CheckCollisionPointRec(mouse_position, (Rectangle){windowBounds.x, windowBounds.y, windowBounds.width, 24}) || windowGrabbed) && m1) { // <- Rectangle is window bar.
                windowGrabbed = true;

                windowBounds.x += mouse_delta.x;
                windowBounds.y += mouse_delta.y;
            } else {
                windowGrabbed = false;

                if (windowBounds.x < 0) {
                    windowBounds.x = 0;
                } else if (windowBounds.x > (float)screen_width - windowBounds.width) {
                    windowBounds.x = (float)screen_width - windowBounds.width;
                }

                if (windowBounds.y < 0) {
                    windowBounds.y = 0;
                } else if (windowBounds.y > (float)screen_height - windowBounds.height) {
                    windowBounds.y = (float)screen_height - windowBounds.height;
                }


            }
        }
        
        // Draw (and a little updating bc immediate mode, y'know)
        
        BeginDrawing();
            
            ClearBackground(BLACK);

            if (windowActive) {
                windowActive = !GuiWindowBox(windowBounds, windowTitle);

                GuiLabel((Rectangle){windowBounds.x + 4, windowBounds.y + 24, windowBounds.width, 24}, TextFormat("%i FPS", GetFPS()));

                // Window stretching logic
                if ((CheckCollisionPointRec(mouse_position, (Rectangle){windowBounds.x + windowBounds.width - 4, windowBounds.y + windowBounds.height - 4, 8, 8}) || windowStretching)) { // <- Rectangle is bottom right corner of window.
                    SetMouseCursor(MOUSE_CURSOR_RESIZE_NWSE);
                    
                    if (m1 && !windowGrabbed) {
                        windowStretching = true;

                        windowBounds.width += mouse_delta.x;
                        windowBounds.height += mouse_delta.y;

                        if (windowBounds.width < MeasureTextEx(GuiGetFont(), windowTitle, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING)).x + 48) {
                            windowBounds.width = MeasureTextEx(GuiGetFont(), windowTitle, (float)GuiGetStyle(DEFAULT, TEXT_SIZE), (float)GuiGetStyle(DEFAULT, TEXT_SPACING)).x + 48;
                        } else if (windowBounds.width > screen_width - windowBounds.x) {
                            windowBounds.width = screen_width - windowBounds.x;
                        }

                        if (windowBounds.height > screen_height - windowBounds.y) {
                            windowBounds.height = screen_height - windowBounds.y;
                        }
                    } else windowStretching = false;
                    
                } else {
                    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                    windowStretching = false;
                }
            } else windowActive = GuiButton(windowOpenBounds, windowOpenText);
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}