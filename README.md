##### 1. [Software](#software)
##### 2. [PNGs](#pngs)
##### 3. [xcursorgen](#xcursorgen)
##### 4. [THE FUN](#the-fun)
##### 5. [How I do it](#how-i-do-it)
##### 6. [Porting from Windows](#porting-from-windows)
##### 7. [Check current cursor in use](#check-current-cursor-in-use)
##### 8. [SVGs](#svgs)
##### 9. [Special Thanks](#special-thanks)

---
<a name="software"></a>
## Software
`imagemagick` for converting images to different sizes

`xcursorgen` for creating **Xcursor** files 

<a name="pngs"></a>
##  PNGs

First of all, you have to convert .png files to appropriate resolutions. 

You can do it with `imagemagick`:

`convert default.png -resize 32x32 x1/default.png`

`convert default.png -resize 40x40 x1_25/default.png`

`convert default.png -resize 48x48 x1_5/default.png`

`convert default.png -resize 64x64 x2/default.png`


It will convert .png pictures to four resolutions (**32x32**, **40x40**, **48x48** and **64x64**) and put them into folders: **x1**, **x1_25**, **x1_5** and **x2** respectively.

<a name="xcursorgen"></a>
##  xcursorgen

Then you need to write config file for `xcursorgen`.

##### For example: 

`default.cursor`

```
24 4 4 x1/default.png
30 5 5 x1_25/default.png
36 6 6 x1_5/default.png
48 8 8 x2/default.png
```

1st column: Sizes of the cursor.

2nd and 3rd columns: For **default** cursor it will be coordinates of pointy end, for **pointer** (hand with finger) it will be coordinates of the end of the finger, etc. 

4th column: Path to images with corresponding sizes.

##### Then you execute these commands for every config:

`xcursorgen default.cursor output/default`

`xcursorgen pointer.cursor output/pointer`

`etc`

<a name="the-fun"></a>
## THE FUN

Now you have to create symbolic links, because there is absolutely no consistency in cursor namings. For example, some applications use **arrow** cursor instead of the **default**. So we have to symlink it.

`ln -sf default arrow`

All of them can be found here: https://git.auteiy.me/dmitry/cursorsPosy/blob/master/Black/cursorList

Left column contains cursors that don't exist and have to be symlinked.

<a name="how-i-do-it"></a>
## How I do it

1. I convert PNGs using `convertPNGs/convertsize.sh` script. It will convert .png pictures to four resolutions (**32x32**, **40x40**, **48x48** and **64x64**) and put them into folders: **x1**, **x1_25**, **x1_5** and **x2** respectively. 
2. Then I take cursors from folder **x_2** only and put them in corresponding folders for my theme: **Black**, **White**, **Black** and **BlackTiny**.
3. Then I run `create.sh` script (can be found [here](https://git.auteiy.me/dmitry/cursorsPosy/blob/master/create.sh)), that converts PNGs from **x2** folder to different sizes mentioned above and puts them in folders corresponding to dimensions, then it creates **xcursor** files using `xcursorgen`, and then it creates symbolic links from `cursorList` file.
4. Done

<a name="porting-from-windows"></a>
## Porting from Windows

Windows uses .cur and .ani formats for its cursors. 

To convert them to PNG put files into `convertWindows/inputs` folder and run `convertWindows/convert.sh` script. PNGs will be in `convertWindows/outputs` folder. 

Software has some bugs, but works fine. 

Converting with `imagemagick` doesn't work, at least for me.
<a name="check-current-cursor-in-use"></a>"
## Check current cursor in use
If you find that cursor fallbacks to default theme, instead of using your own, you might use `localeMouse` software to get names of missing cursors, so you can symlink them (or draw). 

You'll have to open `mouseLang.pro` in `qtcreator` and build it yourself. Then run newly compiled file and trigger cursor's fallback. 


<a name="svgs"></a>
## SVGs

If you have your cursors in SVG format, then you might want to check [Capitaine Cursors](https://github.com/keeferrourke/capitaine-cursors) theme, since it uses SVGs files to convert from. 
<a name="special-thanks"></a>
### Special Thanks

##### To [Keefer Rourke](https://github.com/keeferrourke) for his [Capitaine Cursors](https://github.com/keeferrourke/capitaine-cursors) theme, from which I took scripts and whole repository structure.

##### To [Patrick Gordon](https://github.com/paddygord) for creating [ani2ico](https://github.com/paddygord/cursor-converter/) converter from Windows's formats 

##### To FisHlaBsoMAN for creating [localemouse](https://bitbucket.org/group-of-coders/localemouse/src/master/) software that check current cursor in use. 
