/* date = July 7th 2022 6:01 pm */

#ifndef WINDOW_H
#define WINDOW_H

typedef struct OS_Window {
	u32 width;
	u32 height;
	string title;
	u64 v[1];
} OS_Window;

void Test();

OS_Window OS_WindowCreate(u32 width, u32 height, string title);
void OS_WindowShow(OS_Window* window);
b8   OS_WindowIsOpen(OS_Window* window);
void OS_PollEvents();

#endif //WINDOW_H
