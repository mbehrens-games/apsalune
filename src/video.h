/*******************************************************************************
** video.h (...killed the radio star)
*******************************************************************************/

#ifndef VIDEO_H
#define VIDEO_H

#include <SDL2/SDL.h>

enum
{
  VIDEO_SCREEN_RES_480P = 0, 
  VIDEO_SCREEN_RES_600P, 
  VIDEO_SCREEN_RES_720P, 
  VIDEO_SCREEN_RES_768P, 
  VIDEO_SCREEN_RES_1080P, 
  VIDEO_NUM_SCREEN_RESES 
};

enum
{
  VIDEO_OVERSCAN_RES_320_X_224 = 0, 
  VIDEO_OVERSCAN_RES_400_X_224, 
  VIDEO_OVERSCAN_RES_400_X_200, 
  VIDEO_NUM_OVERSCAN_RESES
};

/* sdl window, renderer, etc */
extern SDL_Window*    G_video_sdl_window;
extern SDL_Renderer*  G_video_sdl_renderer;
extern SDL_Texture*   G_video_sdl_frame_texture;

extern int G_video_screen_res;
extern int G_video_overscan_res;

/* function declarations */
short int video_init();
short int video_deinit();

short int video_display_frame();

short int video_increase_window_size();
short int video_decrease_window_size();

#endif

