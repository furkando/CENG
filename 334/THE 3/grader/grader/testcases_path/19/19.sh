echo 19
./filecopy image.img allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks28 /pale/blue/dot 2>> runtime_errors.txt
mkdir mnt
sudo chmod 755 mnt
sudo fuse-ext2 image.img mnt/ -o rw+ ;
./sstat mnt/pale/blue/dot/allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks28 > a
./sstat allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks28 > b 
diff a b > 19.stat  2>&1
rm a b
diff mnt/pale/blue/dot/allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks28 allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks28  > 19.diff  2>&1
umount mnt
fsck.ext2 -n image.img > 19.fsck  2>&1
sleep 1