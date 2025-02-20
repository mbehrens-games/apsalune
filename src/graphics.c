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

  unsigned char byte[3];

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
  for (m = 0; m < (DRAWING_GUI_PALS_ARRAY_SIZE / 2); m++)
  {
    if (fread(&byte[0], 1, 3, fp) < 3)
    {
      fclose(fp);
      return 1;
    }

    G_drawing_gui_pals[2 * m + 0] = ((byte[0] << 4) & 0x0FF0) | ((byte[1] >> 4) & 0x000F);
    G_drawing_gui_pals[2 * m + 1] = ((byte[1] << 8) & 0x0F00) | (byte[2] & 0x00FF);
  }

  /* read cells */
  for (m = 0; m < (DRAWING_GUI_CELLS_ARRAY_SIZE / 4); m++)
  {
    if (fread(&byte[0], 1, 1, fp) < 1)
    {
      fclose(fp);
      return 1;
    }

    G_drawing_gui_cells[4 * m + 0] = (byte[0] >> 6) & 0x03;
    G_drawing_gui_cells[4 * m + 1] = (byte[0] >> 4) & 0x03;
    G_drawing_gui_cells[4 * m + 2] = (byte[0] >> 2) & 0x03;
    G_drawing_gui_cells[4 * m + 3] = byte[0] & 0x03;
  }

  /* close file */
  fclose(fp);

  return 0;
}

