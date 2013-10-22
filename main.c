/** \file main.c
  * \author Nikolay Zamotaev
  * \brief Main program file for programmable watch
  */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "launcher.h"
#include "state.h"
#include "ivm.h"
#include "bitmaps.h"
#include "memory.h"
#include "peripherials/lcd.h"

void hardware_init( void )
{
  lcd_init(  );
}

typedef struct vm_list
{
  char *name;
  unsigned int instructions;
  struct t_virtual_machine *machine_ptr;
  struct vm_list *next;
} t_vm_list;

/** \brief main program function
  */
int main( int argc, char **argv )
{
  t_vm_list *vm_list_head = malloc( sizeof( t_vm_list ) );      //Allocate first element;
  t_vm_list *current_machine = vm_list_head;

  vm_list_head->next = vm_list_head;    //Кольцевой список
  vm_list_head->name = NULL;
  vm_list_head->instructions = 0;
  vm_list_head->machine_ptr = NULL;

  hardware_init(  );

  //Begin reading VMs
  DIR *vm_images_dir = opendir( ROOT_PATH STATES_PATH );
  struct dirent *directory_entry;
  while( ( directory_entry = readdir( vm_images_dir ) ) != NULL ) {
    //Проверить имя файла, если это .vm - залить в память структуры.
    char *temp1 = NULL;
    temp1 = strstr( directory_entry->d_name, ".vm" );
    if( ( temp1 - directory_entry->d_name ) ==
        ( strlen( directory_entry->d_name ) - strlen( ".vm" ) ) ) {
      //В имени не только .vm есть, но и расположен он в конце
      //Это образ состояния виртуалки. Загружаем
      t_vm_list *next_element = malloc( sizeof( t_vm_list ) );
      if( next_element == NULL ) {
        //Not enough RAM, terminate loading of this applet, add logging
        continue;
      };
      next_element->name = strdup( directory_entry->d_name );    //TODO: implement checking for RAM
      next_element->instructions = 0;
      next_element->machine_ptr = malloc( sizeof( struct t_virtual_machine ) ); //TODO: implement checking for RAM
      if( ( next_element->name == NULL )
          || ( next_element->machine_ptr == NULL ) ) {
        //Not enough RAM, terminate loading, free up RAM
        free( next_element->name );
        free( next_element->machine_ptr );
        free( next_element );
        continue;
      };
      next_element->next = vm_list_head->next;
      //Загрузка VM - здесь
      {
        char *filename = malloc( strlen( ROOT_PATH STATES_PATH ) + strlen( next_element->name ) + 1 );  //TODO: implement memory checks
        strcpy( filename, ROOT_PATH STATES_PATH );
        strcat( filename, next_element->name );
        FILE *new_image = fopen( filename, "r+" );
        printf( "Loading VM image: %s\t", filename );
        if( vm_load_state( new_image, next_element->machine_ptr ) !=
            0 ) {
          //ERROR while loading VM.
          //Log it and cleanup
          free( next_element->name );
          free( next_element->machine_ptr );
          free( next_element );
          printf( "FAIL\n" );
        }
        else {
          printf( "OK\n" );
        };
        fclose( new_image );
        free( filename );
        vm_list_head->next = next_element;
      };
    }
  };

  closedir( vm_images_dir );
  //End reading VMs

  printf( "\033[18;1H \033[17;30r" );
//  ivm_reset( &machine );
//  vm_save_state( new_image, machine );
  while( 1 ) {
    struct t_virtual_machine *machine;
    machine = current_machine->machine_ptr;
    if( machine != NULL ) {
      printf
        ( "name=%10s i=%04x ivm_pc=%04x  ivm_dp=% 2d  ivm_rp=% 2d insn=%04x\n",
          current_machine->name, current_machine->instructions,
          machine->ivm_pc, machine->ivm_dp, machine->ivm_rp,
          ivm_mem_get( machine, machine->ivm_pc ) & 0xffffu );
      ivm_step( machine, ivm_mem_get( machine, machine->ivm_pc ) );
      current_machine->instructions++;
      if( current_machine->instructions % 1024 == 0 ) {
        char *filename = malloc( strlen( ROOT_PATH STATES_PATH ) + strlen( current_machine->name ) + 1 );       //TODO: implement memory checks
        strcpy( filename, ROOT_PATH STATES_PATH );
        strcat( filename, current_machine->name );
        FILE *new_image = fopen( filename, "w+" );
        vm_save_state( new_image, *machine );
        fclose( new_image );
        free( filename );
      };
    };
    current_machine = current_machine->next;
  };

  return 0;
};
