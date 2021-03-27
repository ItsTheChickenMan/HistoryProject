/* date = January 12th 2021 7:47 pm */

#ifndef SHAPES_H
#define SHAPES_H


float plane_v[] = {
    0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // top left 
};
//
u32 plane_i[] = {  
    0, 1, 3,  
    1, 2, 3   
};  

// Position {x, y, z}, UV{ s, t}, Normal {x, y, z}
float cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
    0.5f,  -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
    0.5f,   0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f, 
    0.5f,   0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
    
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
    0.5f,  -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
    0.5f,   0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    0.5f,   0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
    
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    
    0.5f,   0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
    0.5f,   0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
    0.5f,   0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
    
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
    0.5f,  -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
    0.5f,  -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    0.5f,  -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
    
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    0.5f,   0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    0.5f,   0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    0.5f,   0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
};

#endif //SHAPES_H