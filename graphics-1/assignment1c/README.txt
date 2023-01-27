I've made separated files to make the program easier to understand, and also used
inheritance for shapes so that my shape methods aren't as ugly anymore (and are also 
extendable).

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

All scenes can be found in the hw1c outputs.zip folder. If possible, I would like my 
showcase image to be laugh.png.