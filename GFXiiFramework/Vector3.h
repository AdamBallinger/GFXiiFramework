/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#pragma once
class Vector3
{
private:
	float		m_element[3];

public:
	Vector3()
	{
		m_element[0] = 0.0f;
		m_element[1] = 0.0f;
		m_element[2] = 0.0f;
	}
	
	Vector3(float x, float y, float z)
	{
		m_element[0] = x; m_element[1] = y; m_element[2] = z;
	}
	
	virtual ~Vector3(){ ; }

	float operator [] (const int i) const;
	float& operator [] (const int i);
	Vector3 operator + (const Vector3& rhs) const;
	Vector3 operator - (const Vector3& rhs) const;
	Vector3& operator = (const Vector3& rhs);
	Vector3 operator * (const Vector3& rhs) const;
	Vector3 operator * (float scale) const;

	float Norm()	const;
	float Norm_Sqr() const;
	Vector3 Normalise();

	float DotProduct(const Vector3& rhs) const;
	Vector3 CrossProduct(const Vector3& rhs) const;
	Vector3 Reflect(const Vector3& n) const;
	Vector3 Refract(const Vector3& n, float r_index) const;
	
	void SetZero();
	
	inline void SetVector(float x, float y, float z)
	{ 
		m_element[0] = x; m_element[1] = y; m_element[2] = z; 
	}
};

struct Vertex
{
	Vector3 m_position;
	Vector3 m_normal;
	Vector3 m_texcoords;
};

