# webGLviewer
webGL .pto viewer for modern Browser  

## Goal
Show the files properly according to the values from the .pto file.  
This is NOT about stitching (combining) the six source images into a single large, panorama.  
Instead to show the six images directly.  

##Preparation:
Install hugin and get the provided example working.  
Hugin:  
http://hugin.sourceforge.net/  
Example:  
http://www.applied-streetview.com/data/Preview/hugin%20example.zip  

Understand the application of roll, pitch and yaw in the "Fast Panorama preview - Move/Drag" tab:  
http://www.applied-streetview.com/data/Preview/Hugin%20-%20Fast%20Panorama%20preview%20-%20MoveDrag.jpg  


##Milestones:
1) Rebuild the "Fast Panorama preview - Move/Drag" tab with webGL and shaders for a browser.  
Parse the pto file.  
Do not violate the GPL.  
Performance (speed) not to degrade.  

The fils from the "hugin example" is online for this:  
http://www.applied-streetview.com/data/webGLviewer/hugin_example/000000012-1.jpg  
http://www.applied-streetview.com/data/webGLviewer/hugin_example/000000012-2.jpg  
http://www.applied-streetview.com/data/webGLviewer/hugin_example/000000012-3.jpg  
http://www.applied-streetview.com/data/webGLviewer/hugin_example/000000012-4.jpg  
http://www.applied-streetview.com/data/webGLviewer/hugin_example/000000012-5.jpg  
http://www.applied-streetview.com/data/webGLviewer/hugin_example/000000012-6.jpg  
.pto file with links to the files above:  
http://www.applied-streetview.com/data/webGLviewer/hugin_example/000000012-net.pto  

Original pto file:  
http://www.applied-streetview.com/data/webGLviewer/hugin_example/000000012.pto  

Stretch Target:
2) Improve color matching
Reduce/Remove artifacts (vignetting/wrong colors) in the corners of the 6 source images.
http://www.applied-streetview.com/data/Preview/artefacts.jpg

Upload your results often to github, incl. screenshtos of what i should see.
