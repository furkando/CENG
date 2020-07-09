#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "ext21.h"
#include <errno.h>

#include <string.h>

#define INODES_COUNT 32
#define BLOCKS_COUNT 128
#define BLOCK_PTR(i) (disk + (i)*EXT2_BLOCK_SIZE)

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

unsigned int block_size = 0;
#define BLOCK_OFFSET(block) (BASE_OFFSET + (block - 1) * block_size)

extern unsigned char *disk;

struct path
{
    char name[30];
    int isfile;
    struct path *next;
};


struct ext2_inode *find_inode(unsigned int idx)
{

    struct ext2_group_desc *gd = (struct ext2_group_desc *)(disk + (EXT2_BLOCK_SIZE * 2));

    unsigned char *inode_table = (unsigned char *)(disk + EXT2_BLOCK_SIZE * gd->bg_inode_table);

    return (struct ext2_inode *)(inode_table + (sizeof(struct ext2_inode) * (idx - 1)));
}

struct ext2_dir_entry *search_dir(struct ext2_dir_entry *directory, char *entry)
{
    struct ext2_dir_entry *dir = directory;
    int size = 0;

    while (size < EXT2_BLOCK_SIZE)
    {
        size += dir->rec_len;

        if (strncmp(entry, dir->name, dir->name_len) == 0)
        {
            return dir;
        }
        dir = (void *)dir + dir->rec_len;
    }
    return NULL;
}

struct path *divide_path(char *path)
{
    int isfile = 1;
    int len = strlen(path);
    if (path[0] != '/')
    {
        perror("Target path is wrong");
        exit(1);
    }
    if (path[len - 1] == '/')
    {
        path[len - 1] = '\0';
        isfile = 0;
    }

    struct path *head = NULL;
    struct path *tail;

    int i = 1;
    int j = 1;
    int lenght;
    while (i < len)
    {
        if (path[i] == '/')
        {

            if (head == NULL)
            {
                head = malloc(sizeof(struct path));
                tail = head;
            }
            else
            {

                tail->next = malloc(sizeof(struct path));
                tail = tail->next;
            }
            tail->next = NULL;
            lenght = i - j;
            tail->isfile = 0;
            memcpy(tail->name, &(path[j]), lenght);
            tail->name[lenght] = 0;
            j = i + 1;
        }
        i++;
    }
    if (j != i)
    {
        if (head == NULL)
        {
            head = malloc(sizeof(struct path));
            tail = head;
        }
        else
        {
            tail->next = malloc(sizeof(struct path));
            tail = tail->next;
        }

        tail->next = NULL;
        tail->isfile = 0;
        lenght = i - j;
        memcpy(tail->name, &(path[j]), lenght);
        tail->name[lenght] = '\0';
    }
    if (isfile)
    {
        tail->isfile = 1;
    }
    return head;
}

int locate_directory(struct path *head)
{
    int i = EXT2_ROOT_INO;
    struct ext2_group_desc *blgrp = (struct ext2_group_desc *)(disk + 2048);
    unsigned char *itable = (unsigned char *)(disk + EXT2_BLOCK_SIZE * blgrp->bg_inode_table);

    while (head != NULL)
    {
        if (i > 2 && i < EXT2_GOOD_OLD_FIRST_INO)
        {
            printf("inode in reserved space\n");
            break;
        }
        struct ext2_inode *inode = (struct ext2_inode *)(itable + sizeof(struct ext2_inode) * (i - 1));

        if (inode->i_size == 0)
        {
            printf("inode size is 0");
            break;
        }

        if (EXT2_S_IFDIR & inode->i_mode)
        {
            int j = 0;
            int mark = 0;
            int num_blocks = inode->i_blocks / 2;
            int block_no;
            for (j = 0; j < num_blocks; j++)
            {
                if (j >= 12)
                {
                    return -1;
                }
                block_no = inode->i_block[j];
                char *entry = head->name;
                struct ext2_dir_entry *dir = (struct ext2_dir_entry *)(disk + EXT2_BLOCK_SIZE * block_no);
                struct ext2_dir_entry *found = search_dir(dir, entry);
                if (found)
                {
                    mark = 1;
                    i = found->inode;
                    if (head->next == NULL)
                    {
                        return i;
                    }
                    struct path *new = head;
                    head = head->next;
                    free(new);
                    break;
                }
            }
            if (!mark)
            {
                perror("Cannot find inode");
                return -1;
            }
        }
        if (EXT2_S_IFREG & inode->i_mode)
        {
            return i;
        }
    }
    return -1;
}

int find_free_block(unsigned char *bitmap, int size)
{
    int i, x;
    int position;
    unsigned char buf;
    for (i = 0; i < size; i++)
    {
        for (x = 0; x < 8; x++)
        {
            buf = *bitmap;
            if (!((buf >> x) & 1))
            {
                position = 8 * i + x;
                return position;
            }
        }
        bitmap++;
    }
    return -1;
}

void set_bitmap(char type, unsigned int index)
{
    struct ext2_group_desc *blgrp = (struct ext2_group_desc *)(disk + 2048);
    unsigned char *start;
    if (type == 'i')
    {
        start = (unsigned char *)(disk + EXT2_BLOCK_SIZE * blgrp->bg_inode_bitmap);
    }
    if (type == 'b')
    {
        start = (unsigned char *)(disk + EXT2_BLOCK_SIZE * blgrp->bg_block_bitmap);
    }
    if (type != 'i' && type != 'b')
    {
        return;
    }
    unsigned int idx = index - 1;
    unsigned char *byte = start + idx / 8;
    *byte |= 1 << (idx % 8);
}

void unset_bitmap(char type, unsigned int index)
{
    struct ext2_group_desc *blgrp = (struct ext2_group_desc *)(disk + 2048);
    unsigned char *start;
    if (type == 'i')
    {
        start = (unsigned char *)(disk + EXT2_BLOCK_SIZE * blgrp->bg_inode_bitmap);
    }
    if (type == 'b')
    {
        start = (unsigned char *)(disk + EXT2_BLOCK_SIZE * blgrp->bg_block_bitmap);
    }
    if (type != 'i' && type != 'b')
    {
        return;
    }
    unsigned int idx = index - 1;
    unsigned char *byte = start + (idx / 8);
    *byte &= (unsigned int)(~(1 << (idx % 8)));
}


int check_bitmap(char type, unsigned int index)
{
    struct ext2_group_desc *blgrp = (struct ext2_group_desc *)(disk + 2048);
    unsigned char *start;
    if (type == 'i')
    {
        start = (unsigned char *)(disk + EXT2_BLOCK_SIZE * blgrp->bg_inode_bitmap);
    }
    if (type == 'b')
    {
        start = (unsigned char *)(disk + EXT2_BLOCK_SIZE * blgrp->bg_block_bitmap);
    }
    if (type != 'i' && type != 'b')
    {
        return -1;
    }
    unsigned int idx = index - 1;
    unsigned char *byte = start + (idx / 8);
    unsigned int ret = (*byte >> (idx % 8)) & 1;
    return ret;
}

void free_path(struct path *p)
{
    while (p != NULL)
    {
        free(p);
        p = p->next;
    }
}

char *return_path(char *file)
{
    char *filepath = file;
    int len = strlen(filepath);
    int lenght = 0;
    int i = len - 1;
    if (filepath[i] == '/')
    {
        filepath[i] = '\0';
    }

    while (i < len && i > -1)
    {
        if (filepath[i] == '/')
        {
            break;
        }
        lenght++;
        i--;
    }

    if ((len - lenght) > 1)
    {
        char *remainder = malloc(len - lenght + 1);
        memcpy(remainder, filepath, (len - lenght));
        remainder[len - lenght] = '\0';
        return remainder;
    }
    return NULL;
}


char *return_last_entry(char *file)
{
    char *filepath = file;
    int pos = 0; 
    int len = strlen(filepath);
    int lenght = 0;
    int i = len - 1;
    if (filepath[i] == '/')
    {
        filepath[i] = '\0';
    }

    while (i < len && i > -1)
    {
        if (filepath[i] == '/')
        {
            pos = i + 1;
            break;
        }
        lenght++;
        i--;
    }

    char *last_entry = malloc(lenght + 1);
    memcpy(last_entry, &(filepath[pos]), lenght);
    last_entry[lenght] = '\0';
    return last_entry;
}


unsigned int new_block()
{
    struct ext2_super_block *sb = (struct ext2_super_block *)(disk + 1024);
    struct ext2_group_desc *gd = (struct ext2_group_desc *)(disk + (2 * EXT2_BLOCK_SIZE));

    unsigned char *bm = (unsigned char *)(disk + EXT2_BLOCK_SIZE * gd->bg_block_bitmap);
    int size = sb->s_blocks_count / 8;
    int bit = find_free_block(bm, size);
    if (bit < 0)
    {
        return -1;
    }
    int block = bit + sb->s_first_data_block;

    unsigned char *byte = bm + bit / 8;
    *byte |= 1 << (bit % 8);

    gd->bg_free_blocks_count--;
    sb->s_free_blocks_count--;
    return block;
}


unsigned int new_inode()
{
    struct ext2_super_block *sb = (struct ext2_super_block *)(disk + 1024);
    struct ext2_group_desc *gd = (struct ext2_group_desc *)(disk + (2 * EXT2_BLOCK_SIZE));

    unsigned char *bm = (unsigned char *)(disk + EXT2_BLOCK_SIZE * gd->bg_inode_bitmap);
    int size = sb->s_inodes_count / 8;
    int bit = find_free_block(bm, size);

    if (bit < 0)
    {
        return -1;
    }
    int inode = bit + 1;

    unsigned char *byte = bm + bit / 8;
    *byte |= 1 << (bit % 8);

    gd->bg_free_inodes_count--;
    sb->s_free_inodes_count--;

    return inode;
}


int insert_dir_entry(unsigned int target, struct ext2_dir_entry *dir)
{
    int i;
    struct ext2_inode *target_inode = find_inode(target);
    int size_dir = sizeof(struct ext2_dir_entry) + dir->name_len;

    for (i = 0; i < 12; i++)
    {

        if (target_inode->i_block[i])
        {

            int size = 0;
            int rec = 0;
            struct ext2_dir_entry *dir_entry = (struct ext2_dir_entry *)(disk + EXT2_BLOCK_SIZE * target_inode->i_block[i]);

            while (size < EXT2_BLOCK_SIZE)
            {
                size += dir_entry->rec_len;

                int act_size = sizeof(struct ext2_dir_entry) + dir_entry->name_len;
                int padding = dir_entry->rec_len - act_size;

                if (strncmp(dir->name, dir_entry->name, dir->name_len) == 0)
                {
                    perror("file already in path");
                    return EEXIST;
                }

                if (act_size < dir_entry->rec_len && padding >= size_dir)
                {
                    dir_entry->rec_len = act_size;
                    dir->rec_len = padding;
                    dir_entry = (void *)dir_entry + dir_entry->rec_len;
                    memcpy(dir_entry->name, dir->name, dir->name_len);
                    dir_entry->name[dir->name_len] = '\0';
                    dir_entry->name_len = dir->name_len;
                    dir_entry->rec_len = dir->rec_len;
                    dir_entry->inode = dir->inode;
                    dir_entry->file_type = dir->file_type;
                    struct ext2_inode *link = find_inode(dir->inode);
                    link->i_links_count++;
                    return 0;
                }
                rec += dir_entry->rec_len;
                dir_entry = (void *)dir_entry + dir_entry->rec_len;
            }
            continue;
        }
        else
        {

            target_inode->i_block[i] = new_block();
            target_inode->i_blocks = target_inode->i_blocks + 2;
            target_inode->i_size++;
            struct ext2_dir_entry *bl = (struct ext2_dir_entry *)(disk + EXT2_BLOCK_SIZE * target_inode->i_block[i]);
            bl->rec_len = EXT2_BLOCK_SIZE;
            memcpy(bl->name, dir->name, dir->name_len);
            bl->name[dir->name_len] = '\0';
            bl->name_len = dir->name_len;
            bl->inode = dir->inode;
            bl->file_type = dir->file_type;
            struct ext2_inode *link = find_inode(dir->inode);
            link->i_links_count++;
            return 0;
        }
    }
    perror(" No space found in dir");
    return -1;
}



