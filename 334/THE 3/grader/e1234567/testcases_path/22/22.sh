echo 22
./filecopy image.img file / 2>> runtime_errors.txt
mkdir mnt
sudo chmod 755 mnt
sudo fuse-ext2 image.img mnt/ -o rw+ ;
./sstat mnt/file > a
./sstat file > b
diff a b > 22.stat 2>&1
rm a b
diff mnt/file file > 22.diff 2>&1
umount mnt
fsck.ext2 -n image.img > 22.fsck 2>&1
sleep 1

