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

static short S_drawing_theme_text_pal[7] = {2, 3, 4, 5, 6, 7, 8};
static short S_drawing_theme_bg_pal[7] = {9, 10, 11, 12, 13, 14, 15};

/*******************************************************************************
** drawing_reset_all()
*******************************************************************************/
short int drawing_reset_all()
{
  int m;

  G_drawing_gui_theme = 0;

  for (m = 0; m < DRAWING_GUI_PALS_ARRAY_SIZE; m++)
    G_drawing_gui_pals[m] = 0;

  for (m = 0; m < DRAWING_GUI_CELLS_ARRAY_SIZE; m++)
    G_drawing_gui_cells[m] = 0;

  for (m = 0; m < DRAWING_FRAME_PIXELS_ARRAY_SIZE; m++)
    G_drawing_frame_pixels[m] = 0;

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

  short elem_index;

  short list_start;
  short list_size;

  short string_size;
  char  string_ch;

  short pal_offset;

  short cell_index;
  short cell_offset;

  short screen_x;
  short screen_y;

  short color;

  /* make sure the screen is valid */
  if ((screen < 0) || (screen >= LAYOUT_NUM_GUI_SCREENS))
    return 1;

  /* set list based on screen */
  list_start = G_layout_gui_screen_indices[screen];
  list_size = G_layout_gui_lists[list_start];

  /* draw each element! */
  for (k = 0; k < list_size; k++)
  {
    elem_index = G_layout_gui_lists[list_start + 1 + k];

    e = &G_layout_gui_elements[elem_index];

    /* panels */
    if (e->type == LAYOUT_GUI_ELEMENT_TYPE_PANEL)
    {
      /* flood fill with panel background color */
      pal_offset = S_drawing_theme_bg_pal[G_drawing_gui_theme] * DRAWING_GUI_NUM_COLORS;

      for (i = 0; i < 8 * e->ex_1; i++)
      {
        for (j = 0; j < 8 * e->ex_2; j++)
        {
          screen_x = e->x + i;
          screen_y = e->y + j;

          if ((screen_x < 0) || (screen_x >= DRAWING_FRAME_WIDTH))
            continue;

          if ((screen_y < 0) || (screen_y >= DRAWING_FRAME_HEIGHT))
            continue;

          G_drawing_frame_pixels[screen_y * DRAWING_FRAME_WIDTH + screen_x] = 
            G_drawing_gui_pals[pal_offset + 2];
        }
      }

      /* draw cells for the panel border */
      pal_offset = (S_drawing_theme_text_pal[G_drawing_gui_theme] & DRAWING_GUI_PALS_MASK) * DRAWING_GUI_NUM_COLORS;

      for (m = 0; m < e->ex_1; m++)
      {
        for (n = 0; n < e->ex_2; n++)
        {
          /* determine the cell offset */
          cell_index = 96;

          if (n == 0)
            cell_index += 0 * 3;
          else if (n == e->ex_2 - 1)
            cell_index += 2 * 3;
          else
            cell_index += 1 * 3;

          if (m == 0)
            cell_index += 0;
          else if (m == e->ex_1 - 1)
            cell_index += 2;
          else
            cell_index += 1;

          /* if this is not on the border of the panel, skip it */
          if (cell_index == 100)
            continue;

          cell_offset = (cell_index & DRAWING_GUI_CELLS_MASK) * DRAWING_PIXELS_PER_CELL;

          /* draw cell */
          for (i = 0; i < 8; i++)
          {
            for (j = 0; j < 8; j++)
            {
              screen_x = e->x + 8 * m + i;
              screen_y = e->y + 8 * n + j;

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
    /* fixed text strings */
    else if (e->type == LAYOUT_GUI_ELEMENT_TYPE_FIXED_TEXT)
    {
      pal_offset = (S_drawing_theme_text_pal[G_drawing_gui_theme] & DRAWING_GUI_PALS_MASK) * DRAWING_GUI_NUM_COLORS;

      string_size = G_layout_gui_strings[e->ex_1];

      for (m = 0; m < string_size; m++)
      {
        /* determine cell offset */
        string_ch = G_layout_gui_strings[e->ex_1 + 1 + m];

        if (string_ch < 32)
          cell_index = 0;
        else
          cell_index = string_ch - 32;

        cell_offset = (cell_index & DRAWING_GUI_CELLS_MASK) * DRAWING_PIXELS_PER_CELL;

        /* draw the cell */
        for (i = 0; i < 8; i++)
        {
          for (j = 0; j < 8; j++)
          {
            screen_x = e->x + 8 * m + i;
            screen_y = e->y + j;

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
  drawing_draw_screen(LAYOUT_GUI_SCREEN_INSTRUMENT_EDIT);

  return 0;
}

