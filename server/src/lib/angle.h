#ifndef ANGLE
#define ANGLE

#define ANGLE_ADD(A,B)      (A + B) % 360
#define ANGLE_MINUS(A,B)    (A - B + 360) % 360

//relative to absolute and absolute to relative
#define RTOA(A,B)           (A + B) % 360
#define ATOR(A,B)           (A - B + 360) % 360

#endif
