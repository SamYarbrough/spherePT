#pragma once
class vec2 
{
	public:
		double x, y = 0;

		//constructors

		vec2() { x = y = 0; };

		vec2(const double sc) { x = y = sc; };

		vec2(const double xv, const double yv) {
			x = xv;
			y = yv;
		}

		// operator overloads

		vec2 operator+ (const vec2& v) const {
			return vec2(x + v.x, y + v.y);
		}

		void operator+= (const vec2& v) {
			x += v.x;
			y += v.y;
		}

		vec2 operator- (const vec2& v) const {
			return vec2(x - v.x, y - v.y);
		}

		void operator-= (const vec2& v) {
			x -= v.x;
			y -= v.y;
		}

		vec2 operator* (const vec2& v) const {
			return vec2(x * v.x, y * v.y);
		}

		vec2 operator* (const double& sc) const {
			return vec2(x * sc, y * sc);
		}

		void operator*= (const vec2& v) {
			x *= v.x;
			y *= v.y;
		}

		void operator*= (const double& sc) {
			x *= sc;
			y *= sc;
		}

		vec2 operator/ (const vec2& v) const {
			return vec2(x / v.x, y / v.y);
		}

		vec2 operator/ (const double& sc) const {
			return vec2(x / sc, y / sc);
		}

		void operator/= (const vec2& v) {
			x /= v.x;
			y /= v.y;
		}

		void operator/= (const double& sc) {
			x /= sc;
			y /= sc;
		}

		bool operator== (const vec2& v) const {
			return (x == v.x && y == v.y);
		}
};

// utility

double dot(const vec2 v1, const vec2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

double length(const vec2 v) {
	return sqrt(dot(v, v));
}

vec2 normalize(const vec2 v) {
	double l = length(v);
	return vec2(v.x / l, v.y / l);
}

double distance(const vec2 v1, const vec2 v2) {
	return length(v2 - v1);
}

vec2 max(const vec2 v, const double sc) {
	return vec2(fmax(v.x, sc), fmax(v.y, sc));
}

vec2 max(const vec2 v1, const vec2 v2) {
	return vec2(fmax(v1.x, v2.x), fmax(v1.y, v2.y));
}

vec2 min(const vec2 v, const double sc) {
	return vec2(fmin(v.x, sc), fmin(v.y, sc));
}

vec2 min(const vec2 v1, const vec2 v2) {
	return vec2(fmin(v1.x, v2.x), fmin(v1.y, v2.y));
}

vec2 clamp(const vec2 v, const double a, const double b) {
	return max(min(v, b), a);
}

vec2 powv(const vec2 v, const double sc) {
	return vec2(pow(v.x, sc), pow(v.y, sc));
}

// allow commutative vector multiplication with scalar

vec2 operator* (const double& sc, const vec2 v) {
	return v * sc;
}
