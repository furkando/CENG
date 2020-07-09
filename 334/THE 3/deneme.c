#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "ext21.h"
#include "utilities.c"

#define BASE_OFFSET 1024
#define EXT2_BLOCK_SIZE 1024
#define IMAGE "image.img"

typedef unsigned char bmap;
#define __NBITS (8 * (int)sizeof(bmap))
#define __BMELT(d) ((d) / __NBITS)
#define __BMMASK(d) ((bmap)1 << ((d) % __NBITS))
#define BM_SET(d, set) ((set[__BMELT(d)] |= __BMMASK(d)))
#define BM_CLR(d, set) ((set[__BMELT(d)] &= ~__BMMASK(d)))
#define BM_ISSET(d, set) ((set[__BMELT(d)] & __BMMASK(d)) != 0)

#define BLOCK_OFFSET(block) (BASE_OFFSET + (block - 1) * block_size)

unsigned char *disk;


int main(int argc, char **argv) {
    unsigned int dest_inode_idx; //index of destination inode
    //check if number of arguments is correct
    if(argc != 4) {
        fprintf(stderr, "Usage: %s <image file name> <source file path> < target file path>\n", argv[0]); //print usage
        exit(1);
    }
    //open the image file 
    int fd = open(argv[1], O_RDWR);
    //create new mapping, MAP_SHARED for commiting chages back to file, ready for read and write
    disk = mmap(NULL, 128 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); //memory mapping
    if(disk == MAP_FAILED) { //mmap returns MAP_FAILED if memory mapping fails
        perror("mmap");
        exit(1);
    }
    //get struct for the super block
    struct ext2_super_block *sb = (struct ext2_super_block *)(disk + 1024);
    //get struct for the block group
    struct ext2_group_desc *blgrp = (struct ext2_group_desc *)( disk + 2048);
    //open the source file for reading
    FILE *f = fopen(argv[2], "r");
    //string for reading
    char *buffer = NULL;
    if (f != NULL){	//read through file
          fseek(f, 0, SEEK_END);
          int length = ftell(f);
          fseek(f, 0, SEEK_SET);
          buffer = malloc(length+1); 

          //read entire file 
          fread(buffer, length, 1, f);
          fclose(f);  
    }else{  //if readin was not done successfully
    	perror("The source file path could not be opened ");
    	exit(ENOENT);
    }

    int i = strlen(argv[2]); //length of source file path
    char ptr = argv[2][i-1];
    //last char is delimenter
    if( ptr == '/'){
		  perror("Incorrect source file path");
		  exit(1);
	  }

	  //get filename
    char *filename = NULL;
    char *dest_path = NULL;
    filename = return_last_entry(argv[3]);
    dest_path = return_path(argv[3]);

    
    struct path *dest = divide_path(argv[3]);
    if(dest_path == NULL){
        dest_inode_idx = EXT2_ROOT_INO; //set index to root inode if target file path is NULL
    }else{        
        dest = divide_path(dest_path);  //if not, get path of destination
        dest_inode_idx = locate_directory(dest);  //locate the destination directory index and store to find inode
       
    }
    //free since no longer needed
    free_path(dest);
    if(dest_inode_idx < 0){ //index should not be less than 0
      perror("Incorrect dest path ");
      exit(ENOENT);
    }
    //get the inode for the directory
    struct ext2_inode * dest_inode = find_inode(dest_inode_idx);
    //if the inode is directory
    if(dest_inode && ( dest_inode->i_mode & EXT2_S_IFDIR)){
      //allocate a directory entry
      struct ext2_dir_entry *dir = malloc(sizeof( struct ext2_dir_entry));
      dir->inode = new_inode(); //create new inode for directory entry
      if( dir->inode < 0){ //should not be less than 0
        free(dir); //freeing last allocated thing
        perror("cant find free inode");
        return 0;
      }
      
      //initialize everything needed for the directory entry
      strncpy(dir->name, argv[2], strlen(argv[2])); //init name
      dir->name_len = strlen(argv[2]); //init name lenghth
      dir->rec_len = 0; //set rec_len to 0
      dir->file_type = EXT2_FT_REG_FILE; //set the file type as e regular file
      int result = insert_dir_entry(dest_inode_idx, dir); //insert the directory entry
      
      if ( result == EEXIST ){  //check if the dir entry exists
         unset_bitmap('i', dir->inode); //unset the bitmap for the inode
         sb->s_free_inodes_count++;//increment free inodes for super block
         blgrp->bg_free_inodes_count++; //increment free inodes of group block
         free(dir);
         return EEXIST;
      }
      
      if ( result > -1){ //if -1 then no space in dir

        //initalize everything needed for the target inode
        struct ext2_inode * target_inode = find_inode(dir->inode); //get the target inode
        target_inode->i_size = strlen(buffer); //set byte size
        target_inode->i_mode &= EXT2_S_IFREG; //set as regular file
        target_inode->i_dtime = 0; //file not marked for removal
        int size = 0;
        int max = strlen(buffer);
        int i = 0;

        //copying the source file contents into the data blocks
        while( size < max){
            if( i == 11){ //last direct pointer block
               target_inode->i_block[i] = new_block();
               void * d_block = BLOCK_PTR(target_inode->i_block[i]);
               target_inode->i_blocks += 2;

               while(size < max){
                int *bl = d_block;
                *bl = new_block();               
                memcpy(d_block ,buffer, EXT2_BLOCK_SIZE);
                size += EXT2_BLOCK_SIZE;
                d_block = (void *) d_block + sizeof(int);
               //may be wrong if the file is still more than block size
               }
               break;
            }
            //simply copy contents
            if( (max - size) < EXT2_BLOCK_SIZE){
                 memcpy(BLOCK_PTR(target_inode->i_block[i]),buffer, strlen(buffer));
                 target_inode->i_blocks += 2;
                 break;
            }
            memcpy(BLOCK_PTR(target_inode->i_block[i]),buffer, EXT2_BLOCK_SIZE);
            target_inode->i_blocks += 2;
            size += EXT2_BLOCK_SIZE;
            buffer = (void*) buffer + EXT2_BLOCK_SIZE;
            i++;
        }
      }

    }
    return 1;
}