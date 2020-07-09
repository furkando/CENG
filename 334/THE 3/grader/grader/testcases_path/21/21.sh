echo 21
./filecopy image.img file /pale/blue/dot 2>> runtime_errors.txt
mkdir mnt
sudo chmod 755 mnt
sudo fuse-ext2 image.img mnt/ -o rw+ ;
./sstat mnt/pale/blue/dot/file > a
./sstat file > b
diff a b > 21.stat 2>&1
rm a b
diff mnt/pale/blue/dot/file file > 21.diff 2>&1
umount mnt
fsck.ext2 -n image.img > 21.fsck 2>&1
sleep 1
