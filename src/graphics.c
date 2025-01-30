/*******************************************************************************
** graphics.c (the gui graphics data file)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "drawing.h"
#include "graphics.h"

/*******************************************************************************
** graphics_load_from_file()
*******************************************************************************/
short int graphics_load_from_file(char* path)
{
  int m;

  FILE* fp;

  char signature[17];

  unsigned short value;

  unsigned char input_byte;

  /* reset signature */
  for (m = 0; m < 17; m++)
    signature[m] = '\0';

  /* make sure filename is valid */
  if (path == NULL)
    return 1;

  /* open graphics file */
  fp = fopen(path, "rb");

  /* if file did not open, return */
  if (fp == NULL)
    return 1;

  /* read signature */
  if (fread(signature, 1, 16, fp) < 16)
  {
    fclose(fp);
    return 1;
  }

  if ((strncmp(&signature[0], "NSKM", 4))     || 
      (strncmp(&signature[4], "GRAPHICS", 8)) || 
      (strncmp(&signature[12], "v1.0", 4)))
  {
    fclose(fp);
    return 1;
  }

  /* read palettes */
  for (m = 0; m < DRAWING_GUI_PALS_ARRAY_SIZE; m++)
  {
    /* high byte */
    if (fread(&input_byte, 1, 1, fp) < 1)
    {
      fclose(fp);
      return 1;
    }

    value = (input_byte << 8) & 0xFF00;

    /* low byte */
    if (fread(&input_byte, 1, 1, fp) < 1)
    {
      fclose(fp);
      return 1;
    }

    value |= (input_byte & 0x00FF);
    value &= 0x7FFF;

    G_drawing_gui_pals[m] = value;
  }

  /* read cells */
  for (m = 0; m < (DRAWING_GUI_CELLS_ARRAY_SIZE / 4); m++)
  {
    if (fread(&input_byte, 1, 1, fp) < 1)
    {
      fclose(fp);
      return 1;
    }

    G_drawing_gui_cells[4 * m + 0] = (input_byte >> 6) & 0x03;
    G_drawing_gui_cells[4 * m + 1] = (input_byte >> 4) & 0x03;
    G_drawing_gui_cells[4 * m + 2] = (input_byte >> 2) & 0x03;
    G_drawing_gui_cells[4 * m + 3] = input_byte & 0x03;
  }

  /* close file */
  fclose(fp);

  return 0;
}

