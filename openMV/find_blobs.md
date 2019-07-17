# 寻找色块find_blobs参数手册
```
image.find_blobs(
  thresholds, roi=Auto, x_stride=2, y_stride=1, invert=False,area_threshold=10, 
  pixels_threshold=10, merge=False, margin=0, threshold_cb=None, merge_cb=None
)
```
- thresholds是颜色的阈值。这个参数是一个列表，可以包含多个颜色。
- roi是感兴趣区。
- x_stride是查找的色块的x方向上最小宽度的像素，默认为2。
- y_stride是查找的色块的y方向上最小宽度的像素，默认为1。
- invert反转阈值，把阈值以外的颜色作为阈值进行查找。
- area_threshold面积阈值，如果色块框起来的面积小于这个值，会被过滤掉。
- pixels_threshold像素个数阈值，如果色块像素数量小于这个值，会被过滤掉。
- merge合并，如果设置为True，那么合并所有重叠的blob为一个。
- margin边界，如果设置为1，那么两个blobs如果间距1个像素点，也会被合并。
