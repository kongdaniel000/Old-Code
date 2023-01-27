The program asks for the name of a file, then takes in that text file with the expected elements, such as viewdir, eye, updir, and so on.
The program can take in multiple spheres and colors (no cylinders since it took me forever to write), with the color corresponding to most recently allocated sphere or vice versa.
    For instance, if a mtlcolor were declared, then a sphere, that mtlcolor would correspond with that sphere.
    If a sphere were declared, then an mtlcolor, that sphere would correspond with that mtlcolor.
    To do this, a shape vector and color vector were used to iterate through corresponding spheres and colors.
No extra files outside of raytracer.cpp and input.txt were used. This is because I was too lazy to separate my structs and functions from the file including main.