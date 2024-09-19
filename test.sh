#/bin/bash
echo "Initial image size $(du --apparent-size -h test.bmp || (echo ERROR && exit))"
./build test.bmp -o test.bmp.comp
echo "Compressed image size $(du --apparent-size -h test.bmp.comp || (echo ERROR && exit))"
