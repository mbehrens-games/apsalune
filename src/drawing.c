/*******************************************************************************
** drawing.c (putting pencil to paper)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "drawing.h"
#include "layout.h"

unsigned short G_drawing_gui_theme;

unsigned short G_drawing_gui_pals[DRAWING_GUI_PALS_ARRAY_SIZE];
unsigned char  G_drawing_gui_cells[DRAWING_GUI_CELLS_ARRAY_SIZE];

unsigned short G_drawing_frame_pixels[DRAWING_FRAME_PIXELS_ARRAY_SIZE];

static draw_cmd S_drawing_draw_cmds[DRAWING_DRAW_CMD_ARRAY_SIZE];
static short    S_drawing_draw_cmd_count;

static short S_drawing_theme_text_pal[7] = {2, 3, 4, 5, 6, 7, 8};
static short S_drawing_theme_bg_pal[7] = {9, 10, 11, 12, 13, 14, 15};

/*******************************************************************************
** drawing_reset_all()
*******************************************************************************/
short int drawing_reset_all()
{
  int m;

  draw_cmd* dc;

  G_drawing_gui_theme = 0;

  for (m = 0; m < DRAWING_GUI_PALS_ARRAY_SIZE; m++)
    G_drawing_gui_pals[m] = 0;

  for (m = 0; m < DRAWING_GUI_CELLS_ARRAY_SIZE; m++)
    G_drawing_gui_cells[m] = 0;

  for (m = 0; m < DRAWING_FRAME_PIXELS_ARRAY_SIZE; m++)
    G_drawing_frame_pixels[m] = 0;

  for (m = 0; m < DRAWING_DRAW_CMD_ARRAY_SIZE; m++)
  {
    dc = &S_drawing_draw_cmds[m];

    dc->type = DRAW_CMD_TYPE_SPRITE;

    dc->pal = 0;
    dc->cell = 0;

    dc->x = 0;
    dc->y = 0;
    dc->w = 1;
    dc->h = 1;
  }

  S_drawing_draw_cmd_count = 0;

  return 0;
}

/*******************************************************************************
** drawing_draw_screen()
*******************************************************************************/
short int drawing_draw_screen(short int screen)
{
  int k;
  int m;
  int n;

  int i;
  int j;

  layout_element* e;
  layout_range* r;

  draw_cmd* dc;

  char ascii_ch;

  short pal_offset;

  short cell_index;
  short cell_offset;

  short screen_x;
  short screen_y;

  short color;

  /* make sure the screen is valid */
  if ((screen < 0) || (screen >= LAYOUT_NUM_GUI_SCREENS))
    return 1;

  /* reset draw commands */
  for (m = 0; m < S_drawing_draw_cmd_count; m++)
  {
    dc = &S_drawing_draw_cmds[m];

    dc->type = DRAW_CMD_TYPE_SPRITE;

    dc->pal = 0;
    dc->cell = 0;

    dc->x = 0;
    dc->y = 0;
    dc->w = 1;
    dc->h = 1;
  }

  S_drawing_draw_cmd_count = 0;

  /* set elements to iterate over based on screen */
  r = &G_layout_gui_screen_ranges[screen];

  /* generate draw commands for each element */
  S_drawing_draw_cmd_count = 0;

  for (m = r->start_index; m < r->end_index; m++)
  {
    e = &G_layout_gui_elements[m];

    if (e->type == LAYOUT_GUI_ELEMENT_TYPE_PANEL)
    {
      dc = &S_drawing_draw_cmds[S_drawing_draw_cmd_count];

      dc->type = DRAW_CMD_TYPE_PANEL;

      dc->pal = S_drawing_theme_text_pal[G_drawing_gui_theme];
      dc->cell = 96;

      dc->x = e->x;
      dc->y = e->y;

      dc->w = e->w;
      dc->h = e->h;

      S_drawing_draw_cmd_count += 1;
    }
    else if (e->type == LAYOUT_GUI_ELEMENT_TYPE_NAME)
    {
      for (n = 0; n < LAYOUT_GUI_NAME_LENGTH; n++)
      {
        ascii_ch = G_layout_gui_names[e->index * LAYOUT_GUI_NAME_LENGTH + n];

        if (ascii_ch == '\0')
          break;
        else
        {
          dc = &S_drawing_draw_cmds[S_drawing_draw_cmd_count];

          dc->type = DRAW_CMD_TYPE_SPRITE;

          dc->pal = S_drawing_theme_text_pal[G_drawing_gui_theme];

          if (ascii_ch < 32)
            dc->cell = 0;
          else
            dc->cell = ascii_ch - 32;

          dc->x = e->x + 8 * n;
          dc->y = e->y;

          dc->w = 1;
          dc->h = 1;

          S_drawing_draw_cmd_count += 1;
        }
      }
    }
  }

  /* process all draw commands */
  for (k = 0; k < S_drawing_draw_cmd_count; k++)
  {
    dc = &S_drawing_draw_cmds[k];

    /* flood fill with panel background color if necessary */
    if (dc->type == DRAW_CMD_TYPE_PANEL)
    {
      pal_offset = S_drawing_theme_bg_pal[G_drawing_gui_theme] * DRAWING_GUI_NUM_COLORS;

      for (i = 0; i < 8 * dc->w; i++)
      {
        for (j = 0; j < 8 * dc->h; j++)
        {
          screen_x = dc->x + i;
          screen_y = dc->y + j;

          if ((screen_x < 0) || (screen_x >= DRAWING_FRAME_WIDTH))
            continue;

          if ((screen_y < 0) || (screen_y >= DRAWING_FRAME_HEIGHT))
            continue;

          G_drawing_frame_pixels[screen_y * DRAWING_FRAME_WIDTH + screen_x] = 
            G_drawing_gui_pals[pal_offset + 2];
        }
      }
    }

    /* draw cells associated with this draw command */
    pal_offset = (dc->pal & DRAWING_GUI_PALS_MASK) * DRAWING_GUI_NUM_COLORS;

    for (m = 0; m < dc->w; m++)
    {
      for (n = 0; n < dc->h; n++)
      {
        if (dc->type == DRAW_CMD_TYPE_PANEL)
        {
          cell_index = dc->cell;

          if (n == 0)
            cell_index += 0 * 3;
          else if (n == dc->h - 1)
            cell_index += 2 * 3;
          else
            cell_index += 1 * 3;

          if (m == 0)
            cell_index += 0;
          else if (m == dc->w - 1)
            cell_index += 2;
          else
            cell_index += 1;
        }
        else if (dc->type == DRAW_CMD_TYPE_SPRITE)
          cell_index = dc->cell + n * dc->w + m;
        else
          cell_index = dc->cell;

        cell_offset = (cell_index & DRAWING_GUI_CELLS_MASK) * DRAWING_PIXELS_PER_CELL;

        /* draw cell */
        for (i = 0; i < 8; i++)
        {
          for (j = 0; j < 8; j++)
          {
            screen_x = dc->x + 8 * m + i;
            screen_y = dc->y + 8 * n + j;

            if ((screen_x < 0) || (screen_x >= DRAWING_FRAME_WIDTH))
              continue;

            if ((screen_y < 0) || (screen_y >= DRAWING_FRAME_HEIGHT))
              continue;

            color = G_drawing_gui_cells[cell_offset + 8 * j + i];

            if (color == 0)
              continue;

            G_drawing_frame_pixels[screen_y * DRAWING_FRAME_WIDTH + screen_x] = 
              G_drawing_gui_pals[pal_offset + (color & DRAWING_GUI_COLORS_MASK)];
          }
        }
      }
    }
  }

  return 0;
}

/*******************************************************************************
** drawing_compose_frame()
*******************************************************************************/
short int drawing_compose_frame()
{
  int m;

  short theme_pal;
  short bg_color;

#if 0
  /* clear framebuffer */
  for (m = 0; m < DRAWING_FRAME_PIXELS_ARRAY_SIZE; m++)
    G_drawing_frame_pixels[m] = 0x0000;
#endif

  /* draw background color */
  theme_pal = S_drawing_theme_bg_pal[G_drawing_gui_theme];
  bg_color = G_drawing_gui_pals[theme_pal * DRAWING_GUI_NUM_COLORS + 1];

  for (m = 0; m < DRAWING_FRAME_PIXELS_ARRAY_SIZE; m++)
    G_drawing_frame_pixels[m] = bg_color;

  /* draw instruments screen */
  drawing_draw_screen(LAYOUT_GUI_SCREEN_INSTRUMENT);

  return 0;
}

