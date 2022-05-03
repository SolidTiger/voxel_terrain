### Blog for project in DH2323

## First blog 3/5
I have set upp the project with all the libraries. I started with creating a quad in opengl
this quad had a texture on it that we can set the pixels on through a buffer of pixels on the 
cpu. I also managed to draw my first terrain after some debugging. I had a hard time getting 
the projection right and orientating the world. But after a while i got it working and 
managed to draw my first terrain. The color on the terrain is just determined by the height. My next 
step will be to implement a depth buffer. Right now I am using the painters algorithm and drawing
back to front with a lot of unnecessary drawing. I will also try to add camera rotation and 
movement besides up and down and also terrain color from a color map.

![Image](images/first_terrain.png)