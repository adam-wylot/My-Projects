#ifndef REPOSITORY_H_IS_INCLUDED
#define REPOSITORY_H_IS_INCLUDED

#define BUFSIZE 256

typedef enum { WIN32=0, UNIX } system_t;
enum tiles_num { WHITE=0, BLACK };

struct {
    char* file_name;
    system_t oper_system;

} repository;

void init_rep( char* file_name);

// wiadomości
void push_error( char* message );
void push_crit_error( char* message );
void send_message( char* message );
void clear_console( void );

#endif /// REPOSITORY_H_IS_INCLUDED
