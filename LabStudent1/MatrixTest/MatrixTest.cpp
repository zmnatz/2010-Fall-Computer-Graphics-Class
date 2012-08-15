//============================================================================
//	Johns Hopkins University Engineering Programs for Professionals
//	605.467 Computer Graphics and 605.767 Applied Computer Graphics
//	Instructor:	David W. Nesbitt
//
//	Author:  David W. Nesbitt
//	File:    MatrixTest.cpp
//	Purpose: Test the matrix class.
//
//============================================================================

#include <stdio.h>
#include <stdarg.h>

#include "geometry/geometry.h"

// Simple logging function
void logmsg(const char *message, ...)
{
   // Open file if not already opened
   static FILE *lfile = NULL;
   if (lfile == NULL)
      lfile = fopen("MatrixTest.log", "w");

   va_list arg;
   va_start(arg, message);
   vfprintf(lfile, message, arg);
   putc('\n', lfile);
   fflush(lfile);
   va_end(arg);
}

int main(int argc, char* argv[])
{
   logmsg("Matrix Test");

   Matrix4x4 I;
   I.SetIdentity();
   I.Log("Identity Matrix");
   
   // Set a scaling matrix
   Matrix4x4 S;
   S.m11() = 5.0f;	
   S.m22() = 10.0f;
   S.m33() = 5.0f;
   S.Log("Scaling Matrix");

   // Set a translation matrix
   Matrix4x4 T;
   T.m14() = 25.0f;	
   T.m24() = -10.0f;
   T.m34() = 10.0f;
   T.Log("Translation Matrix");

   // Set a rotation matrix about z
   Matrix4x4 R;
   R.m11() = cosf(degreesToRadians(35.0f));
   R.m22() = R.m11();
   R.m21() = sinf(degreesToRadians(35.0f));
   R.m12() = -R.m21();
   R.Log("Rotation (z) Matrix");

   // Perform composite
   Matrix4x4 C = T * R * S;
   C.Log("Composite TRS");

   // Transform point 1,1,1,1 (homogeneous point)
   HPoint3 P(1.0f, 1.0f, 1.0f, 1.0f);
   HPoint3 P1 = C * P;
   logmsg("Transformed Point is %f %f %f %f", P1.x, P1.y, P1.z, P1.w);

   // Show how the order matters
   Matrix4x4 C2 = S * R * T;
   C2.Log("Composite SRT");
   HPoint3 P2 = C2 * P;
   logmsg("Transformed Point is %f %f %f %f", P2.x, P2.y, P2.z, P2.w);

   // Test the local Translate, Rotate, Scale methods. Should match C
   logmsg("\nTest Local Translate, Rotate, Scale");
   logmsg("Should match Composite TRS Above");
   Matrix4x4 M;
   M.Translate(25.0f, -10.0f, 10.0f);
   M.Rotate(35.f, 0.0f, 0.0f, 1.0f);
   M.Scale(5.0f, 10.0f, 5.0f);
   M.Log("After TRS: Composite matrix is");

   // New composite matrix
   C.SetIdentity();
   C.Translate(-10.0f, 20.0f, 3.0f);
   C.Rotate(60.f, 1.0f, 0.0f, 0.0f);     // 60 degrees about x axis
   C.Scale(15.0f, 15.0f, 15.0f);
   C.Log("Composite TRS");

   // Transform point 1,0,1
   Point3 P3(1.0f, 1.0f, 0.0f);
   P1 = C * P3;
   logmsg("Transformed Point is %f %f %f %f", P1.x, P1.y, P1.z, P1.w);

   // Get the transpose of C
   Matrix4x4 D = C.Transpose(true);
   D.Log("D = Transpose of C");

   // Get the transpose of C
   C.Transpose();
   C.Log("Transpose of C");

   // Get the inverse of C
   Matrix4x4 CInverse = C.Inverse();
   CInverse.Log("Inverse of C");

   // Verify the Inverse
   Matrix4x4 VerifyInverse = C * CInverse;
   VerifyInverse.Log("Matrix times its Inverse (Should be Identity Matrix)");

   // Test the ability to get the matrix and set it
   logmsg("Verify Set and Get Methods: Result should match rotation matrix");
   const float* m = R.Get();
   R.Set(m);
   R.Log("Rotation matrix after Get and Set");

   return 1;
}