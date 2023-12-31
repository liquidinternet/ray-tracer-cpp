# A simple ray tracer written in C++

This repository is based on the series [Ray Tracing in One Weekend](https://github.com/RayTracing/raytracing.github.io/) and features an implementation of a basic CPU ray tracer.

![A rendered image produced by this ray tracer.](render.png)

A sample scene, rendered at 1920x1080 using 500 samples per pixel.

After building, run `./build/main > render.ppm` to render scene to a file. Code compiled using C++20 and Clang. Tested on macOS running on Apple Silicon.