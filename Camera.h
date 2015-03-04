#ifndef CAMERA_H
#define CAMERA_H

#define PI 3.14159265358979323846
#define CAMERA_SPEED 0.25
#define CAMERA_ROTATION_SPEED 1.75
#define DISTANCE_OF_HEAD_FROM_FLOOR 15.0

extern void InitCameraRotation();
extern void InitCameraTranslation();
extern void MoveRight();
extern void MoveLeft();
extern void MoveForward();
extern void MoveBackward();
extern void MoveUpward();
extern void MoveDownward();
extern void RotateRight();
extern void RotateLeft();
extern void RotateUp();
extern void RotateDown();

extern float fpXAngle;
extern float fpYAngle;
extern float fpCameraXCoordinate;
extern float fpCameraYCoordinate;
extern float fpCameraZCoordinate;

#endif