/*******************************************************************************
** layout.h (the lay of the land!)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "layout.h"

layout_element  G_layout_gui_elements[LAYOUT_GUI_ELEMENTS_ARRAY_SIZE];
short int       G_layout_gui_element_count;

layout_range    G_layout_gui_screen_ranges[LAYOUT_NUM_GUI_SCREENS];

char      G_layout_gui_names[LAYOUT_GUI_NAMES_ARRAY_SIZE];
short int G_layout_gui_name_count;

char      G_layout_gui_values[LAYOUT_GUI_VALUES_ARRAY_SIZE];
short int G_layout_gui_value_count;

/*******************************************************************************
** layout_add_gui_panel()
*******************************************************************************/
short int layout_add_gui_panel( short int corner_x, short int corner_y, 
                                short int width, short int height)
{
  layout_element* e;

  /* make sure there is space for this element */
  if (G_layout_gui_element_count >= LAYOUT_GUI_ELEMENTS_ARRAY_SIZE)
    return 1;

  /* add the panel! */
  e = &G_layout_gui_elements[G_layout_gui_element_count];

  e->type = LAYOUT_GUI_ELEMENT_TYPE_PANEL;

  e->x = corner_x;
  e->y = corner_y;
  e->w = width;
  e->h = height;

  e->index = 0;

  G_layout_gui_element_count += 1;  

  return 0;
}

/*******************************************************************************
** layout_add_gui_name()
*******************************************************************************/
short int layout_add_gui_name(short int corner_x, short int corner_y, 
                              char* str)
{
  layout_element* e;

  /* make sure there is space for this element */
  if (G_layout_gui_element_count >= LAYOUT_GUI_ELEMENTS_ARRAY_SIZE)
    return 1;

  if (G_layout_gui_name_count >= LAYOUT_GUI_NAMES_ARRAY_SIZE)
    return 1;

  /* add the name! */
  e = &G_layout_gui_elements[G_layout_gui_element_count];

  e->type = LAYOUT_GUI_ELEMENT_TYPE_NAME;

  e->x = corner_x;
  e->y = corner_y;
  e->w = LAYOUT_GUI_NAME_LENGTH;
  e->h = 1;

  e->index = G_layout_gui_name_count;

  strncpy(&G_layout_gui_names[G_layout_gui_name_count * LAYOUT_GUI_NAME_LENGTH], 
          str, LAYOUT_GUI_NAME_LENGTH);

  G_layout_gui_element_count += 1;
  G_layout_gui_name_count += 1;

  return 0;
}

/*******************************************************************************
** layout_generate_tables()
*******************************************************************************/
short int layout_generate_tables()
{
  int m;

  layout_element* e;
  layout_range* r;

  /* reset element array */
  for (m = 0; m < LAYOUT_GUI_ELEMENTS_ARRAY_SIZE; m++)
  {
    e = &G_layout_gui_elements[m];

    e->type = 0;

    e->x = 0;
    e->y = 0;
    e->w = 0;
    e->h = 0;

    e->index = 0;
  }

  G_layout_gui_element_count = 0;

  /* reset screen ranges */
  for (m = 0; m < LAYOUT_NUM_GUI_SCREENS; m++)
  {
    r = &G_layout_gui_screen_ranges[m];

    r->start_index = 0;
    r->end_index = 0;
  }

  /* reset text arrays */
  for (m = 0; m < LAYOUT_GUI_NAMES_ARRAY_SIZE; m++)
    G_layout_gui_names[m] = '\0';

  G_layout_gui_name_count = 0;

  for (m = 0; m < LAYOUT_GUI_VALUES_ARRAY_SIZE; m++)
    G_layout_gui_values[m] = '\0';

  G_layout_gui_value_count = 0;

  /* instrument screen */
  r = &G_layout_gui_screen_ranges[LAYOUT_GUI_SCREEN_INSTRUMENT];

  r->start_index = G_layout_gui_element_count;

  layout_add_gui_name(64, 16, "Amp");
  layout_add_gui_name(64, 32, "Mul");

  layout_add_gui_panel(304, 16, 8, 12);

  r->end_index = G_layout_gui_element_count;

  return 0;
}

