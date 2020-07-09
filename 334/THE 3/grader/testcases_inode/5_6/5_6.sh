echo 5_6
./filecopy image.img file 2 2>> runtime_errors.txt
mkdir mnt
sudo chmod 755 mnt
sudo fuse-ext2 image.img mnt/ -o rw+ ;
./sstat mnt/file > a
./sstat file > b
diff a b > 5.stat 2>&1
rm a b
diff mnt/file file > 5.diff 2>&1
umount mnt
fsck.ext2 -n image.img > 5.fsck 2>&1
sleep 1
cp image.img image.img.bak
./filecopy image.img file2 2 2>> runtime_errors.txt
sudo fuse-ext2 image.img mnt/ -o rw+ ;
./sstat mnt/file2 > a
./sstat file2 > b
diff a b > 6.stat 2>&1
rm a b
diff mnt/file2 file2 > 6.diff 2>&1
umount mnt
fsck.ext2 -n image.img > 6.fsck 2>&1
sleep 1


