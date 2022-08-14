#pragma once
class vec3
{
	public:
		double x, y, z = 0;

		// constructors

		vec3() { x = y = z = 0; };

		vec3(const double sc) { x = y = z = sc; };

		vec3(const double xv, const double yv, const double zv) {
			x = xv;
			y = yv;
			z = zv;
		}

		vec3(const vec2 v, const double sc) {
			x = v.x;
			y = v.y;
			z = sc;
		}

		vec3(const double sc, const vec2 v) {
			x = sc;
			y = v.x;
			z = v.y;
		}

		// operator overloads

		vec3 operator+ (const vec3& v) const {
			return vec3(x + v.x, y + v.y, z + v.z);
		}

		void operator+= (const vec3& v) {
			x += v.x;
			y += v.y;
			z += v.z;
		}

		vec3 operator- (const vec3& v) const {
			return vec3(x - v.x, y - v.y, z - v.z);
		}

		void operator-= (const vec3& v) {
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}

		vec3 operator* (const vec3& v) const {
			return vec3(x * v.x, y * v.y, z * v.z);
		}

		vec3 operator* (const double& sc) const {
			return vec3(x * sc, y * sc, z * sc);
		}

		void operator*= (const vec3& v) {
			x *= v.x;
			y *= v.y;
			z *= v.z;
		}

		void operator*= (const double& sc) {
			x *= sc;
			y *= sc;
			z *= sc;
		}

		vec3 operator/ (const vec3& v) const {
			return vec3(x / v.x, y / v.y, z / v.z);
		}

		vec3 operator/ (const double& sc) const {
			return vec3(x / sc, y / sc, z / sc);
		}

		void operator/= (const vec3& v) {
			x /= v.x;
			y /= v.y;
			z /= v.z;
		}

		void operator/= (const double& sc) {
			x /= sc;
			y /= sc;
			z /= sc;
		}

		bool operator== (const vec3& v) const {
			return (x == v.x && y == v.y && z == v.z);
		}
};

// utility

double dot(const vec3 v1, const vec3 v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

double length(const vec3 v) {
	return sqrt(dot(v, v));
}

vec3 normalize(const vec3 v) {
	double l = length(v);
	return vec3(v.x / l, v.y / l, v.z / l);
}

vec3 cross(const vec3 v1, const vec3 v2) {
	return vec3(v1.y * v2.z - v2.y * v1.z,
		v1.z * v2.x - v2.z * v1.x,
		v1.x * v2.y - v2.x * v1.y);
}

double distance(const vec3 v1, const vec3 v2) {
	return length(v2 - v1);
}

vec3 max(const vec3 v, const double sc) {
	return vec3(fmax(v.x, sc), fmax(v.y, sc), fmax(v.z, sc));
}

vec3 max(const vec3 v1, const vec3 v2) {
	return vec3(fmax(v1.x, v2.x), fmax(v1.y, v2.y), fmax(v1.z, v2.z));
}

vec3 min(const vec3 v, const double sc) {
	return vec3(fmin(v.x, sc), fmin(v.y, sc), fmin(v.z, sc));
}

vec3 min(const vec3 v1, const vec3 v2) {
	return vec3(fmin(v1.x, v2.x), fmin(v1.y, v2.y), fmin(v1.z, v2.z));
}

vec3 clamp(const vec3 v, const double a, const double b) {
	return max(min(v, b), a);
}

vec3 powv(const vec3 v, const double sc) {
	return vec3(pow(v.x, sc), pow(v.y, sc), pow(v.z, sc));
}

vec3 reflect(const vec3 v, const vec3 n) {
	return v - n * 2.0 * dot(v, n);
}

vec3 refract(const vec3 v, const vec3 n, const double eta) {
	double k = 1.0 - eta * eta * (1.0 - dot(n, v) * dot(n, v));
	if (k < 0.0) {
		return vec3(0.0);
	}
	else {
		return v * eta - n * (eta * dot(n, v) + sqrt(k));
	}
}

// allow commutative vector multiplication with scalar

vec3 operator* (const double& sc, const vec3 v) {
	return v * sc;
}
