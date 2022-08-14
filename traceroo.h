#pragma once

const int WIDTH = 640; //width
const int HEIGHT = 360; //height

const vec2 iResolution = vec2((double)WIDTH, (double)HEIGHT);
const vec2 iMouse = vec2(0);

const double PI = 3.14159265358979323846;
const double MAX_DIST = 100000.0;

const int scenen = 0;

const int SPF = 8;

const vec3 mi1 = vec3(32.0/255.0,22.0/255.0,15.0/255.0);
const vec3 mi2 = vec3(126.0/255.0,126.0/255.0,103.0/255.0);
const vec3 mi3 = vec3(126.0/255.0,126.0/255.0,103.0/255.0);
const vec3 mi4 = vec3(255.0/255.0,218.0/255.0,173.0/255.0);
const vec3 mi5 = vec3(255.0/255.0,218.0/255.0,173.0/255.0);
const vec3 mi6 = vec3(238.0/255.0,250.0/255.0,255.0/255.0);
const vec3 mi7 = vec3(238.0/255.0,250.0/255.0,255.0/255.0);
const vec3 mi8 = vec3(148.0/255.0,184.0/255.0,209.0/255.0);

vec3 center;
vec3 ray;
double rdist;
vec3 normal;
vec3 color;
int material;
double ior;
bool inside;

double frand() {
	return (double) rand()/RAND_MAX;
}

vec2 randCircle(const double rad) {
	double d = frand()*2.0*PI;
	double m = sqrt(frand())*rad;
	return m*vec2(cos(d), sin(d));
}

double mix(double x, double y, double a) {
	if (y > x) {
		fmax(fmin(x * (1 - a) + y * a, y), x);
	} else {
		fmax(fmin(x * (1 - a) + y * a, x), y);
	}
}

vec3 cosineDirection(vec3 nor)
{
    double u = frand()*2.0-1.0;
    double a = frand()*2.0*PI;
    return normalize(nor+vec3(sqrt(1.0-u*u)*vec2(cos(a),sin(a)), u) );
}

vec3 gammaCorrection(vec3 incol) {
    return powv(incol, 0.4545);
}

double vignette(const double x, double intensity) {
	intensity = 1.0 - intensity;
	return (4.0 - 4.0 * intensity) * (x - x * x) + intensity;
}

vec3 rotate(vec3 inpoint, double pitch, double roll, double yaw) {
    // create rotation matrix
    
    double cosa = cos(yaw);
    double sina = sin(yaw);

    double cosb = cos(pitch);
    double sinb = sin(pitch);

    double cosc = cos(roll);
    double sinc = sin(roll);
    
    vec3 Ax = vec3(cosa*cosb, cosa*sinb*sinc - sina*cosc, cosa*sinb*cosc + sina*sinc);
    vec3 Ay = vec3(sina*cosb, sina*sinb*sinc + cosa*cosc, sina*sinb*cosc - cosa*sinc);
    vec3 Az = vec3(-sinb, cosb*sinc, cosb*cosc);

    // rotate
    return vec3(dot(Ax, inpoint),
                dot(Ay, inpoint),
                dot(Az, inpoint));
}

// schlick's approx (for dielectric materials)
double calcFresnel(double n0, double n1) {
    double r = (n0-n1) / (n0+n1);
    r *= r;
    double a = 1.0 + dot(ray, normal);
    return r + (1.0 - r) * a*a*a*a*a;
}

vec3 sky2(double y) {
    y = (y + 1.0) * 0.5;
    vec3 oc = vec3(0);
    if (y < 0.32 && y >= 0.0) {
        y = y / 0.32;
        oc.x = mix(mi1.x, mi2.x, y);
        oc.y = mix(mi1.y, mi2.y, y);
        oc.z = mix(mi1.z, mi2.z, y);
    } else if (y < 0.5) {
        y = (y - 0.32) / 0.18;
        oc.x = mix(mi3.x, mi4.x, y);
        oc.y = mix(mi3.y, mi4.y, y);
        oc.z = mix(mi3.z, mi4.z, y);
    } else if (y < 0.57) {
        y = (y - 0.5) / 0.07;
        oc.x = mix(mi5.x, mi6.x, y);
        oc.y = mix(mi5.y, mi6.y, y);
        oc.z = mix(mi5.z, mi6.z, y);
    } else {
        y = (y - 0.57) / 0.43;
        oc.x = mix(mi7.x, mi8.x, y);
        oc.y = mix(mi7.y, mi8.y, y);
        oc.z = mix(mi7.z, mi8.z, y);
    }
    return oc;
}

vec3 filmBleed(vec3 inc, double bleedThreshold, double bleedAmount) {
    vec3 addon = max(inc - vec3(bleedThreshold), vec3(0))*vec3(bleedAmount);
    return inc + vec3(addon.y+addon.z, addon.x+addon.z, addon.x+addon.y);
}

void iSphere(vec3 ce, double ra, vec3 sColor, int m) {
    vec3 oc = center - ce;
    double b = dot( oc, ray );
    double c = dot( oc, oc ) - ra*ra;
    double h = b*b - c;
    if(h >= 0.0) {
        h = sqrt(h);
        bool hit = false;
        double d;
        if (distance(center, ce) < ra) {
            d = -b+h;
            if (d < rdist && d > 0.0) {
                hit = true;
                inside = true;
                rdist = d;
                vec3 intersect = center+ray*rdist;
                normal = normalize(ce-intersect);
            }
        } else {
            d = -b-h;
            if (d < rdist && d > 0.0) {
                hit = true;
                inside = false;
                rdist = d;
                vec3 intersect = center+ray*rdist;
                normal = normalize(intersect-ce);
            }
        }
        if (hit) {
            color = sColor;
            material = m;
            
            // glass
            if (m == 2) {
                double f = calcFresnel(inside?1.333:1.0, inside?1.0:1.333);
                if (frand() < f) {
                    material = 3;
                    color = vec3(1);
                }
            } else
            
            // glossy
            if (m == 4) {
                material = 0;
                double f = calcFresnel(1.0, 1.15);
                if (frand() < f) {
                    material = 3;
                    color = vec3(1);
                }
            }
        }
    } 
}

void scene() {
    if (scenen == 0) {
        iSphere(vec3(-5.14*3.0, 5.14, 10.0), 5.0, vec3(0.99, 0.99, 0.99), 3); // only specular
        iSphere(vec3(-5.14, 5.14, 20.0), 5.0, 1.5*vec3(200, 100, 50), 1); // light source
        iSphere(vec3(5.14, 5.14, 5.0), 5.0, vec3(0.99, 0.99, 0.99), 2); // glass (ior 1.333)
        iSphere(vec3(5.14*3.0, 5.14, 25.0), 5.0, vec3(0.99, 0.99, 0.99), 4); // glossy

        // ground
        iSphere(vec3(0, -1000000, 30), 1000000.0, vec3(0.7, 0.7, 0.7), 4);

        // light
        iSphere(vec3(-10, 50, 20), 25.0, 2.0*vec3(200, 200, 175), 1);
    } else {
        iSphere(vec3(-12.0, 3.05, -49.18), 8.0, vec3(0.9, 0.25, 0.25), 4);
        iSphere(vec3(-1.67, -0.55, -66.5802), 4.277, vec3(0.99, 0.99, 0.99), 2);
        iSphere(vec3(-9.75, -2.2, -77.38), 2.649, 400.0*vec3(1.0, 0.4, 0.1), 1);
        iSphere(vec3(18.58, 8.6, -94.41), 13.451, vec3(0.5, 0.2, 0.1), 0);
        iSphere(vec3(3.88, -1.1, -94.41), 3.725, vec3(0.99, 0.99, 0.99), 3);
        iSphere(vec3(0.68, 3.2, -105.3), 8.0, vec3(0.99, 0.99, 0.99), 2);
        iSphere(vec3(-22.32, 0.5, -172.6), 5.357, vec3(0.3, 0.8, 0.4), 4);
        
        iSphere(vec3(0, -1000005, 30), 1000000.0, vec3(0.9, 0.9, 0.9), 0);
    }
}

// trace the scene
vec3 raytrace(vec2 uv, double DoF, double aperture) {
    uv += vec2(frand(), frand())/iResolution.y;
    
    const double invDoF = 1.0/DoF;

    vec3 s = vec3(1.0/256.0);
    
    // depth of field
    vec2 coff = randCircle(aperture);
    //vec2 coff = vec2(0);
    center = vec3(0.0+coff.x, 4.0+coff.y, -30.0);
    vec2 m;
    if (iMouse == vec2(0)) {
        m = vec2(0);
    } else {
        m = vec2((iMouse.x-iResolution.x*0.5)/(0-iResolution.y), (iMouse.y-iResolution.y*0.5)/iResolution.y);
    }
    if (scenen == 0) {
        ray = rotate(normalize(vec3(uv, 2)), (0-coff.x)*invDoF+m.x, coff.y*invDoF+m.y, 0.0);
    } else {
        ray = rotate(normalize(vec3(uv, 2)), coff.x*invDoF+m.x+PI, coff.y*invDoF+m.y, 0.0);
    }
    
    // light bounces
    for (int i=0; i<8; i++) {
        rdist = MAX_DIST;
        scene();
        if (rdist<MAX_DIST) {
            s *= color;
            if (material == 2) {
                center += rdist*ray-normal*0.1;
            } else {
                center += rdist*ray+normal*0.1; 
            }
            if (material == 0) {
                ray = cosineDirection(normal);
            } else if (material == 1) {
                break;
            } else if (material == 2) {
                double eta = 1.0/1.333;
                if (inside) {eta = 1.333;}
                ray = normalize(refract(ray, normal, eta));
            } else if (material == 3) {
                ray = normalize(reflect(ray, normal));
            }
        } else {
            if (scenen == 0) {
                s = vec3(0); // black sky (light only comes from light sources)
            } else {
                s *= 256.0*sky2(ray.y);
            }
            break;
        }
    } 
    
    return filmBleed(s, 1.0, 1.0);
}

vec3 render(vec2 fragCoord)
{
	vec3 col = vec3(0);
    vec2 uv = fragCoord/iResolution;
    vec2 px = (fragCoord-iResolution*0.5)/iResolution.y;
    
    // do SPF samples per pixel per frame
    for (int i = 0; i < SPF; i++) {
        if (scenen == 0) {
            col += raytrace(px, 42.87, 2.0);
        } else {
            col += raytrace(px, 64.727989, 2.0);
        }
    }
    
	col /= (double)SPF;

	double vi = vignette(uv.x, 0.25) * vignette(uv.y, 0.25);

	return gammaCorrection(col*vi);
}
