#include "rsw_math.h"

namespace rsw
{



/*
 * Matrix functions
 */

/*
 * mat3
 */


mat3 operator*(const mat3& a, const mat3& b)
{
	mat3 tmp;
	tmp[0] = a.x()*b.c1();
	tmp[1] = a.x()*b.c2();
	tmp[2] = a.x()*b.c3();
	tmp[3] = a.y()*b.c1();
	tmp[4] = a.y()*b.c2();
	tmp[5] = a.y()*b.c3();
	tmp[6] = a.z()*b.c1();
	tmp[7] = a.z()*b.c2();
	tmp[8] = a.z()*b.c3();
	return tmp;
}


void load_rotation_mat3(mat3& the_matrix, vec3 v, float angle)
{
	float s, c;
	float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

	s = float(sin(angle));
	c = float(cos(angle));

	float *mat = the_matrix.matrix;
	// Rotation matrix is normalized
	v.normalize();

	xx = v.x * v.x;
	yy = v.y * v.y;
	zz = v.z * v.z;
	xy = v.x * v.y;
	yz = v.y * v.z;
	zx = v.z * v.x;
	xs = v.x * s;
	ys = v.y * s;
	zs = v.z * s;
	one_c = 1.0f - c;

	mat[0] = (one_c * xx) + c;
	mat[1] = (one_c * xy) - zs;
	mat[2] = (one_c * zx) + ys;

	mat[3] = (one_c * xy) + zs;
	mat[4] = (one_c * yy) + c;
	mat[5] = (one_c * yz) - xs;

	mat[6] = (one_c * zx) - ys;
	mat[7] = (one_c * yz) + xs;
	mat[8] = (one_c * zz) + c;
}



/*
 * mat4
 */

mat4 operator*(const mat4& a, const mat4& b)
{
	//assume 1 in 15th position of both, 0's in rest of bottom row
	mat4 tmp;
	tmp[0] = a.x()*b.c1();
	tmp[1] = a.x()*b.c2();
	tmp[2] = a.x()*b.c3();
	tmp[3] = a.x()*b.c4();

	tmp[4] = a.y()*b.c1();
	tmp[5] = a.y()*b.c2();
	tmp[6] = a.y()*b.c3();
	tmp[7] = a.y()*b.c4();

	tmp[8] = a.z()*b.c1();
	tmp[9] = a.z()*b.c2();
	tmp[10] = a.z()*b.c3();
	tmp[11] = a.z()*b.c4();

	tmp[12] = a.w()*b.c1();
	tmp[13] = a.w()*b.c2();
	tmp[14] = a.w()*b.c3();
	tmp[15] = a.w()*b.c4();

	//bottom row is already appropriately set to 0 0 0 1 from constructor
	//but maybe the last multiplications are necessary in some cases

	return tmp;
}

void load_rotation_mat4(mat4& the_matrix, vec3 v, float angle)
{
	float s, c;
	float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

	s = float(sin(angle));
	c = float(cos(angle));

	float *mat = the_matrix.matrix;

//	// Rotation matrix is normalized
	v.normalize();

	xx = v.x * v.x;
	yy = v.y * v.y;
	zz = v.z * v.z;
	xy = v.x * v.y;
	yz = v.y * v.z;
	zx = v.z * v.x;
	xs = v.x * s;
	ys = v.y * s;
	zs = v.z * s;
	one_c = 1.0f - c;

	mat[0] = (one_c * xx) + c;
	mat[1] = (one_c * xy) - zs;
	mat[2] = (one_c * zx) + ys;
	mat[3] = 0.0f;

	mat[4] = (one_c * xy) + zs;
	mat[5] = (one_c * yy) + c;
	mat[6] = (one_c * yz) - xs;
	mat[7] = 0.0f;

	mat[8] = (one_c * zx) - ys;
	mat[9] = (one_c * yz) + xs;
	mat[10] = (one_c * zz) + c;
	mat[11] = 0.0f;

	mat[12] = 0.0f;
	mat[13] = 0.0f;
	mat[14] = 0.0f;
	mat[15] = 1.0f;
}




static float DetIJ(const mat4& m, const int i, const int j)
{
	float ret, mat[3][3];
	int x = 0, y = 0;

	for (int ii=0; ii<4; ii++) {
		y = 0;
		if (ii == i) continue;
		for (int jj=0; jj<4; jj++) {
			if (jj == j) continue;
			mat[x][y] = m.matrix[ii*4+jj];
			y++;
		}
		x++;
	}



	ret =  mat[0][0]*(mat[1][1]*mat[2][2]-mat[2][1]*mat[1][2]);
	ret -= mat[0][1]*(mat[1][0]*mat[2][2]-mat[2][0]*mat[1][2]);
	ret += mat[0][2]*(mat[1][0]*mat[2][1]-mat[2][0]*mat[1][1]);

	return ret;
}


mat4 invert_mat4(const mat4& m)
{
	int i, j;
	float det, detij;
	mat4 mInverse;

	// calculate 4x4 determinant
	det = 0.0f;
	for (i = 0; i < 4; i++)
	{
		det += (i & 0x1) ? (-m.matrix[i] * DetIJ(m, 0, i)) : (m.matrix[i] * DetIJ(m, 0, i));
	}
	det = 1.0f / det;

	// calculate inverse
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			detij = DetIJ(m, j, i);
			mInverse.matrix[(i*4)+j] = ((i+j) & 0x1) ? (-detij * det) : (detij *det);
		}
	}

	return mInverse;
}







////////////////////////////////////////////////////////////////////////////////////////////

//assumes converting from canonical view volume [-1,1]^3
//works just like glViewport, x and y are lower left corner.  opengl should be 1.
void make_viewport_matrix(mat4& mat, int x, int y, unsigned int width, unsigned int height, int opengl)
{
	float w, h, l, t, b, r;

	if (opengl) {
		//See glspec page 104, integer grid is lower left pixel corners
		w = width, h = height;
		l = x, b = y;
		//range is [0, w) x [0 , h)
		r = l + w - 0.01; //epsilon larger than float precision
		t = b + h - 0.01;

		mat[0] = (r - l) / 2;
		mat[1] = 0;
		mat[2] = 0;
		mat[3] = (l + r) / 2;
		mat[4] = 0;

		//this used to be negative to flip y till I changed glFramebuffer and draw_pixel to accomplish the same thing
		mat[5] = (t - b) / 2;

		mat[6] = 0;
		mat[7] = (b + t) / 2;
		mat[8] = 0;
		mat[9] = 0;
		mat[10] = 1;
		mat[11] = 0;
		mat[12] = 0;
		mat[13] = 0;
		mat[14] = 0;
		mat[15] = 1;

	} else {
		//old way with pixel centers at integer coordinates
		//see pages 133/4 and 144 of FoCG
		//necessary for fast integer only bresenham line drawing

		w = width, h = height;
		l = x - 0.5f;
		b = y - 0.5f;
		r = l + w;
		t = b + h;

		mat[0] = (r - l) / 2;
		mat[1] = 0;
		mat[2] = 0;
		mat[3] = (l + r) / 2;
		mat[4] = 0;

		//make this negative to reflect y otherwise positive y maps to lower half of the screen
		//this is mapping the unit square [-1,1]^2 to the window size. x is fine because it increases left to right
		//but the screen coordinates (ie framebuffer memory) increase top to bottom opposite of the canonical square so no matter what I did
		//one of my coordinates always seemed wrong no matter how I oriented my model frame because my y was always
		//flipped in this matrix
		//I think this is the easiest way to fix it without any side effects.
		mat[5] = (t - b) / 2;
		mat[6] = 0;
		mat[7] = (b + t) / 2;
		mat[8] = 0;
		mat[9] = 0;
		mat[10] = 1;
		mat[11] = 0;
		mat[12] = 0;
		mat[13] = 0;
		mat[14] = 0;
		mat[15] = 1;
	}
}


//I can't really think of any reason to ever use this matrix alone.
//You'd always do ortho * pers and really if you're doing perspective projection
//just use make_perspective_matrix (or less likely make perspective_proj_matrix)
//
//This function is really just for completeness sake based off of FoCG 3rd edition pg 152
//changed slightly.  z_near and z_far are always positive and z_near < z_far
//
//Inconsistently, to generate an ortho matrix to multiply with that will get the equivalent
//of the other 2 functions you'd use -z_near and -z_far and near > far.
void make_pers_matrix(mat4& mat, float z_near, float z_far)
{
	mat[0] = z_near;
	mat[5] = z_near;
	mat[10] = z_near + z_far;
	mat[11] = (z_far * z_near);
	mat[14] = -1;
	mat[15] = 0;
}



// Create a projection matrix
// Similiar to the old gluPerspective... fov is in radians btw...
void make_perspective_matrix(mat4 &mat, float fov, float aspect, float n, float f)
{
	//assume it's already identity
	std::cout << fov << "\t" << aspect << "\n\n";

	float t = n * tanf(fov * 0.5f);
	float b = -t;
	float l = b * aspect;
	float r = -l;


	//check with book TODO done
	mat[0] = (2.0f * n) / (r - l);
	mat[2] = (r + l) / (r - l);

	mat[5] = (2.0f * n) / (t - b);
	mat[6] = (t + b) / (t - b);

	mat[10] = -((f + n) / (f - n));
	mat[11] = -((2.0f * (f*n))/(f - n));

	mat[14] = -1.0f;
	mat[15] = 0.0f;
}

void make_perspective_proj_matrix(mat4 &mat, float l, float r, float b, float t, float n, float f)
{
	//assume mat is identity already

	mat[0] = (2*n)/(r - l);
	mat[2] = (r + l)/(r - l);

	mat[5] = (2*n)/(t - b);
	mat[6] = (t + b)/(t - b);

	mat[10] = -(f + n)/(f - n);
	mat[11] = -(2*f*n)/(f - n);

	mat[14] = -1;
	mat[15] = 0;
}




//n and f really are near and far not min and max so if you want the standard looking down the -z axis
// then n > f otherwise n < f
void make_orthographic_matrix(mat4 &the_matrix, float l, float r, float b, float t, float n, float f)
{
	//assume the_matrix is already identity
	float *mat = the_matrix.matrix;

	mat[0] = 2.0f / (r - l);
	mat[3] = -((r + l)/(r - l));
	mat[5] = 2.0f / (t - b);
	mat[7] = -((t + b)/(t - b));
	mat[10] = 2.0f / (f - n);		//removed - in front of 2 . . . book doesn't have it but superbible did
	mat[11] = -((n + f)/(f - n));
	mat[15] = 1;

	//now I know why the superbible had the -
	//OpenGL uses a left handed canonical view volume [-1,1]^3 when passed the identity matrix
	//ie in Normalized Device Coordinates.  The math/matrix presented in Fundamentals of Computer
	//Graphics assumes a right handed version of the same volume.  The negative isn't necessary
	//if you set n and f correctly as near and far not low and high
}


int intersect_segment_plane(vec3 a, vec3 b, Plane p, float& t, vec3& q)
{
	vec3 ab = b - a;
	t = (p.d - (p.n*a)) / (p.n * ab);

	//if t in [0-1] compute point
	if (t >= 0.0f && t <= 1.0f) {
		q = a + t*ab;
		return 1;
	}

	//else no intersection
	return 0;
}









}	//close robert3dmath namespace
