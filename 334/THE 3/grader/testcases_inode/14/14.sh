echo 14
./filecopy image.img file 11 2>> runtime_errors.txt
mkdir mnt
sudo chmod 755 mnt
sudo fuse-ext2 image.img mnt/ -o rw+
./sstat mnt/lost+found/file > a
./sstat file > b
diff a b > 14.stat 2>&1
rm a b
diff mnt/lost+found/file file > 14.diff 2>&1
umount mnt
fsck.ext2 -n image.img > 14.fsck 2>&1
sleep 1
