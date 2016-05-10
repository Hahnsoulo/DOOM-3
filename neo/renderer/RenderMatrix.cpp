#include "RenderMatrix.h"

const fhRenderMatrix fhRenderMatrix::identity;

fhRenderMatrix fhRenderMatrix::CreateProjectionMatrix( float fov, float aspect, float nearClip, float farClip ) {
	const float D2R = idMath::PI / 180.0;
	const float scale = 1.0 / tan( D2R * fov / 2.0 );
	const float nearmfar = nearClip - farClip;

	fhRenderMatrix m;
	m[0] = scale;
	m[5] = scale;
	m[10] = (farClip + nearClip) / nearmfar;
	m[11] = -1;
	m[14] = 2 * farClip*nearClip / nearmfar;
	m[15] = 0.0f;
	return m;
}

fhRenderMatrix fhRenderMatrix::CreateInfiniteProjectionMatrix( float fov, float aspect, float nearClip ) {
	const float ymax = nearClip * tan( fov * idMath::PI / 360.0f );
	const float ymin = -ymax;
	const float xmax = nearClip * tan( fov * idMath::PI / 360.0f );
	const float xmin = -xmax;
	const float width = xmax - xmin;
	const float height = ymax - ymin;

	fhRenderMatrix m;
	memset( &m, 0, sizeof(m) );

	m[0] = 2 * nearClip / width;
	m[5] = 2 * nearClip / height;

	// this is the far-plane-at-infinity formulation, and
	// crunches the Z range slightly so w=0 vertexes do not
	// rasterize right at the wraparound point
	m[10] = -0.999f;
	m[11] = -1;
	m[14] = -2.0f * nearClip;
	return m;
}

fhRenderMatrix fhRenderMatrix::CreateLookAtMatrix( const idVec3& viewOrigin, const idVec3& at, const idVec3& up )
{
	fhRenderMatrix rot = CreateLookAtMatrix( at - viewOrigin, up );

	fhRenderMatrix translate;
	translate[12] = -viewOrigin.x;
	translate[13] = -viewOrigin.y;
	translate[14] = -viewOrigin.z;

	return rot * translate;
}

fhRenderMatrix fhRenderMatrix::CreateLookAtMatrix( const idVec3& dir, const idVec3& up )
{
	idVec3 zaxis = (dir * -1).Normalized();
	idVec3 xaxis = up.Cross( zaxis ).Normalized();
	idVec3 yaxis = zaxis.Cross( xaxis );

	fhRenderMatrix m;
	m[0] = xaxis.x;
	m[1] = yaxis.x;
	m[2] = zaxis.x;

	m[4] = xaxis.y;
	m[5] = yaxis.y;
	m[6] = zaxis.y;

	m[8] = xaxis.z;
	m[9] = yaxis.z;
	m[10] = zaxis.z;
	return m;
}

fhRenderMatrix fhRenderMatrix::CreateViewMatrix( const idVec3& origin ) {
	fhRenderMatrix m;
	m[12] = -origin.x;
	m[13] = -origin.y;
	m[14] = -origin.z;
	return m;
}

fhRenderMatrix fhRenderMatrix::FlipMatrix() {
	static float flipMatrix[16] = {
		// convert from our coordinate system (looking down X)
		// to OpenGL's coordinate system (looking down -Z)
		0, 0, -1, 0,
		-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 0, 1
	};

	static const fhRenderMatrix m( flipMatrix );
	return m;
}