NO third party to construct Ray  Tracer

According to  *Ray Tracing in a Weekend* , but I use  window output instead of ppm outfile

## Here are my pre

1.the setup for windows 

2.add  easy math lib---glm

3.add std_image for loading or storing image

### Step By Step

1. Constructing a ray
2. Adding a sphere
4. add hitable list 
5. Antialiasing 
8. positionable camera

## Result

Now I doubt there is bug or not ,but my programs still work not good,

But most importanly , it is too SLOW(may be one evening for the complex scene)

The following is my result.

![](https://github.com/zhulinspace/MyTinyRayTracer/blob/main/RayTracing/result_img/RayTracer1.png)



## next

1.use BVH to make it faster!!!

I try a simple bvh and compare it to the origin one,the former spent 269 seconds for a scene like above,and for the origin one ,it spent 96 seconds. Ops ,it just a sad story,May be something went wrong!!