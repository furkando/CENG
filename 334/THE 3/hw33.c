#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include "ext21.h"

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

int groupCount;

int readBlock(int fd, int index, struct ext2_dir_entry *dir, int i)
{
    lseek(fd, index * EXT2_BLOCK_SIZE + i, SEEK_SET);
    read(fd, dir, sizeof(*dir));
    return 0;
}

int readInode(int fd, int index, struct ext2_group_desc group, struct ext2_inode *inode)
{
    lseek(fd, group.bg_inode_table * EXT2_BLOCK_SIZE + (index - 1) * sizeof(*inode), SEEK_SET);
    read(fd, inode, sizeof(*inode));
    return 0;
}

int readSuperBlock(int fd, struct ext2_super_block *super)
{
    lseek(fd, BASE_OFFSET, SEEK_SET);
    read(fd, super, sizeof(*super));

    return 0;
}

int readGroupBlock(int fd, struct ext2_super_block super, int bgid, struct ext2_group_desc *group)
{
    lseek(fd, BASE_OFFSET + sizeof(super) + sizeof(group) * bgid, SEEK_SET);
    read(fd, group, sizeof(*group));

    return 0;
}

unsigned int *getInodes(int fd, struct ext2_super_block super, struct ext2_group_desc group, unsigned int *cnt)
{
    unsigned int *res, i;
    *cnt = 0;
    bmap *bitmap = (bmap *)malloc(EXT2_BLOCK_SIZE);
    res = malloc(EXT2_BLOCK_SIZE * 8 * sizeof(unsigned int));

    lseek(fd, group.bg_inode_bitmap * EXT2_BLOCK_SIZE, SEEK_SET);
    read(fd, bitmap, EXT2_BLOCK_SIZE);
    for (i = 0; i < super.s_inodes_per_group; i++)
    {
        if (BM_ISSET(i, bitmap))
        {
            res[(*cnt)++] = i + 1;
            // printf("inodes : %d\n", i + 1);
        }
    }

    free(bitmap);
    return res;
}

unsigned int *getBlocks(int fd, struct ext2_super_block super, struct ext2_group_desc group, unsigned int *cnt)
{
    unsigned int *res, i;
    *cnt = 0;
    bmap *bitmap = (bmap *)malloc(EXT2_BLOCK_SIZE);
    res = malloc(EXT2_BLOCK_SIZE * 8 * sizeof(unsigned int));

    lseek(fd, group.bg_block_bitmap * EXT2_BLOCK_SIZE, SEEK_SET);
    read(fd, bitmap, EXT2_BLOCK_SIZE);
    for (i = 0; i < super.s_blocks_count; i++)
    {
        if (BM_ISSET(i, bitmap))
        {
            res[(*cnt)++] = i + 1;
            // sprintf("blocks : %d\n", i + 1);
        }
    }

    free(bitmap);
    return res;
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

unsigned int new_block(int fd)
{
    struct ext2_super_block super;
    struct ext2_group_desc group;
    readSuperBlock(fd, &super);
    readGroupBlock(fd, super, 0, &group);
    unsigned char *bm;
    lseek(fd, group.bg_block_bitmap * EXT2_BLOCK_SIZE, SEEK_SET);
    read(fd, bm, EXT2_BLOCK_SIZE);

    int size = super.s_blocks_count / 8;
    int bit = find_free_block(bm, size);
    // block not found;
    if (bit < 0)
    {
        return -1;
    }
    int block = bit + super.s_first_data_block;

    unsigned char *byte = bm + bit / 8;
    *byte |= 1 << (bit % 8);

    group.bg_free_blocks_count--;
    super.s_free_blocks_count--;
    return block;
}

int insert_dir_entry(int fd, struct ext2_group_desc group, unsigned int target, struct ext2_dir_entry *dir)
{
    printf("ttt\n");

    int i;
    struct ext2_inode target_inode;
    // printf("lll\n");
    readInode(fd, target, group, &target_inode);
    // printf("aaa %d %d\n", target_inode.i_block[0], target);

    int size_dir = sizeof(struct ext2_dir_entry) + dir->name_len;

    unsigned int size;
    unsigned char block[EXT2_BLOCK_SIZE];
    // printf("aaa %d\n", target_inode.i_block[0]);

    for (i = 0; i < 12; i++)
    {

        if (target_inode.i_block[i])
        {

            int size = 0;
            int rec = 0;
            // printf("aaa\n");

            lseek(fd, target_inode.i_block[i] * EXT2_BLOCK_SIZE, SEEK_SET);
            read(fd, block, EXT2_BLOCK_SIZE);

            struct ext2_dir_entry *dir_entry = (struct ext2_dir_entry *)block;
            // printf("xxx\n");

            while (size < EXT2_BLOCK_SIZE)
            {
                size += dir_entry->rec_len;

                int act_size = sizeof(struct ext2_dir_entry) + dir_entry->name_len;
                int padding = dir_entry->rec_len - act_size;

                // printf("padding %d %d %d\n", padding, dir_entry->rec_len, act_size);

                if (strncmp(dir->name, dir_entry->name, dir->name_len) == 0)
                {
                    return 0;
                }

                if (act_size < dir_entry->rec_len && padding >= size_dir)
                {
                    dir_entry->rec_len = act_size;
                    lseek(fd, (target_inode.i_block[i]) * EXT2_BLOCK_SIZE + (EXT2_BLOCK_SIZE - padding - act_size), SEEK_SET);
                    write(fd, dir_entry, padding);
                    dir->rec_len = padding;
                    dir_entry = (void *)dir_entry + dir_entry->rec_len;
                    memcpy(dir_entry->name, dir->name, dir->name_len);
                    dir_entry->name[dir->name_len] = '\0';
                    dir_entry->name_len = dir->name_len;
                    dir_entry->rec_len = dir->rec_len;
                    dir_entry->inode = dir->inode;
                    dir_entry->file_type = dir->file_type;
                    struct ext2_inode link;
                    readInode(fd, dir->inode, group, &link);
                    link.i_links_count++;
                    // printf("test %d %d \n", target_inode.i_block[i], (EXT2_BLOCK_SIZE - padding));
                    lseek(fd, (target_inode.i_block[i]) * EXT2_BLOCK_SIZE + (EXT2_BLOCK_SIZE - padding), SEEK_SET);
                    write(fd, dir_entry, padding);
                    return 0;
                }
                rec += dir_entry->rec_len;
                dir_entry = (void *)dir_entry + dir_entry->rec_len;
            }
            continue;
        }
        else
        {

            target_inode.i_block[i] = new_block(fd);
            target_inode.i_blocks = target_inode.i_blocks + 2;
            target_inode.i_size++;
            lseek(fd, target_inode.i_block[i] * EXT2_BLOCK_SIZE, SEEK_SET);
            read(fd, block, EXT2_BLOCK_SIZE);
            struct ext2_dir_entry *bl = (struct ext2_dir_entry *)block;
            bl->rec_len = EXT2_BLOCK_SIZE;
            memcpy(bl->name, dir->name, dir->name_len);
            bl->name[dir->name_len] = '\0';
            bl->name_len = dir->name_len;
            bl->inode = dir->inode;
            bl->file_type = dir->file_type;
            struct ext2_inode link;
            readInode(fd, dir->inode, group, &link);
            link.i_links_count++;
            return 0;
        }
    }
    return -1;
}

int main(int argc, char **argv)
{
    struct stat fileStat;
    if (stat(argv[2], &fileStat) < 0)
        return 1;

    struct ext2_super_block super;
    struct ext2_group_desc group;
    struct ext2_inode inode;

    unsigned int fd, icnt, *inodes, bcnt, *blocks;
    unsigned int *btoInode, *blockCount;

    FILE *fp;
    char buffer[EXT2_BLOCK_SIZE];

    fp = fopen(argv[2], "r");

    fd = open(argv[1], O_RDWR);

    readSuperBlock(fd, &super);
    readGroupBlock(fd, super, 0, &group);
    inodes = getInodes(fd, super, group, &icnt);
    blocks = getBlocks(fd, super, group, &bcnt);
    // btoInode = malloc(sizeof(int) * (EXT2_BLOCK_SIZE * 8 + 1));
    // blockCount = malloc(sizeof(int) * (EXT2_BLOCK_SIZE * 8 + 1));
    groupCount = 1 + ((super.s_blocks_count - 1) / super.s_blocks_per_group);

    printf("s_blocks_count : %d \n", super.s_blocks_count);
    printf("s_blocks_per_group : %d \n", super.s_blocks_per_group);

    printf("s_inodes_per_group : %d \n", super.s_inodes_per_group);

    printf("total group number : %d \n", groupCount);

    // printf("nfree inode count: %d \n", icnt);
    // printf("nfree block count: %d \n", bcnt);

    printf("inode count: %d \n", super.s_inodes_count);
    printf("block count: %d \n", super.s_blocks_count);

    printf("free inode count: %d \n", group.bg_free_inodes_count);
    printf("free block count: %d \n", group.bg_free_blocks_count);

    // readInode(fd, 2, group, &inode);
    // printf("size : %d pointer : %d %d %hu %d \n", inode.i_size, inode.i_block[0], inode.i_mode, inode.i_blocks);
    // inode.i_blocks = 4;

    fseek(fp, 0, SEEK_SET);
    int seek = 0;
    while (!feof(fp))
    {
        fread(buffer, sizeof(buffer), 1, fp);
        lseek(fd, (2 * groupCount + 4 + 25 + seek) * EXT2_BLOCK_SIZE, SEEK_SET);
        write(fd, buffer, EXT2_BLOCK_SIZE);
        seek++;
    }
    fclose(fp);

    struct ext2_inode *newInode;
    newInode = malloc(sizeof(struct ext2_inode));
    //  printf("File Size: \t\t%d bytes\n", fileStat.st_size);

    printf("xxx\n");
    newInode->i_mode = fileStat.st_mode;
    printf("123\n");
    newInode->i_uid = fileStat.st_uid;
    newInode->i_gid = fileStat.st_gid;
    newInode->i_size = fileStat.st_size;
    newInode->i_blocks = fileStat.st_blocks;
    newInode->i_atime = fileStat.st_atime;
    newInode->i_mtime = fileStat.st_mtime;
    newInode->i_ctime = fileStat.st_ctime;
    newInode->i_block[0] = 26;
    printf("yyy\n");

    lseek(fd, group.bg_inode_table * EXT2_BLOCK_SIZE + 11 * sizeof(newInode), SEEK_SET);
    write(fd, (void *)newInode, sizeof(newInode));

    readInode(fd, 12, group, &inode);
    printf("size : %d pointer : %d %d %hu \n", inode.i_size, inode.i_block[0], inode.i_mode, inode.i_blocks);

    struct ext2_dir_entry *entry, *newDirectory;
    unsigned int size;
    unsigned char block[EXT2_BLOCK_SIZE];
    newDirectory = malloc(sizeof(struct ext2_dir_entry));
    strncpy(newDirectory->name, argv[2], strlen(argv[2]));
    newDirectory->name[strlen(argv[2])] = '\0';
    newDirectory->name_len = strlen(argv[2]);
    newDirectory->rec_len = 0;
    newDirectory->inode = 23;
    newDirectory->file_type = EXT2_FT_REG_FILE;

    // readBlock(fd, 9, &dir, 0);
    // printf("test : %d %d %d %d %s\n", dir.inode, dir.rec_len, dir.name_len, dir.file_type, file_name);
    insert_dir_entry(fd, group, 2, newDirectory);

    lseek(fd, 9 * EXT2_BLOCK_SIZE, SEEK_SET);
    read(fd, block, EXT2_BLOCK_SIZE);
    size = 0;
    entry = (struct ext2_dir_entry *)block;

    char file_name[EXT2_NAME_LEN + 1];
    while (size < EXT2_BLOCK_SIZE)
    {
        memcpy(file_name, entry->name, entry->name_len);
        file_name[entry->name_len] = '\0';
        printf("%d %s %d %d\n", entry->inode, file_name, entry->name_len, entry->rec_len, entry->file_type);
        size += entry->rec_len;
        entry = (void *)entry + entry->rec_len;
    }
    return 0;
}