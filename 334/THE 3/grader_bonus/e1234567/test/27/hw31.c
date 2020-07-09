#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>
#include "ext21.h"
#include <errno.h>

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
// #define BLOCK_OFFSET(block) (BASE_OFFSET + BASE_OFFSET + (block - 1) * block_size)

unsigned char *image_map;

int BLOCK_OFFSET(int block)
{
    int a = 1 + 2048 / (block_size + 1);
    return (2 * BASE_OFFSET + (block - a) * block_size);
}

struct dir_path
{
    char name[30];
    int isFile;
    struct dir_path *next;
};

struct ext2_inode *find_inode(unsigned int idx)
{
    struct ext2_super_block *super_block = (struct ext2_super_block *)(image_map + BASE_OFFSET);

    int i = (idx - 1) / super_block->s_inodes_per_group;

    struct ext2_group_desc *group_descriptor_block = (struct ext2_group_desc *)(image_map + (BASE_OFFSET + BASE_OFFSET) + (i * sizeof(struct ext2_group_desc)));

    unsigned char *inode_table = (unsigned char *)(image_map + BLOCK_OFFSET(group_descriptor_block->bg_inode_table));
    int a = idx % super_block->s_inodes_per_group;
    return (struct ext2_inode *)(inode_table + (sizeof(struct ext2_inode) * (a - 1)));
}

struct ext2_dir_entry *search_dir(struct ext2_dir_entry *directory, char *entry)
{
    struct ext2_dir_entry *ext2_dir = directory;
    int size = 0;

    while (size < block_size)
    {
        size += ext2_dir->rec_len;

        if (strncmp(entry, ext2_dir->name, ext2_dir->name_len) == 0)
        {
            return ext2_dir;
        }
        ext2_dir = (void *)ext2_dir + ext2_dir->rec_len;
    }
    return NULL;
}

struct dir_path *divide_dir_path(char *dir_path)
{
    int isFile = 1;

    int len = strlen(dir_path);

    if (dir_path[len - 1] == '/')
    {
        dir_path[len - 1] = '\0';
        isFile = 0;
    }

    struct dir_path *head = NULL;
    struct dir_path *tail;

    int i = 1;
    int j = 1;
    int lenght;
    while (i < len)
    {
        if (dir_path[i] == '/')
        {

            if (head == NULL)
            {
                head = malloc(sizeof(struct dir_path));
                tail = head;
            }
            else
            {

                tail->next = malloc(sizeof(struct dir_path));
                tail = tail->next;
            }
            tail->next = NULL;
            lenght = i - j;
            tail->isFile = 0;
            memcpy(tail->name, &(dir_path[j]), lenght);
            tail->name[lenght] = 0;
            j = i + 1;
        }
        i++;
    }
    if (j != i)
    {
        if (head == NULL)
        {
            head = malloc(sizeof(struct dir_path));
            tail = head;
        }
        else
        {
            tail->next = malloc(sizeof(struct dir_path));
            tail = tail->next;
        }

        tail->next = NULL;
        tail->isFile = 0;
        lenght = i - j;
        memcpy(tail->name, &(dir_path[j]), lenght);
        tail->name[lenght] = '\0';
    }
    if (isFile)
    {
        tail->isFile = 1;
    }
    return head;
}

int locate_directory(struct dir_path *head)
{
    int i = EXT2_ROOT_INO;
    struct ext2_group_desc *blgrp = (struct ext2_group_desc *)(image_map + BASE_OFFSET + BASE_OFFSET);
    unsigned char *itable = (unsigned char *)(image_map + BLOCK_OFFSET(blgrp->bg_inode_table));

    while (head != NULL)
    {
        if (i > 2 && i < EXT2_GOOD_OLD_FIRST_INO)
        {
            break;
        }
        struct ext2_inode *inode = (struct ext2_inode *)(itable + sizeof(struct ext2_inode) * (i - 1));

        if (inode->i_size == 0)
        {
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
                struct ext2_dir_entry *ext2_dir = (struct ext2_dir_entry *)(image_map + BLOCK_OFFSET(block_no));
                struct ext2_dir_entry *found = search_dir(ext2_dir, entry);
                if (found)
                {
                    mark = 1;
                    i = found->inode;
                    if (head->next == NULL)
                    {
                        return i;
                    }
                    struct dir_path *new = head;
                    head = head->next;
                    free(new);
                    break;
                }
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

void free_dir_path(struct dir_path *p)
{
    while (p != NULL)
    {
        free(p);
        p = p->next;
    }
}

char *return_dir_path(char *file)
{
    char *filedir_path = file;
    int len = strlen(filedir_path);
    int lenght = 0;
    int i = len - 1;
    if (filedir_path[i] != '/')
    {
        filedir_path[i + 1] = '/';
        filedir_path[i + 2] = '\0';
        i++;
        len++;
    }

    while (i < len && i > -1)
    {
        if (filedir_path[i] == '/')
        {
            break;
        }
        lenght++;
        i--;
    }

    if ((len - lenght) > 1)
    {
        char *remainder = malloc(len - lenght + 1);
        memcpy(remainder, filedir_path, (len - lenght));
        remainder[len - lenght] = '\0';
        return remainder;
    }
    return NULL;
}

unsigned int new_block(int inode)
{

    struct ext2_super_block *super_block = (struct ext2_super_block *)(image_map + BASE_OFFSET);

    int i = (inode - 1) / super_block->s_inodes_per_group;
    // i = 0;
    struct ext2_group_desc *group_descriptor_block = (struct ext2_group_desc *)(image_map + (BASE_OFFSET + BASE_OFFSET) + (i * sizeof(struct ext2_group_desc)));

    unsigned char *bit_map = (unsigned char *)(image_map + BLOCK_OFFSET(group_descriptor_block->bg_block_bitmap));
    int size = super_block->s_blocks_per_group / 8;

    int free_block = find_free_block(bit_map, size);
    if (free_block >= 0)
    {
        free_block += (i)*super_block->s_blocks_per_group;
    }
    // i = 1;
    while (free_block < 0)
    {
        group_descriptor_block = (struct ext2_group_desc *)(image_map + (BASE_OFFSET + BASE_OFFSET) + (i * sizeof(struct ext2_group_desc)));
        bit_map = (unsigned char *)(image_map + BLOCK_OFFSET(group_descriptor_block->bg_block_bitmap));
        free_block = find_free_block(bit_map, size);
        if (free_block >= 0)
        {
            free_block += (i)*super_block->s_blocks_per_group;
            break;
        }
        i++;
    }
    int block = free_block + super_block->s_first_data_block;

    if (free_block >= super_block->s_blocks_per_group)
    {
        free_block -= (i)*super_block->s_blocks_per_group;
    }

    unsigned char *byte = bit_map + free_block / 8;
    *byte |= 1 << (free_block % 8);

    group_descriptor_block->bg_free_blocks_count--;
    super_block->s_free_blocks_count--;
    return block;
}

unsigned int new_inode()
{
    struct ext2_super_block *super_block = (struct ext2_super_block *)(image_map + BASE_OFFSET);
    // printf("asd\n");
    // fflush(stdout);

    struct ext2_group_desc *group_descriptor_block = (struct ext2_group_desc *)(image_map + (BASE_OFFSET + BASE_OFFSET));
    // printf("gd : %d\n", group_descriptor_block->bg_inode_bitmap);
    // fflush(stdout);
    unsigned char *bit_map = (unsigned char *)(image_map + BLOCK_OFFSET(group_descriptor_block->bg_inode_bitmap));
    int size = super_block->s_inodes_per_group / 8;
    // printf("size : %d \n", size);
    int free_block = find_free_block(bit_map, size);
    // printf("gd : %d\n", free_block);
    // fflush(stdout);
    int i = 1;
    while (free_block < 0)
    {
        group_descriptor_block = (struct ext2_group_desc *)(image_map + (BASE_OFFSET + BASE_OFFSET) + (i * sizeof(struct ext2_group_desc)));
        bit_map = (unsigned char *)(image_map + BLOCK_OFFSET(group_descriptor_block->bg_inode_bitmap));
        free_block = find_free_block(bit_map, size);
        if (free_block >= 0)
        {
            // printf("gd : %d %d\n", free_block, i);
            free_block += (i)*super_block->s_inodes_per_group;
            break;
        }
        i++;
    }
    int inode = free_block + 1;
    if (free_block >= super_block->s_inodes_per_group)
    {
        free_block -= (i)*super_block->s_inodes_per_group;
    }

    unsigned char *byte = bit_map + free_block / 8;
    *byte |= 1 << (free_block % 8);
    // printf("inode number of group : %d \n", i);
    group_descriptor_block->bg_free_inodes_count--;
    super_block->s_free_inodes_count--;
    // printf("inode free: %d %d\n", super_block->s_free_inodes_count, group_descriptor_block->bg_free_inodes_count);

    return inode;
}

int insert_dir_entry(unsigned int target, struct ext2_dir_entry *ext2_dir)
{
    int i;
    struct ext2_inode *copy_inode = find_inode(target);
    int size_dir = 9 + ext2_dir->name_len;
    //printf("asd");

    for (i = 0; i < 12; i++)
    {
        if (copy_inode->i_block[i])
        {
            // printf("dir block : %d \n", copy_inode->i_block[i]);
            int size = 0;
            struct ext2_dir_entry *dir_entry = (struct ext2_dir_entry *)(image_map + BLOCK_OFFSET(copy_inode->i_block[i]));

            while (size < block_size)
            {
                size += dir_entry->rec_len;
                int act_size = 9 + dir_entry->name_len;
                int actual_size = (1 + ((act_size - 1) / 4)) * 4;
                int padding = dir_entry->rec_len - actual_size;
                // printf("dir : %s %d\n", dir_entry->name, dir_entry->name_len);
                if (actual_size < dir_entry->rec_len && padding >= size_dir)
                {
                    dir_entry->rec_len = actual_size;
                    ext2_dir->rec_len = padding;
                    // printf("dir : %s %d\n", dir_entry->name, dir_entry->name_len);
                    dir_entry = (void *)dir_entry + dir_entry->rec_len;
                    memcpy(dir_entry->name, ext2_dir->name, ext2_dir->name_len);
                    dir_entry->name[ext2_dir->name_len] = '\0';
                    dir_entry->name_len = ext2_dir->name_len;
                    dir_entry->rec_len = ext2_dir->rec_len;
                    dir_entry->inode = ext2_dir->inode;
                    dir_entry->file_type = ext2_dir->file_type;
                    // printf("dir : %s %d %d %d\n", dir_entry->name, dir_entry->name_len, dir_entry->inode, dir_entry->rec_len);
                    return 0;
                }
                dir_entry = (void *)dir_entry + dir_entry->rec_len;
            }
            continue;
        }
        else
        {
            copy_inode->i_block[i] = new_block(target);
            copy_inode->i_size++;
            struct ext2_dir_entry *bl = (struct ext2_dir_entry *)(image_map + BLOCK_OFFSET(copy_inode->i_block[i]));
            bl->rec_len = block_size;
            memcpy(bl->name, ext2_dir->name, ext2_dir->name_len);
            bl->name[ext2_dir->name_len] = '\0';
            bl->name_len = ext2_dir->name_len;
            bl->inode = ext2_dir->inode;
            bl->file_type = ext2_dir->file_type;
            return 0;
        }
    }
    return -1;
}

int readInode(int file_descriptor, int index, struct ext2_group_desc group, struct ext2_inode *inode)
{
    lseek(file_descriptor, group.bg_inode_table * block_size + (index - 1) * sizeof(*inode), SEEK_SET);
    read(file_descriptor, inode, sizeof(*inode));
    return 0;
}

int readBlock(int file_descriptor, int index, struct ext2_dir_entry *ext2_dir, int i)
{
    lseek(file_descriptor, index * block_size + i, SEEK_SET);
    read(file_descriptor, ext2_dir, sizeof(*ext2_dir));
    return 0;
}

int readSuperBlock(int file_descriptor, struct ext2_super_block *super)
{
    lseek(file_descriptor, BASE_OFFSET, SEEK_SET);
    read(file_descriptor, super, sizeof(*super));

    return 0;
}

int readGroupBlock(int file_descriptor, struct ext2_group_desc *group)
{
    lseek(file_descriptor, BASE_OFFSET + BASE_OFFSET, SEEK_SET);
    read(file_descriptor, group, sizeof(*group));

    return 0;
}

int main(int argc, char **argv)
{
    // printf("1\n");
    // fflush(stdout);
    struct stat fileStat;
    if (stat(argv[2], &fileStat) < 0)
        return 1;
    // printf("2\n");
    // fflush(stdout);
    struct ext2_super_block super;
    struct ext2_group_desc group;

    unsigned int directory_inode_index;

    int file_descriptor = open(argv[1], O_RDWR);

    readSuperBlock(file_descriptor, &super);
    readGroupBlock(file_descriptor, &group);

    // printf("3\n");
    // fflush(stdout);
    block_size = 1024 << super.s_log_block_size;

    image_map = mmap(NULL, (super.s_blocks_count * block_size), PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);

    FILE *f = fopen(argv[2], "r");

    char *all_file = NULL;
    if (f != NULL)
    {
        fseek(f, 0, SEEK_END);
        int length = ftell(f);
        fseek(f, 0, SEEK_SET);
        all_file = malloc(length + 1);
        fread(all_file, length, 1, f);
        fclose(f);
    }
    // printf("4\n");
    // fflush(stdout);
    if (argv[3][0] == '/')
    {
        char *directory_path = NULL;

        directory_path = return_dir_path(argv[3]);

        struct dir_path *dest = NULL;

        if (directory_path == NULL)
        {
            directory_inode_index = EXT2_ROOT_INO;
        }
        else
        {
            dest = divide_dir_path(directory_path);
            directory_inode_index = locate_directory(dest);
        }
        free_dir_path(dest);
    }
    else
    {
        directory_inode_index = atoi(argv[3]);
    }
    // printf("5\n");
    // fflush(stdout);
    struct ext2_inode *dest_inode = find_inode(directory_inode_index);
    // printf("6\n");
    // fflush(stdout);
    if (dest_inode && (dest_inode->i_mode & EXT2_S_IFDIR))
    {
        struct ext2_dir_entry *ext2_dir = malloc(sizeof(struct ext2_dir_entry));
        ext2_dir->inode = new_inode();
        printf("%d ", ext2_dir->inode);
        strncpy(ext2_dir->name, argv[2], strlen(argv[2]));
        // printf("7\n");
        // fflush(stdout);
        ext2_dir->name_len = strlen(argv[2]);
        ext2_dir->rec_len = 0;
        ext2_dir->file_type = EXT2_FT_REG_FILE;

        int result = insert_dir_entry(directory_inode_index, ext2_dir);
        // printf("block size : %d %d %d %d %d %d\n", block_size, super.s_blocks_count, super.s_inodes_count, super.s_inodes_per_group, super.s_blocks_per_group, super.s_first_data_block);
        // fflush(stdout);

        // struct ext2_inode *copy_inode = find_inode(2);
        // int size = 0;

        // struct ext2_dir_entry *dir_entry = (struct ext2_dir_entry *)(image_map + BLOCK_OFFSET(copy_inode->i_block[0]));

        // while (size < block_size)
        // {
        //     printf("%s %d %d %d %d\n", dir_entry->name, dir_entry->inode, dir_entry->name_len, dir_entry->rec_len, dir_entry->file_type);
        //     size += dir_entry->rec_len;
        //     dir_entry = (void *)dir_entry + dir_entry->rec_len;
        // }

        if (result > -1)
        {
            unsigned int inodeNumber = ext2_dir->inode;
            struct ext2_inode *copy_inode = find_inode(inodeNumber);

            copy_inode->i_mode = fileStat.st_mode;
            copy_inode->i_uid = fileStat.st_uid;
            copy_inode->i_gid = fileStat.st_gid;
            copy_inode->i_size = fileStat.st_size;
            copy_inode->i_blocks = fileStat.st_blocks;
            copy_inode->i_atime = fileStat.st_atime;
            copy_inode->i_mtime = fileStat.st_mtime;
            copy_inode->i_ctime = fileStat.st_ctime;
            copy_inode->i_dtime = 0;

            int size = 0;
            int file_size = strlen(all_file);
            int i = 0;
            int j = 0;
            int k = 0;
            int l = 0;

            int test, test2, test3;
            while (size < file_size)
            {
                if (i == 12)
                {
                    copy_inode->i_block[i] = new_block(inodeNumber);
                    // copy_inode->i_size++;
                    printf("%d,", copy_inode->i_block[i]);
                    while (size < file_size)
                    {
                        if (j == (block_size / 4))
                        {
                            i++;
                            copy_inode->i_block[i] = new_block(inodeNumber);
                            // copy_inode->i_size++;
                            printf("%d,", copy_inode->i_block[i]);
                            while (size < file_size)
                            {
                                if (k == (block_size / 4))
                                {
                                    j = 0;
                                    k = 0;
                                    l = 0;
                                    i++;
                                    copy_inode->i_block[i] = new_block(inodeNumber);
                                    // copy_inode->i_size++;
                                    printf("%d,", copy_inode->i_block[i]);
                                    while (size < file_size)
                                    {
                                        test = new_block(inodeNumber);
                                        printf("%d,", test);
                                        memcpy((image_map + BLOCK_OFFSET((copy_inode->i_block[i])) + 4 * j), &test, 4);
                                        while (size < file_size)
                                        {
                                            if (k == (block_size / 4))
                                            {
                                                k = 0;
                                                j++;
                                                break;
                                            }
                                            test2 = new_block(inodeNumber);
                                            printf("%d,", test2);
                                            memcpy((image_map + BLOCK_OFFSET(test) + 4 * k), &test2, 4);
                                            while (size < file_size)
                                            {
                                                if (l == (block_size / 4))
                                                {
                                                    l = 0;
                                                    k++;
                                                    break;
                                                }
                                                test3 = new_block(inodeNumber);
                                                printf("%d", test3);
                                                if (size + block_size < file_size)
                                                {
                                                    printf(",");
                                                }
                                                memcpy((image_map + BLOCK_OFFSET(test2) + 4 * l), &test3, 4);
                                                memcpy(image_map + BLOCK_OFFSET(test3), all_file, block_size);
                                                size += block_size;
                                                all_file = (void *)all_file + block_size;
                                                l++;
                                            }
                                            if (size >= file_size)
                                            {
                                                break;
                                            }
                                        }
                                        if (size >= file_size)
                                        {
                                            break;
                                        }
                                    }
                                    if (size >= file_size)
                                    {
                                        break;
                                    }
                                }
                                test = new_block(inodeNumber);
                                printf("%d,", test);
                                memcpy((image_map + BLOCK_OFFSET((copy_inode->i_block[i])) + 4 * k), &test, 4);
                                while (size < file_size)
                                {
                                    if (l == (block_size / 4))
                                    {
                                        l = 0;
                                        k++;
                                        break;
                                    }
                                    test2 = new_block(inodeNumber);
                                    printf("%d", test2);
                                    if (size + block_size < file_size)
                                    {
                                        printf(",");
                                    }
                                    memcpy((image_map + BLOCK_OFFSET(test) + 4 * l), &test2, 4);
                                    memcpy(image_map + BLOCK_OFFSET(test2), all_file, block_size);
                                    size += block_size;
                                    all_file = (void *)all_file + block_size;
                                    l++;
                                }
                                if (size >= file_size)
                                {
                                    break;
                                }
                            }
                            if (size >= file_size)
                            {
                                break;
                            }
                        } //indirect
                        test = new_block(inodeNumber);
                        printf("%d", test);
                        if (size + block_size < file_size)
                        {
                            printf(",");
                        }
                        memcpy(image_map + BLOCK_OFFSET(copy_inode->i_block[i]) + 4 * j, &test, 4);
                        memcpy(image_map + BLOCK_OFFSET(test), all_file, block_size);
                        size += block_size;
                        all_file = (void *)all_file + block_size;
                        j++;
                    }
                    if (size >= file_size)
                    {
                        break;
                    }
                }
                copy_inode->i_block[i] = new_block(inodeNumber);
                //copy_inode->i_size++;
                printf("%d", copy_inode->i_block[i]);
                if (size + block_size < file_size)
                {
                    printf(",");
                }
                memcpy((image_map + BLOCK_OFFSET(copy_inode->i_block[i])), all_file, block_size);
                size += block_size;
                all_file = (void *)all_file + block_size;
                i++;
            }
        }
    }

    //8308 103
    //24 9
    // char as[1024];
    // lseek(file_descriptor, BLOCK_OFFSET(8308), SEEK_SET);
    // read(file_descriptor, as, 1024);

    // printf("deneme : %d %s\n", block_size, as);

    // struct ext2_dir_entry *abc = (struct ext2_dir_entry *)(image_map + BLOCK_OFFSET(103));
    // int sizeo = 0;
    // while (sizeo < block_size)
    // {
    //     sizeo += abc->rec_len;
    //     printf("deneme : %d %d %d %s %d\n", abc->inode, abc->name_len, abc->rec_len, abc->name, abc->file_type);
    //     abc = (void *)abc + abc->rec_len;
    // }

    // struct ext2_inode *dest_inode2 = find_inode(12);
    // printf("inode2 %d \n", dest_inode2->i_block[0]);

    close(file_descriptor);
    return 1;
}
