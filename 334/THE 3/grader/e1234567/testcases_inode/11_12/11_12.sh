echo 11_12
./filecopy image.img allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks28 14 2>> runtime_errors.txt
mkdir mnt
sudo chmod 755 mnt
sudo fuse-ext2 image.img mnt/ -o rw+ ;
./sstat mnt/pale/blue/dot/allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks28 > a
./sstat allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks28 > b
diff a b > 11.stat 2>&1
rm a b
diff mnt/pale/blue/dot/allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks28  allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks28 > 11.diff 2>&1
umount mnt
fsck.ext2 -n image.img > 11.fsck 2>&1
sleep 1
cp image.img image.img.bak
./filecopy image.img allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks30 14 2>> runtime_errors.txt
sudo fuse-ext2 image.img mnt/ -o rw+ ;
./sstat mnt/pale/blue/dot/allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks30 > a
./sstat allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks30 > b
diff a b > 12.stat 2>&1
rm a b
diff mnt/pale/blue/dot/allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks30  allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks30  > 12.diff 2>&1
umount mnt
fsck.ext2 -n image.img > 12.fsck 2>&1
sleep 1

