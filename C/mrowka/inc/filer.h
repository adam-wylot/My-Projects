#ifndef FILER_H_IS_INCLUDED
#define FILER_H_IS_INCLUDED

#include "net_gui.h"

#define MAX_FILES 10
#define SAVE_PATH "latest"

void file_make_from_gui( net_gui_t, unsigned iteration );
int file_save( net_gui_t, char* file_path );
void file_load( net_gui_t, char* file_path );

#endif /// FILER_H_IS_INCLUDED
