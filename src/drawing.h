/*******************************************************************************
** drawing.h (putting pencil to paper)
*******************************************************************************/

#ifndef DRAWING_H
#define DRAWING_H

#define DRAWING_PIXELS_PER_CELL 64

/* gui palettes and cells */
#define DRAWING_GUI_NUM_COLORS  (1 << 2)
#define DRAWING_GUI_NUM_PALS    (1 << 4)
#define DRAWING_GUI_NUM_CELLS   (1 << 7)

#define DRAWING_GUI_COLORS_MASK (DRAWING_GUI_NUM_COLORS - 1)
#define DRAWING_GUI_PALS_MASK   (DRAWING_GUI_NUM_PALS - 1)
#define DRAWING_GUI_CELLS_MASK  (DRAWING_GUI_NUM_CELLS - 1)

#define DRAWING_GUI_PALS_ARRAY_SIZE  (DRAWING_GUI_NUM_PALS * DRAWING_GUI_NUM_COLORS)
#define DRAWING_GUI_CELLS_ARRAY_SIZE (DRAWING_GUI_NUM_CELLS * DRAWING_PIXELS_PER_CELL)

/* framebuffer */
#define DRAWING_FRAME_WIDTH  512
#define DRAWING_FRAME_HEIGHT 256

#define DRAWING_FRAME_PIXELS_ARRAY_SIZE (DRAWING_FRAME_WIDTH * DRAWING_FRAME_HEIGHT)

extern unsigned short G_drawing_gui_theme;

extern unsigned short G_drawing_gui_pals[DRAWING_GUI_PALS_ARRAY_SIZE];
extern unsigned char  G_drawing_gui_cells[DRAWING_GUI_CELLS_ARRAY_SIZE];

extern unsigned short G_drawing_frame_pixels[DRAWING_FRAME_PIXELS_ARRAY_SIZE];

/* function declarations */
short int drawing_reset_all();

short int drawing_compose_frame();

#endif

