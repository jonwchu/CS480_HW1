README
------
Jonathan Chu
CSCI 480 - Computer Graphics
Project 1 - Height Fields

Controls
--------
Mouse: Rotate (LMB/MMB)

H Key: Single Color Gradient Shading
J Key: Rainbow Gradient Shading
K Key: Wireframe (Scale Mesh to see wireframe, at initial view, it looks too jumbled)
L Key: Points

O Key: Increase Height Field
P Key: Decrease Height Field

Notes
-----
-My code can only accomodate image files of size 256x256 pixels
 this is because of the way I implemented my vertex calculation
 I created a vertex buffer, in which each vertex represented
 a corner of a pixel in the input image. Because of this,
 I needed to use a const int for the size of the buffer,
 so I assumed all inputs would be 256x256 pixels

