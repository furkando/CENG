echo 13
./filecopy image.img file 2 2>> runtime_errors.txt
mkdir mnt
sudo chmod 755 mnt
sudo fuse-ext2 image.img mnt/ -o rw+ ;
./sstat mnt/file > a
./sstat file > b
diff a b > 13.stat 2>&1
rm a b
diff mnt/file file > 13.diff  2>&1
umount mnt
fsck.ext2 -n image.img > 13.fsck 2>&1
sleep 1

