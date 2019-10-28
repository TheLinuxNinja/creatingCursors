mkdir x1
mkdir x1_25
mkdir x1_5
mkdir x2

rm -rf x1/*
rm -rf x1_25/*
rm -rf x1_5/*
rm -rf x2/*

for file in *.png; do convert $file -resize 48x48 x1_5/$file; done

for file in *.png; do convert $file -resize 40x40 x1_25/$file; done

for file in *.png; do convert $file -resize 64x64 x2/$file; done

for file in *.png; do convert $file -resize 32x32 x1/$file; done
