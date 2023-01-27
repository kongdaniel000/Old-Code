I've since understood what accepting commandline arguments are (I think) so I've made a few improvements to the code.

I've also made it so vectorToArray() and vectorToColor() are not necessary any longer and replaced both with 
readInput().

The program runs using "./raytracer" (after making) and accepts an input file in the following manner:
    * ./raytracer <input file> <output name>
    * Please remember to use file extensions.

If no file is found, the defaults are set to
    * eye = 0.0, 0.0, 0.0
    * viewdir = 0.0, 0.0, 1.0
    * updir = 0.0, 1.0, 0.0
    * vfov = 90.0
    * imsize = 400.0, 400.0
    * bkgcolor = 0.0, 0.0, 0.0
    * and the output name will be set to just "raytracer.ppm"

The program can take in multiple spheres and colors (no cylinders since it took me forever to write), with the color 
corresponding to earliest allocated sphere without an mtlcolor or vice versa.
    * For instance, if a mtlcolor were declared, then a sphere, that mtlcolor would correspond with that sphere.

    * If a sphere were declared, then an mtlcolor, that sphere would correspond with that mtlcolor.

    * If sphere1 were declared, then sphere2 were declared, then an mtlcolor1, sphere1 would correspond with 
      mtlcolor1, and sphere2 would be assigned the next mtlcolor or be dropped.

    * If mtlcolor1 were declared, then mtlcolor2 were declared, then a sphere, mtlcolor1 would correspond with
      sphere1, and mtlcolor2 would be assigned the next sphere or be dropped.
      
    * To do this, a shape vector and color vector were used to iterate through corresponding spheres and colors.