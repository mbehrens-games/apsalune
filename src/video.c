/*******************************************************************************
** video.c (...killed the radio star)
*******************************************************************************/

#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#include "drawing.h"
#include "video.h"

/* sdl window, renderer, etc */
SDL_Window*   G_video_sdl_window;
SDL_Renderer* G_video_sdl_renderer;
SDL_Texture*  G_video_sdl_frame_texture;

int G_video_screen_res;
int G_video_overscan_res;

static int  S_video_screen_res_w[VIDEO_NUM_SCREEN_RESES] = 
            {  854, 1067, 1280, 1366, 1920 };

static int  S_video_screen_res_h[VIDEO_NUM_SCREEN_RESES] = 
            {  480,  600,  720,  768, 1080 };

static int  S_video_overscan_res_w[VIDEO_NUM_OVERSCAN_RESES] = 
            {  320,  400,  400 };

static int  S_video_overscan_res_h[VIDEO_NUM_OVERSCAN_RESES] = 
            {  224,  224,  200 };

/*******************************************************************************
** video_init()
*******************************************************************************/
short int video_init()
{
  /* initialize pointers to null */
  G_video_sdl_window = NULL;
  G_video_sdl_renderer = NULL;
  G_video_sdl_frame_texture = NULL;

  /* set default resolutions */
  G_video_screen_res = VIDEO_SCREEN_RES_480P;
  G_video_overscan_res = VIDEO_OVERSCAN_RES_400_X_224;

  /* create the window */
  G_video_sdl_window = SDL_CreateWindow("Apsalune",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        S_video_screen_res_w[G_video_screen_res], 
                                        S_video_screen_res_h[G_video_screen_res], 
                                        SDL_WINDOW_SHOWN);

  if (G_video_sdl_window == NULL)
  {
    printf("Failed to create window: %s\n", SDL_GetError());
    return 1;
  }

  /* create the renderer */
  G_video_sdl_renderer = SDL_CreateRenderer(G_video_sdl_window, 
                                            -1, 
                                            0);

  if (G_video_sdl_renderer == NULL)
  {
    printf("Failed to create renderer: %s\n", SDL_GetError());
    return 1;
  }

  /* set overscan size and upscaling mode */
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_RenderSetLogicalSize( G_video_sdl_renderer, 
                            S_video_overscan_res_w[G_video_overscan_res], 
                            S_video_overscan_res_h[G_video_overscan_res]);

  /* create the framebuffer texture */
  G_video_sdl_frame_texture = SDL_CreateTexture(G_video_sdl_renderer,
                                                SDL_PIXELFORMAT_RGB555,
                                                SDL_TEXTUREACCESS_STREAMING,
                                                DRAWING_FRAME_WIDTH, 
                                                DRAWING_FRAME_HEIGHT);

  if (G_video_sdl_frame_texture == NULL)
  {
    printf("Failed to create framebuffer texture: %s\n", SDL_GetError());
    return 1;
  }

  /* initialize window to black */
  SDL_SetRenderDrawColor(G_video_sdl_renderer, 0, 0, 0, 255);
  SDL_RenderClear(G_video_sdl_renderer);
  SDL_RenderPresent(G_video_sdl_renderer);

  return 0;
}

/*******************************************************************************
** video_deinit()
*******************************************************************************/
short int video_deinit()
{
  SDL_DestroyTexture(G_video_sdl_frame_texture);
  SDL_DestroyRenderer(G_video_sdl_renderer);
  SDL_DestroyWindow(G_video_sdl_window);

  return 0;
}

/*******************************************************************************
** video_display_frame()
*******************************************************************************/
short int video_display_frame()
{
  SDL_Rect overscan_rect;

  /* clear screen */
  SDL_SetRenderDrawColor(G_video_sdl_renderer, 0, 0, 0, 255);
  SDL_RenderClear(G_video_sdl_renderer);

  /* setup overscan rectangle */
  overscan_rect.x = 0;
  overscan_rect.y = 0;
  overscan_rect.w = S_video_overscan_res_w[G_video_overscan_res];
  overscan_rect.h = S_video_overscan_res_h[G_video_overscan_res];

  /* copy the framebuffer to the texture and draw it on screen */
  SDL_UpdateTexture(G_video_sdl_frame_texture, 
                    &overscan_rect, 
                    G_drawing_frame_pixels, 
                    DRAWING_FRAME_WIDTH * sizeof (unsigned short));

  SDL_RenderCopy( G_video_sdl_renderer, 
                  G_video_sdl_frame_texture, 
                  &overscan_rect, 
                  NULL);

  SDL_RenderPresent(G_video_sdl_renderer);

  return 0;
}

/*******************************************************************************
** video_resize_window()
*******************************************************************************/
short int video_resize_window(int res)
{
  int index;

  SDL_DisplayMode mode;

  /* make sure the resolution is valid */
  if ((res < 0) || (res >= VIDEO_NUM_SCREEN_RESES))
    return 1;

  /* determine display resolution */
  index = SDL_GetWindowDisplayIndex(G_video_sdl_window);
  SDL_GetDesktopDisplayMode(index, &mode);

  /* determine if the window will fit on-screen */
  if ((mode.w < S_video_screen_res_w[res]) || 
      (mode.h < S_video_screen_res_h[res]))
  {
    return 1;
  }

  /* set the new screen resolution */
  G_video_screen_res = res;

  /* resize the window if not in fullscreen */
  SDL_SetWindowSize(G_video_sdl_window, 
                    S_video_screen_res_w[G_video_screen_res], 
                    S_video_screen_res_h[G_video_screen_res]);

  SDL_SetWindowPosition(G_video_sdl_window, 
                        SDL_WINDOWPOS_CENTERED, 
                        SDL_WINDOWPOS_CENTERED);

  return 0;
}

/*******************************************************************************
** video_increase_window_size()
*******************************************************************************/
short int video_increase_window_size()
{
  if (G_video_screen_res < VIDEO_NUM_SCREEN_RESES - 1)
    video_resize_window(G_video_screen_res + 1);

  return 0;
}

/*******************************************************************************
** video_decrease_window_size()
*******************************************************************************/
short int video_decrease_window_size()
{
  if (G_video_screen_res > 0)
    video_resize_window(G_video_screen_res - 1);

  return 0;
}

