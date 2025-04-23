#pragma once
#pragma once  
#include "Matrix4x4.h"  

class Vector4 {  
public:  
   float x, y, z, w;  


   /// チーム制作のため坪木が作成した
   ///↓



   Vector4() : x(0), y(0), z(0), w(0) {}  
   Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}  

   // Overload the multiplication operator for Matrix4x4 * Vector4  
   friend Vector4 operator*(const Matrix4x4& matrix, const Vector4& vector) {  
       return Vector4(  
           matrix.m[0][0] * vector.x + matrix.m[0][1] * vector.y + matrix.m[0][2] * vector.z + matrix.m[0][3] * vector.w,  
           matrix.m[1][0] * vector.x + matrix.m[1][1] * vector.y + matrix.m[1][2] * vector.z + matrix.m[1][3] * vector.w,  
           matrix.m[2][0] * vector.x + matrix.m[2][1] * vector.y + matrix.m[2][2] * vector.z + matrix.m[2][3] * vector.w,  
           matrix.m[3][0] * vector.x + matrix.m[3][1] * vector.y + matrix.m[3][2] * vector.z + matrix.m[3][3] * vector.w  
       );  
   }  
};
