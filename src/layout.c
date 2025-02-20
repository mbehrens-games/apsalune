/*******************************************************************************
** layout.h (the lay of the land!)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "layout.h"

layout_element  G_layout_gui_elements[LAYOUT_GUI_ELEMENTS_ARRAY_SIZE];
short           G_layout_gui_element_count;

short G_layout_gui_lists[LAYOUT_GUI_LISTS_ARRAY_SIZE];
short G_layout_gui_list_count;

char  G_layout_gui_strings[LAYOUT_GUI_STRINGS_ARRAY_SIZE];
short G_layout_gui_string_count;

short G_layout_gui_screen_indices[LAYOUT_NUM_GUI_SCREENS];

/*******************************************************************************
** layout_add_gui_panel()
*******************************************************************************/
short int layout_add_gui_panel( short corner_x, short corner_y, 
                                short width, short height)
{
  layout_element* e;

  /* make sure there is space for this element */
  if (G_layout_gui_element_count >= LAYOUT_GUI_ELEMENTS_ARRAY_SIZE)
    return 1;

  if (G_layout_gui_list_count >= LAYOUT_GUI_LISTS_ARRAY_SIZE)
    return 1;

  /* add panel to the element array */
  e = &G_layout_gui_elements[G_layout_gui_element_count];

  e->type = LAYOUT_GUI_ELEMENT_TYPE_PANEL;

  e->x = corner_x;
  e->y = corner_y;

  e->ex_1 = width;
  e->ex_2 = height;

  /* add panel to the current list */
  G_layout_gui_lists[G_layout_gui_list_count] = G_layout_gui_element_count;

  G_layout_gui_list_count += 1;
  G_layout_gui_element_count += 1;

  return 0;
}

/*******************************************************************************
** layout_add_gui_fixed_text()
*******************************************************************************/
short int layout_add_gui_fixed_text(short center_x, short center_y, 
                                    char* str)
{
  int m;

  layout_element* e;

  short length;

  /* make sure there is space for this element */
  if (G_layout_gui_element_count >= LAYOUT_GUI_ELEMENTS_ARRAY_SIZE)
    return 1;

  if (G_layout_gui_list_count >= LAYOUT_GUI_LISTS_ARRAY_SIZE)
    return 1;

  /* compute length of text */
  length = 0;

  while ((length < 64) && (str[length] != '\0'))
    length += 1;

  /* make sure there is space for this string */
  if (G_layout_gui_string_count + length + 1 >= LAYOUT_GUI_STRINGS_ARRAY_SIZE)
    return 1;

  /* add text to the strings array */
  G_layout_gui_strings[G_layout_gui_string_count + 0] = length;

  for (m = 0; m < length; m++)
    G_layout_gui_strings[G_layout_gui_string_count + 1 + m] = str[m];

  /* add text to the element array */
  e = &G_layout_gui_elements[G_layout_gui_element_count];

  e->type = LAYOUT_GUI_ELEMENT_TYPE_FIXED_TEXT;

  e->x = center_x;
  e->y = center_y;

  e->ex_1 = G_layout_gui_string_count;
  e->ex_2 = 0;

  /* add text to the current list */
  G_layout_gui_lists[G_layout_gui_list_count] = G_layout_gui_element_count;

  G_layout_gui_list_count += 1;
  G_layout_gui_element_count += 1;
  G_layout_gui_string_count += 1 + length;

  return 0;
}

/*******************************************************************************
** layout_generate_instrument_edit_screen()
*******************************************************************************/
short int layout_generate_instrument_edit_screen()
{
  int m;

  short column_x;
  short column_y;

  short list_size_index;

  /* initialize new list */
  list_size_index = G_layout_gui_list_count;
  G_layout_gui_list_count += 1;

  G_layout_gui_screen_indices[LAYOUT_GUI_SCREEN_INSTRUMENT_EDIT] = list_size_index;

  /* initialize cooridnates */
  column_x = 12 * 8 + 0;
  column_y =  2 * 8 + 4;

  /* make the screen layout */
  layout_add_gui_fixed_text(column_x, column_y, "Loop");
  column_y += 12;

  layout_add_gui_fixed_text(column_x, column_y, "Mode");
  column_y += 12;

  layout_add_gui_fixed_text(column_x, column_y, "Fade");
  column_y += 16;

  layout_add_gui_fixed_text(column_x, column_y, "Keyscale");
  column_y += 12;

  layout_add_gui_fixed_text(column_x, column_y, "LScal");
  column_y += 12;

  layout_add_gui_fixed_text(column_x, column_y, "LCurv");
  column_y += 12;

  layout_add_gui_fixed_text(column_x, column_y, "RScal");
  column_y += 12;

  layout_add_gui_fixed_text(column_x, column_y, "RCurv");
  column_y += 12;

  layout_add_gui_fixed_text(column_x, column_y, "Break");
  column_y += 12;

  /* update list size */
  G_layout_gui_lists[list_size_index] = G_layout_gui_list_count - (list_size_index + 1);

  return 0;
}

/*******************************************************************************
** layout_generate_tables()
*******************************************************************************/
short int layout_generate_tables()
{
  int m;

  layout_element* e;

  /* reset element array */
  for (m = 0; m < LAYOUT_GUI_ELEMENTS_ARRAY_SIZE; m++)
  {
    e = &G_layout_gui_elements[m];

    e->type = 0;

    e->x = 0;
    e->y = 0;

    e->ex_1 = 0;
    e->ex_2 = 0;
  }

  G_layout_gui_element_count = 0;

  /* reset list array */
  for (m = 0; m < LAYOUT_GUI_LISTS_ARRAY_SIZE; m++)
    G_layout_gui_lists[m] = 0;

  G_layout_gui_list_count = 0;

  /* reset string array */
  for (m = 0; m < LAYOUT_GUI_STRINGS_ARRAY_SIZE; m++)
    G_layout_gui_strings[m] = '\0';

  G_layout_gui_string_count = 0;

  /* reset screen index array */
  for (m = 0; m < LAYOUT_NUM_GUI_SCREENS; m++)
    G_layout_gui_screen_indices[m] = 0;

  /* generate screen layouts */
  layout_generate_instrument_edit_screen();

  return 0;
}

