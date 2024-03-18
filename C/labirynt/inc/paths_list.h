#ifndef _PATHS_LIST_H_
#define _PATHS_LIST_H_

#include "path.h"
#include "cell.h"

typedef struct p_l {
    path_t path;
    struct p_l* next;
} * paths_list_t;

path_t* add_to_paths_list( paths_list_t* );
path_t* get_from_paths_list( paths_list_t, unsigned );
int paths_list_size( paths_list_t );
int get_path_index( paths_list_t, path_t );
void print_paths_list( paths_list_t );
void remove_from_paths_list( paths_list_t*, path_t );
void clear_paths_list( paths_list_t* );
path_t* get_best_path( paths_list_t );

#endif
