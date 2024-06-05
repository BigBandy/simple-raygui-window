/*
	Simple raygui window example, by Andy Bond
	Version: 1.1.0 (June 4, 2024)

	Changelog:

		- Altered variable naming convention.
		- Removed variables from global scope.
		- GUI elements now have a struct instead of being separate variables.
		- Unwieldy if statements refactored into functions.
		- GUI measurements are now based on a constant unit.
		- Resizing is now based on window position instead of solely mouse delta.
*/

#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#define SCREEN_WIDTH_DEFAULT 640
#define SCREEN_HEIGHT_DEFAULT 480

#define VERSION "1.1"

#define GUI_UNIT 24

typedef struct GUIElement {
	bool active;
	Rectangle bounds;
	const char *text;
} GUIElement;

static bool IsPointOnWindowBar(const Rectangle window_bounds, const Vector2 point) {
	const Rectangle window_bar = {
		window_bounds.x, window_bounds.y,
		window_bounds.width,
		GUI_UNIT
	};

	return CheckCollisionPointRec(point, window_bar);
}

static bool IsPointOnWindowCorner(const Rectangle window_bounds, const Vector2 point) {
	const Rectangle window_corner = {
		(window_bounds.x + window_bounds.width) - GUI_UNIT / 6,
		(window_bounds.y + window_bounds.height) - GUI_UNIT / 6,
		GUI_UNIT / 3, GUI_UNIT / 3
	};

	return CheckCollisionPointRec(point, window_corner);
}

int main(void) {
	char window_title[256];
	const char *window_title_format = "simple raygui window v%s";

	snprintf(window_title, 256, window_title_format, VERSION);

	GUIElement window = {
		true,
		(Rectangle){
			0.0f, 0.0f,
			GUI_UNIT * 5, GUI_UNIT * 10
		},
		window_title
	};
	bool window_dragging = false;
	bool window_stretching = false;

	GUIElement open_window = {	// in case you close the window like a doofus.
		!window.active,
		(Rectangle){
			0.0f, 0.0f,
			GUI_UNIT, GUI_UNIT
		},
		GuiIconText(ICON_BURGER_MENU, "")
	};

	InitWindow(SCREEN_WIDTH_DEFAULT, SCREEN_HEIGHT_DEFAULT, window_title);
	SetExitKey(KEY_NULL);

	GuiLoadStyle("resources/dark.rgs");

	while (!WindowShouldClose()) {
		const int screen_width = GetScreenWidth();
		const int screen_height = GetScreenHeight();

		const Vector2 mouse_position = GetMousePosition();
		const Vector2 mouse_delta = GetMouseDelta();

		const bool m1 = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

		// Update

		if (window.active) {
			// Window dragging logic
			if (m1 && !window_stretching && (IsPointOnWindowBar(window.bounds, mouse_position) || window_dragging)) {
				window_dragging = true;

				window.bounds.x += mouse_delta.x;
				window.bounds.y += mouse_delta.y;
            } else {
				window_dragging = false;

				if (window.bounds.x < 0) {
					window.bounds.x = 0;
				} else if (window.bounds.x > (float)screen_width - window.bounds.width) {
					window.bounds.x = (float)screen_width - window.bounds.width;
				}

				if (window.bounds.y < 0) {
					window.bounds.y = 0;
				} else if (window.bounds.y > (float)screen_height - window.bounds.height) {
					window.bounds.y = (float)screen_height - window.bounds.height;
				}

				// Window stretching logic
                if (!window_dragging && (IsPointOnWindowCorner(window.bounds, mouse_position) || window_stretching) ) {
                    SetMouseCursor(MOUSE_CURSOR_RESIZE_NWSE);
                    
                    if (m1) {
                        window_stretching = true;

                        window.bounds.width += mouse_position.x - (window.bounds.x + window.bounds.width);
                        window.bounds.height += mouse_position.y - (window.bounds.y + window.bounds.height);

                        if (window.bounds.width < GUI_UNIT * 3) {
							window.bounds.width = GUI_UNIT * 3;
                        } else if (window.bounds.width > screen_width - window.bounds.x) {
                            window.bounds.width = screen_width - window.bounds.x;
                        }

                        if (window.bounds.height < GUI_UNIT * 2) {
                            window.bounds.height = GUI_UNIT * 2;
                        } else if (window.bounds.height > screen_height - window.bounds.y) {
                            window.bounds.height = screen_height - window.bounds.y;
                        }
                    } else window_stretching = false;
                    
                } else {
                    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                    window_stretching = false;
                }
            }
        }

        // Draw (and a little updating bc immediate mode, y'know)

        BeginDrawing();
            
            ClearBackground(BLACK);

            if (window.active) {
                
                window.active = !GuiWindowBox(window.bounds, window.text);

                GuiLabel((Rectangle){window.bounds.x + GUI_UNIT / 6, window.bounds.y + GUI_UNIT, window.bounds.width, GUI_UNIT}, TextFormat("%i FPS", GetFPS()));

            } else window.active = GuiButton(open_window.bounds, open_window.text);
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}