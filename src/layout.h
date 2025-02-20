/*******************************************************************************
** layout.h (the lay of the land!)
*******************************************************************************/

#ifndef LAYOUT_H
#define LAYOUT_H

enum
{
  LAYOUT_GUI_SCREEN_INSTRUMENT_EDIT = 0, 
  LAYOUT_GUI_SCREEN_REVERB, 
  LAYOUT_NUM_GUI_SCREENS 
};

enum
{
  LAYOUT_GUI_ELEMENT_TYPE_PANEL = 0, 
  LAYOUT_GUI_ELEMENT_TYPE_FIXED_TEXT, 
  LAYOUT_GUI_ELEMENT_TYPE_VARYING_TEXT, 
  LAYOUT_NUM_GUI_ELEMENT_TYPES 
};

#define LAYOUT_GUI_ELEMENTS_ARRAY_SIZE  256
#define LAYOUT_GUI_LISTS_ARRAY_SIZE     256
#define LAYOUT_GUI_STRINGS_ARRAY_SIZE  1024

typedef struct layout_element
{
  short type;

  short x;
  short y;

  short ex_1;
  short ex_2;
} layout_element;

extern layout_element G_layout_gui_elements[LAYOUT_GUI_ELEMENTS_ARRAY_SIZE];
extern short          G_layout_gui_element_count;

extern short  G_layout_gui_lists[LAYOUT_GUI_LISTS_ARRAY_SIZE];
extern short  G_layout_gui_list_count;

extern char   G_layout_gui_strings[LAYOUT_GUI_STRINGS_ARRAY_SIZE];
extern short  G_layout_gui_string_count;

extern short  G_layout_gui_screen_indices[LAYOUT_NUM_GUI_SCREENS];

/* function declarations */
short int layout_generate_tables();

#endif

