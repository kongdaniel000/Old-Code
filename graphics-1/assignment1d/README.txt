lightIntersection's calculations have been turned into a function so that raysmay be 
recursively calculated for reflection, found in reflectionAndRefraction.cpp. It's an 
unfortuante mess because I very clearly did not plan very far ahead, (and also
commented poorly) and as such the function is nearly unreadable. Also, I am now aware
(very late into 1d) that this program has serious memory leak errors, which I would try
to fix if I had the time. Sorry. :(

=========PLEASE DO NOT PUT TEXTURE PPMS IN THE SAME DIRECTORY AS THE PROGRAM!!=========
There should be a folder in the same directory as the program named "texture files" in
which you should put your texture files with .ppm extensions. This is done so that the
Makefile doesn't delete the texture ppms and only deletes the output ppms. 
=============ALL PPMS IN THE MAIN DIRECTORY WILL BE DELETED BY MAKE CLEAN!=============

The program runs using "./raytracer" (after making) and the typical use case is:
    * ./raytracer <input file> <output name>
    * Please remember to use file extensions.

If no input file is found, the program does not run and instead prints the use case.

If no output name is found, the output name will be set "output.ppm".

The program assumes the following:
    * That all material colors are defined before the shapes that use that color.
        * In the same vein, that all textures used are defined before the shapes that 
          require the use of that texture.
    * That the vertices of a triangle are defined before the triangle is defined.

Reflection ray calculations run to a depth of MAXDEPTH, after which the reflected ray 
is returned as bkg, the background color. If it appears that reflections aren't
showing up in the picture, then try turning up the index of refraction of the 
material color.

This took me too long to figure out, and my refraction calculation is still iffy to me.
My input.txt and output.txt are the examples I have provided.