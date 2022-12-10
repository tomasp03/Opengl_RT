#version 460 core
#define FLT_MAX 3.402823466e+38
in vec4 gl_FragCoord;

out vec4 FragColor;

uniform vec2 u_resolution;
uniform float u_time;
uniform vec3 u_camPos;
uniform vec3 u_sphereCenter;
uniform vec3 u_sphereCenter2;
uniform vec3 u_lightPos;
uniform vec2 u_mousePos;
uniform mat4 u_projection;
uniform mat4 u_model;
uniform mat4 u_rotate;

vec3 lightPos = u_lightPos;
vec3 ambient = vec3(0.1);
vec3 camPos = u_camPos;
const int spheresCount = 2;
vec3 bgColor = vec3(0);

struct Ray
{
	vec3 origin;
	vec3 direction;
};

//finds the closest Sphere to ray, if it doesnt intersect any sphere it returns -1
int findClosestSphere(Ray ray);
vec3 determinePiexelColor(Ray ray, int hittedSphere);
vec3 drawBackground(Ray ray);

struct Sphere
{
	vec3 center;
	float radius;
	vec3 color;
};

struct Canvas
{
	float height;
	vec3 center;
	float width;
	float focalLen; //Distance from 0,0,0 to plane, but usually it is a distance from camera IDK
	vec3 horizontal;
	vec3 vertical;
	vec3 lowerLeftCorner;
	vec3 upperLeftCorner;
	vec3 lowerRightCorner;
	vec3 upperRightCorner;
};


float HitSphere(Ray r, Sphere sphere)
{
	vec3 oc = r.origin - sphere.center;
	float a = dot(r.direction, r.direction);
	float b = 2.0 * dot(oc, r.direction);
	float c = dot(oc, oc) - sphere.radius * sphere.radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return -1;
	else
		return (-b - sqrt(discriminant)) / (2.0f * a);
}

vec3 RayAt(Ray ray, float t)
{
	return ray.origin + ray.direction * t;
}


Sphere spheres[spheresCount] = Sphere[spheresCount](
Sphere(u_sphereCenter, 1.0, vec3(1.0, 0.0, 1.0)),
Sphere(u_sphereCenter2 , 100.0, vec3(0.2, 0.3, 1.0)));

void main()
{
	float aspectRatio = u_resolution.x / u_resolution.y;
	Canvas canvas;
	canvas.height = 2.0;
	canvas.width = canvas.height * aspectRatio;
	canvas.center = vec3(0);
	canvas.focalLen = 1;
	canvas.horizontal = vec3(canvas.width, 0, 0);
	canvas.vertical = vec3(0, canvas.height, 0);
	vec4 testLL = u_model * vec4(vec3(canvas.center - canvas.horizontal / 2.0 - canvas.vertical / 2.0 + (u_camPos - vec3(0.0, 0.0, canvas.focalLen))),1.0);
	canvas.lowerLeftCorner = testLL.xyz;
	vec4 testUL = u_model * vec4(vec3(canvas.center - canvas.horizontal / 2.0 + canvas.vertical / 2.0 + (u_camPos - vec3(0.0, 0.0, canvas.focalLen))),1.0);
	canvas.upperLeftCorner = testUL.xyz;
	vec4 testUR = u_model * vec4(vec3(canvas.center + canvas.horizontal / 2.0 + canvas.vertical / 2.0 + (u_camPos - vec3(0.0, 0.0, canvas.focalLen))),1.0);
	canvas.upperRightCorner = testUR.xyz;


	Ray rayFromCamera;
	camPos = (u_model * vec4(camPos, 1)).xyz;
	rayFromCamera.origin = camPos; 
	float u = gl_FragCoord.x / (u_resolution.x - 0.5);
	float v = gl_FragCoord.y / (u_resolution.y - 0.5); 
	
	canvas.vertical = canvas.upperLeftCorner -  canvas.lowerLeftCorner;
	canvas.horizontal = -canvas.upperLeftCorner + canvas.upperRightCorner;

	rayFromCamera.direction = normalize(canvas.lowerLeftCorner + u * canvas.horizontal + v * canvas.vertical - camPos);

	int closestSphere = findClosestSphere(rayFromCamera);

	//draws closest spehere to camera
	vec3 rgb;
	bool shouldDrawSphere = (closestSphere != -1);
	if (shouldDrawSphere)
	{
		rgb = determinePiexelColor(rayFromCamera, closestSphere);
	}
	else
	{
		rgb = bgColor;
	}

	FragColor = vec4(rgb, 1.0);
}

int findClosestSphere(Ray ray)
{
	float hitDistance = FLT_MAX;
	int closestSphere = -1;
	for (int i = 0; i < spheresCount; i++)
	{
		float T = HitSphere(ray, spheres[i]);
		bool hitsSphere = T > 0;
		if (hitsSphere)
		{
			//tests if its the closest sphere to camera
			if (T < hitDistance)
			{
				hitDistance = T;
				closestSphere = i;
			}
		}
	}

	return closestSphere;
}

vec3 determinePiexelColor(Ray ray, int hittedSphere)
{
	vec3 rgb = vec3(0);
	float T = HitSphere(ray, spheres[hittedSphere]);
	vec3 N = normalize(RayAt(ray, T) - spheres[hittedSphere].center);
	vec3 lightDir = normalize(lightPos - RayAt(ray, T));
	Ray rayToLight;
	rayToLight.origin = RayAt(ray, T) + lightDir * 0.0001;
	rayToLight.direction = lightDir;
	int hitsAnotherSphere = 0;
	float shadowMultiplier = 1.0;
	for (int i = 0; i < spheresCount; i++)
	{
		if (i != hittedSphere)
		{
			float closestT = HitSphere(rayToLight, spheres[i]);
			if (closestT > 0.0)
			{
				hitsAnotherSphere += 1;
			}
		}
	}
	if (hitsAnotherSphere)
	{
		shadowMultiplier = 0.5;
	}

	float shade = dot(N, lightDir);
	rgb = spheres[hittedSphere].color * shade * shadowMultiplier + ambient;
	return rgb;
}

vec3 drawBackground(Ray ray)
{
	//linear interpolation
	float t = 0.5 * (ray.direction.y + 1.0);
	return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}