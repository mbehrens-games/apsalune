/*******************************************************************************
** layout.h (the lay of the land!)
*******************************************************************************/

#ifndef LAYOUT_H
#define LAYOUT_H

enum
{
  LAYOUT_GUI_SCREEN_SONG = 0, 
  LAYOUT_GUI_SCREEN_STAFF, 
  LAYOUT_GUI_SCREEN_BAR, 
  LAYOUT_GUI_SCREEN_INSTRUMENT, 
  LAYOUT_NUM_GUI_SCREENS 
};

enum
{
  LAYOUT_GUI_ELEMENT_TYPE_PANEL = 0, 
  LAYOUT_GUI_ELEMENT_TYPE_NAME, 
  LAYOUT_GUI_ELEMENT_TYPE_VALUE, 
  LAYOUT_NUM_GUI_ELEMENT_TYPES 
};

#define LAYOUT_GUI_ELEMENTS_ARRAY_SIZE 256

#define LAYOUT_GUI_NAME_LENGTH  4
#define LAYOUT_GUI_VALUE_LENGTH 4

#define LAYOUT_GUI_NAMES_ARRAY_SIZE   (256 * LAYOUT_GUI_NAME_LENGTH)
#define LAYOUT_GUI_VALUES_ARRAY_SIZE  (256 * LAYOUT_GUI_VALUE_LENGTH)

typedef struct layout_element
{
  short int type;

  short int x;
  short int y;
  short int w;
  short int h;

  short int index;
} layout_element;

typedef struct layout_range
{
  short int start_index;
  short int end_index;
} layout_range;

extern layout_element G_layout_gui_elements[LAYOUT_GUI_ELEMENTS_ARRAY_SIZE];
extern short int      G_layout_gui_element_count;

extern layout_range   G_layout_gui_screen_ranges[LAYOUT_NUM_GUI_SCREENS];

extern char       G_layout_gui_names[LAYOUT_GUI_NAMES_ARRAY_SIZE];
extern short int  G_layout_gui_name_count;

extern char       G_layout_gui_values[LAYOUT_GUI_VALUES_ARRAY_SIZE];
extern short int  G_layout_gui_value_count;

/* function declarations */
short int layout_generate_tables();

#endif

