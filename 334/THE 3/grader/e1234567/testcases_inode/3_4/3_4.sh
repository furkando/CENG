echo 3_4
./filecopy image.img file 11 2>> runtime_errors.txt
mkdir mnt
sudo chmod 755 mnt
sudo fuse-ext2 image.img mnt/ -o rw+
./sstat mnt/lost+found/file > a
./sstat file > b  
diff a b > 3.stat 2>&1
rm a b
diff mnt/lost+found/file file > 3.diff 2>&1
umount mnt
fsck.ext2 -n image.img > 3.fsck 2>&1
sleep 1
cp image.img image.img.bak
./filecopy image.img file2 11 2>> runtime_errors.txt
sudo fuse-ext2 image.img mnt/ -o rw+
./sstat mnt/lost+found/file2 > a
./sstat file2 > b
diff a b > 4.stat 2>&1
rm a b
diff mnt/lost+found/file2 file2 > 4.diff 2>&1
umount mnt
fsck.ext2 -n image.img > 4.fsck 2>&1
sleep 1



