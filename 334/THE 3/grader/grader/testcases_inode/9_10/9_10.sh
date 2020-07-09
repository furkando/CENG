echo 9_10
./filecopy image.img allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks28 2 2>> runtime_errors.txt
mkdir mnt
sudo chmod 755 mnt
sudo fuse-ext2 image.img mnt/ -o rw+ ;
./sstat mnt/allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks28 > a
./sstat allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks28 > b
diff a b > 9.stat 2>&1
rm a b
diff mnt/allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks28 allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks28 > 9.diff 2>&1
umount mnt
fsck.ext2 -n image.img > 9.fsck 2>&1
sleep 1
cp image.img image.img.bak
./filecopy image.img allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks30 2 2>> runtime_errors.txt
sudo fuse-ext2 image.img mnt/ -o rw+ ;
./sstat mnt/allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks30 > a
./sstat allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks30 > b
diff a b > 10.stat 2>&1
rm a b
diff mnt/allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks30 allblockgroupsstillcontaintheblockbitmapinodebitmapinodetableanddatablocks30 > 10.diff 2>&1
umount mnt
fsck.ext2 -n image.img > 10.fsck 2>&1
sleep 1
